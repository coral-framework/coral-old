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
		{ "co.IEnum", "C1250460-AA85-1510-A61EFA75509993CB" },
		{ "lua.Exception", "C9E07715-CB3A-652A-0D46896EFD6FD6AB" },
		{ "co.IReflector", "DA34141A-0D66-C5D4-CDA4E6AA582260FC" },
		{ "co.IArray", "C1FCF916-45B7-1511-98932F0D28BC51E4" },
		{ "co.IModulePart", "CB548771-6C0A-45B6-E9F0207AF936AC15" },
		{ "co.IStruct", "C25F86B0-E323-15BF-4AECD51DC532F424" },
		{ "lua.IState", "9529EB1E-9DA8-45F2-6C43D5E358519C52" },
		{ "co.INamespace", "CEB28153-1F6B-2538-50A568374FD1F8D6" },
		{ "lua.ModulePartLoader", "D9375C32-7792-050C-62AEDED4A8AFEE94" },
		{ "lua.Universe", "674466DC-7676-E5EA-9DB58A350F3EC6D9" },
		{ "co.IModulePartLoader", "4039DBC0-D7BC-1523-99E65E433B5CF4B7" },
		{ "lua.ModulePart", "6930F72B-F675-65A9-FBEAB7CC3864A621" },
		{ "co.INativeClass", "C7C7D7B9-7325-4548-503DE811FE50EAC0" },
		{ "co.IComponent", "0F24538E-092C-653D-285B27026D34A5F7" },
		{ "lua.Component", "6C1E0C83-DB27-15BC-2F1A10C7F4159A47" },
		{ "co.IInterface", "D05CF844-8764-652F-7402E39931D3FA31" },
		{ "co.IException", "1ABB4B23-7F3C-259E-7515248C15295385" },
		{ "lua.Launcher", "7EEBA634-5B15-C5A5-CD0944D063FA039B" },
		{ "co.ISystem", "84729AE8-9359-755F-563BA534495FF53C" },
		{ "lua.lua", "6930F72B-F675-65A9-FBEAB7CC3864A621" },
		{ "lua.ILauncher", "E06FF2D5-D7A6-B534-86822F346C67B288" },
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
