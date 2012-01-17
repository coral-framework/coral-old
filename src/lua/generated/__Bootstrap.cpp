/*
 * Coral - Lightweight C++ Component Framework
 * See Copyright Notice in Coral.h
 */

#include <co/RefPtr.h>
#include <co/IModulePart.h>
#include <co/reserved/LibraryManager.h>

extern "C" CORAL_DLL_EXPORT
const char* coral_module_query_verification_data()
{
	return  "pattern=CORAL_MODULE_VERIFICATION_DATA\n"
			"version=0.8.0\n"
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
		{ "co.IComponent", "1FC6C64B-8889-5545-F0FEF53D0C771366" },
		{ "co.INamespace", "D074B541-8F18-E5EA-C468679917559F15" },
		{ "co.IEnum", "5E677A1B-D72E-65E6-3152FE0C89D35A04" },
		{ "co.IArray", "C4EF23C2-9126-65F1-B8896633476A851B" },
		{ "lua.IState", "9F7DD2A8-EBC2-F5D8-7F1DADA10C3B1393" },
		{ "co.ISystem", "B54F2C72-1E75-05B9-BA9501AA29C7A883" },
		{ "lua.ModulePart", "9AE6C21E-901B-C547-566A3151BE2E83B5" },
		{ "lua.ILauncher", "A86B9B9C-6F82-156D-CE946C7AD3ED7B5D" },
		{ "co.IException", "C28E53B1-F6BE-D583-7743614CC0151B54" },
		{ "lua.lua", "9AE6C21E-901B-C547-566A3151BE2E83B5" },
		{ "lua.Exception", "C9E07715-CB3A-652A-0D46896EFD6FD6AB" },
		{ "lua.Component", "4FDDFEFC-F369-1530-FB1B037DDB6A9FF4" },
		{ "lua.ModulePartLoader", "BF63A138-9E13-45BE-7D64742081FE0699" },
		{ "co.IInterface", "7694B75D-08D0-A58A-3F257B30C23A201C" },
		{ "lua.ScriptedAnnotation", "ED12256F-E29D-D5DF-B0583EF9AAD00F8A" },
		{ "lua.Universe", "070E307E-BFA7-05DA-FF0D81B226284A97" },
		{ "lua.IScripted", "9606E98E-FF4D-8529-25EFA24AF8974287" },
		{ "co.IDynamicTypeProvider", "DAB0D926-2435-F580-B084CF27DED023BB" },
		{ "co.IModulePartLoader", "D0E2D246-B76A-A533-9BE3C19E7AA7D923" },
		{ "co.IDynamicServiceProvider", "BAEA3229-C51F-45EE-97A8F8593814795A" },
		{ "co.IModulePart", "5E22B362-2AEB-65D0-CE3EA3A2F8D1C0C7" },
		{ "co.INativeClass", "1BC29C8D-407C-F5A5-06C69016CF24AD0D" },
		{ "co.IStruct", "02619569-5B28-A5A2-1C8B401F2C5A338D" },
		{ "co.IReflector", "D04A8AF3-DA63-851B-E0447C45F29AFE2F" },
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

	// module retain/release mismatch?
	assert( sg_refCount >= 0 );
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
