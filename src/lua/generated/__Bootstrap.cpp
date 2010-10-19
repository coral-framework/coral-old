/*
 * Coral - A C++ Component Framework.
 * See Copyright Notice in Coral.h
 */

#include <co/RefPtr.h>
#include <co/ModulePart.h>
#include <co/reserved/LibraryManager.h>

extern "C" CORAL_DLL_EXPORT
const char* coral_module_query_verification_data()
{
	return  "pattern=CORAL_MODULE_VERIFICATION_DATA\n"
			"version=0.5.0\n"
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
		{ "co.ModulePartLoader", "536A73D9-F4C8-F597-2FEB68C301937B8D" },
		{ "co.Namespace", "4FD40C32-7A5E-75F8-05DDEE996453EF13" },
		{ "co.ExceptionType", "A42F28BA-50B8-9593-346CABF6B6F93BD6" },
		{ "co.System", "C6E47013-73A8-D554-C221496BDAE2909C" },
		{ "co.EnumType", "1847C34A-33F1-05F8-859BC2523056F718" },
		{ "lua.Exception", "C9E07715-CB3A-652A-0D46896EFD6FD6AB" },
		{ "co.NativeClassType", "E06B94AA-0639-6524-D24AD3260F041B58" },
		{ "co.ArrayType", "F29E701E-02C4-05EF-5678229EC3653D1C" },
		{ "co.ComponentType", "0B216603-8B57-1533-A9B552990562FB3C" },
		{ "co.Reflector", "0AA79840-442C-E518-35F14DC5DA07D79A" },
		{ "lua.ModulePart", "CC83CD39-BDBB-A5E4-1B99D37111BCF066" },
		{ "co.ModulePart", "A34C715E-954E-15CD-2B04800BA19B5052" },
		{ "co.InterfaceType", "22FF144F-E1B5-1593-CA7DF461B59AB85C" },
		{ "lua.ILauncher", "A7FCD03C-4219-A565-4232A36DF1E9BD58" },
		{ "co.StructType", "10C22B97-2CDD-E5E9-6696678A6EAF792F" },
		{ "lua.IState", "66505261-0E1B-2578-26DEB02C5163926F" },
		{ "lua.ModulePartLoader", "6D1199C9-3749-0528-1D4385C12BE804F5" },
		{ "lua.Launcher", "27D80973-9B0D-E5AC-FC3862D589367372" },
		{ "lua.Universe", "CC1C2016-2A07-A536-0DE330A04496CC74" },
		{ "lua.lua", "CC83CD39-BDBB-A5E4-1B99D37111BCF066" },
		{ "lua.Component", "93663B91-8382-6537-A2BCF4D6D0CEAAC5" },
		{ NULL, NULL }
	};
	return s_dependencies;
}

namespace lua {

// The module's ModulePart instance
co::RefPtr<co::ModulePart> sg_instance;

// The module's internal reference count
co::int32 sg_refCount( 0 );

void moduleRetain()
{
	++sg_refCount;
}

void moduleRelease()
{
	// is the module's ModulePart the only active reference?
	if( --sg_refCount == 1 )
	{
		assert( sg_instance.isValid() );
		co::LibraryManager::release( sg_instance.get() );
	}
}

} // namespace lua

// __createModulePart() is implemented by CORAL_EXPORT_MODULE_PART()
co::ModulePart* __createModulePart();

extern "C" CORAL_DLL_EXPORT
co::ModulePart* coral_module_part_instance()
{
	if( !lua::sg_instance.isValid() )
	{
		assert( lua::sg_refCount == 0 );
		lua::sg_instance = __createModulePart();
	}
	return lua::sg_instance.get();
}
