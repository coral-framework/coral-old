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
		{ "lua.Launcher", "7CEA3981-2B07-D561-6FEAACE0CF425C00" },
		{ "co.IException", "C9CB55E6-E218-5537-B0ACD3496727AA27" },
		{ "lua.lua", "9AE6C21E-901B-C547-566A3151BE2E83B5" },
		{ "co.INamespace", "64C954C9-209B-9569-9B32F1628D1E26EB" },
		{ "lua.Exception", "C9E07715-CB3A-652A-0D46896EFD6FD6AB" },
		{ "lua.Component", "03AF8EB9-639E-C593-930E77EEAC947EAA" },
		{ "co.IComponent", "6BDEAEE5-610E-45AA-E7E28CAE40BC58D0" },
		{ "co.INativeClass", "257E62F1-F669-2546-8402FD98287546B9" },
		{ "co.ISystem", "B54F2C72-1E75-05B9-BA9501AA29C7A883" },
		{ "co.IModulePartLoader", "D0E2D246-B76A-A533-9BE3C19E7AA7D923" },
		{ "co.IEnum", "784CE5B2-AAD9-9525-0B73A0E68C220B89" },
		{ "lua.ModulePartLoader", "BF63A138-9E13-45BE-7D64742081FE0699" },
		{ "co.IReflector", "0351892F-B6B6-45A1-533EC0A8932F0051" },
		{ "co.IArray", "ABB6991E-8D48-F5AF-ECF802B8DA098698" },
		{ "lua.Universe", "7BFF1F48-78EC-85D9-353A354C78D8B518" },
		{ "co.IInterface", "C2F313F1-B164-C587-AF3C60751319C983" },
		{ "lua.ModulePart", "9AE6C21E-901B-C547-566A3151BE2E83B5" },
		{ "co.IModulePart", "5E22B362-2AEB-65D0-CE3EA3A2F8D1C0C7" },
		{ "co.IStruct", "036DBE88-35E0-F50C-5FCD93C7A457EE7B" },
		{ "co.IDynamicServiceProvider", "BAEA3229-C51F-45EE-97A8F8593814795A" },
		{ "lua.ILauncher", "A86B9B9C-6F82-156D-CE946C7AD3ED7B5D" },
		{ "lua.IState", "6AFBAE75-323B-95CB-1F49103D4CA8111F" },
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
