/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#include <co/RefPtr.h>
#include <co/IModulePart.h>
#include <co/reserved/LibraryManager.h>

extern "C" CORAL_DLL_EXPORT
const char* coral_module_query_verification_data()
{
	return  "pattern=CORAL_MODULE_VERIFICATION_DATA\n"
			"version=0.6.0\n"
			"buildkey=" CORAL_BUILD_KEY "\n"
			"buildmode=" CORAL_BUILD_MODE "\n";
}

struct TypeDependency
{
	const char* name;
	const char* binarySignature;
};

extern "C" CORAL_DLL_EXPORT
const TypeDependency* coral_module_query_dependencies()
{
	static const TypeDependency s_dependencies[] = {
		{ "lua.Component", "AF454F48-55CB-0516-915C1940A06B3A5B" },
		{ "co.IArrayType", "2BFE916D-9C54-0525-4F9F6167B1DFE344" },
		{ "lua.lua", "03A87259-7370-E5FE-7ABFE3B1F6BAE469" },
		{ "lua.ModulePart", "03A87259-7370-E5FE-7ABFE3B1F6BAE469" },
		{ "co.IModulePart", "9AEAF23D-97F7-7500-00CA0EF11E9ADC90" },
		{ "co.IExceptionType", "ADE7F1DC-3FC5-15DB-CCB1891FAF6A6642" },
		{ "lua.ILauncher", "8133E010-22EA-B55A-90591F019265425D" },
		{ "co.IDynamicProxyHandler", "1C8CE65D-246B-F514-F19EFD0C268F5B8B" },
		{ "co.IModulePartLoader", "677A483E-1073-E567-C9B40CA63BC34B82" },
		{ "lua.Universe", "D2AC4C53-8C1E-C550-8861CDFB71770ED9" },
		{ "co.INativeClassType", "173F02BB-D96E-2514-5EE3FE91AE668DFE" },
		{ "co.IComponentType", "AA98203F-135A-2560-079020893681795B" },
		{ "lua.Launcher", "38F26CA9-7E66-35CF-F0233D4E42B000F8" },
		{ "co.IStructType", "9ED1574E-81B7-55D5-588DD9CCD7963776" },
		{ "co.ISystem", "4A33AF0B-D818-35B9-09A4080EABB96D66" },
		{ "co.IReflector", "E34CAAD5-F5B6-F5CB-CA8D71506D23BD96" },
		{ "co.INamespace", "7762B1DA-748C-C54C-DE06D6861FF68B22" },
		{ "co.IEnumType", "88ED98CD-E6ED-45F4-5959DF1BC07E6C04" },
		{ "co.IInterfaceType", "B557FFDA-4913-75E4-B24E5C627261F0AE" },
		{ "lua.ModulePartLoader", "A88972FE-8C9F-556F-9C7DAF80792E6695" },
		{ "lua.Exception", "C9E07715-CB3A-652A-0D46896EFD6FD6AB" },
		{ "lua.IState", "0586479B-FEE0-057A-6CE2AB7232163A8D" },
		{ NULL, NULL }
	};
	return s_dependencies;
}

namespace lua {

// The module's IModulePart instance
co::RefPtr<co::IModulePart> sg_instance;

// The module's internal reference count
co::int32 sg_refCount( 0 );

void moduleRetain()
{
	++sg_refCount;
}

void moduleRelease()
{
	// is the module's IModulePart the only active reference?
	if( --sg_refCount == 1 )
	{
		assert( sg_instance.isValid() );
		co::LibraryManager::release( sg_instance.get() );
	}
}

// implemented by CORAL_EXPORT_MODULE_PART()
co::IModulePart* createModulePart();

} // namespace lua

extern "C" CORAL_DLL_EXPORT
co::IModulePart* coral_module_part_instance()
{
	if( !lua::sg_instance.isValid() )
	{
		assert( lua::sg_refCount == 0 );
		lua::sg_instance = lua::createModulePart();
	}
	return lua::sg_instance.get();
}
