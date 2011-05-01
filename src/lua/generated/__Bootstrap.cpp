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
		{ "lua.IState", "9F7DD2A8-EBC2-F5D8-7F1DADA10C3B1393" },
		{ "lua.ILauncher", "A86B9B9C-6F82-156D-CE946C7AD3ED7B5D" },
		{ "co.IArray", "ABB6991E-8D48-F5AF-ECF802B8DA098698" },
		{ "lua.Launcher", "7CEA3981-2B07-D561-6FEAACE0CF425C00" },
		{ "lua.ModulePartLoader", "BF63A138-9E13-45BE-7D64742081FE0699" },
		{ "lua.lua", "9AE6C21E-901B-C547-566A3151BE2E83B5" },
		{ "co.IReflector", "D04A8AF3-DA63-851B-E0447C45F29AFE2F" },
		{ "lua.Universe", "070E307E-BFA7-05DA-FF0D81B226284A97" },
		{ "lua.ModulePart", "9AE6C21E-901B-C547-566A3151BE2E83B5" },
		{ "co.IModulePartLoader", "D0E2D246-B76A-A533-9BE3C19E7AA7D923" },
		{ "co.IModulePart", "5E22B362-2AEB-65D0-CE3EA3A2F8D1C0C7" },
		{ "co.INativeClass", "257E62F1-F669-2546-8402FD98287546B9" },
		{ "co.IService", "3ADFC1E4-9CA3-A59F-234C904E4E8B8E5C" },
		{ "co.IException", "C9CB55E6-E218-5537-B0ACD3496727AA27" },
		{ "co.IComponent", "6BDEAEE5-610E-45AA-E7E28CAE40BC58D0" },
		{ "co.IInterface", "1685C953-A509-6551-DAB9111474E37BC8" },
		{ "co.IStruct", "036DBE88-35E0-F50C-5FCD93C7A457EE7B" },
		{ "co.ISystem", "B54F2C72-1E75-05B9-BA9501AA29C7A883" },
		{ "lua.Component", "4FDDFEFC-F369-1530-FB1B037DDB6A9FF4" },
		{ "lua.Exception", "C9E07715-CB3A-652A-0D46896EFD6FD6AB" },
		{ "co.IEnum", "784CE5B2-AAD9-9525-0B73A0E68C220B89" },
		{ "co.INamespace", "64C954C9-209B-9569-9B32F1628D1E26EB" },
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
