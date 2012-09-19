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
		{ "co.ISystem", "B54F2C72-1E75-05B9-BA9501AA29C7A883" },
		{ "co.IArray", "1BB2803D-41B5-E5FB-83F595457D944670" },
		{ "co.IPort", "4C7AF3B8-E7F4-152D-147ED6D3AC257BB4" },
		{ "lua.lua", "9AE6C21E-901B-C547-566A3151BE2E83B5" },
		{ "lua.Component", "4FDDFEFC-F369-1530-FB1B037DDB6A9FF4" },
		{ "co.IStruct", "8A77C9E5-210D-A546-9840491814265445" },
		{ "lua.ScriptedAnnotation", "ED12256F-E29D-D5DF-B0583EF9AAD00F8A" },
		{ "lua.Universe", "E4D2DC7A-C49A-5550-159BCBCC85417DFA" },
		{ "co.IComponent", "9C2E5576-5C64-D5A2-9F9EC6DF89E4793C" },
		{ "co.IReflector", "D04A8AF3-DA63-851B-E0447C45F29AFE2F" },
		{ "co.IDynamicTypeProvider", "DAB0D926-2435-F580-B084CF27DED023BB" },
		{ "lua.IScripted", "9606E98E-FF4D-8529-25EFA24AF8974287" },
		{ "co.IDynamicServiceProvider", "BAEA3229-C51F-45EE-97A8F8593814795A" },
		{ "lua.ModulePartLoader", "BF63A138-9E13-45BE-7D64742081FE0699" },
		{ "co.INativeClass", "60D92261-86B3-1543-D18FAAEB0AACD685" },
		{ "co.IModulePart", "5E22B362-2AEB-65D0-CE3EA3A2F8D1C0C7" },
		{ "lua.IInterceptor", "9CB962BA-CD2A-05EB-0F32E4D48309B7E5" },
		{ "co.IModulePartLoader", "D0E2D246-B76A-A533-9BE3C19E7AA7D923" },
		{ "co.IInterface", "EE11F0E0-F766-D5A0-A7FC9F1C07392F1E" },
		{ "co.IException", "EBFD6600-0AE1-D5E3-8E089A92932F0552" },
		{ "co.IEnum", "0CC59C89-F1ED-F542-22A6128827442B9B" },
		{ "lua.IState", "E39C737F-4D11-85D4-E7CD413206E9E108" },
		{ "co.INamespace", "730CE022-1D63-D554-05E09939ACF54402" },
		{ "lua.Exception", "C9E07715-CB3A-652A-0D46896EFD6FD6AB" },
		{ "lua.ModulePart", "9AE6C21E-901B-C547-566A3151BE2E83B5" },
		{ "lua.Launcher", "7CEA3981-2B07-D561-6FEAACE0CF425C00" },
		{ "lua.ILauncher", "A86B9B9C-6F82-156D-CE946C7AD3ED7B5D" },
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
