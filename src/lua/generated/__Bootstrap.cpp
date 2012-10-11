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
		{ "lua.IScripted", "E2234C73-D3C5-E5EF-5E130AD22E6A7255" },
		{ "lua.lua", "33126FCD-4A1B-25F5-C28BD44710B08D3E" },
		{ "co.IMethod", "21742B65-6549-25D6-7664E237A86EDCE0" },
		{ "lua.Exception", "44A2D558-E75B-0579-BD0A8E2F6C46FC60" },
		{ "co.IField", "98E507DF-0556-450B-3F62F9242F58583A" },
		{ "co.IDynamicTypeProvider", "23BBC11B-3656-6500-448251A29187C699" },
		{ "co.IComponent", "B4ABBD5F-812C-F5F2-1944C17F8735D43A" },
		{ "co.IModulePartLoader", "9075DE37-37F1-55C8-8BA1DBBAF510624E" },
		{ "co.IDynamicServiceProvider", "3BF19D6B-8C23-85F3-60FEBC2C6D23F42E" },
		{ "lua.IInterceptor", "1D69C30B-758C-25A0-2D77EEA3A2B9F444" },
		{ "lua.IState", "DBBBE2AC-EE50-8513-BB29D23E89E7D107" },
		{ "co.IException", "04061BB6-CDA9-C50A-3CB80142C9F2A37E" },
		{ "co.ISystem", "3E872C94-8AE4-E5DA-0DD7B7FD650F83CA" },
		{ "lua.ModulePart", "33126FCD-4A1B-25F5-C28BD44710B08D3E" },
		{ "co.IEnum", "CA386CCE-7511-657B-6BB3BBAFA5503176" },
		{ "co.INativeClass", "0CF38360-6E1F-05D0-0F691037BE16B957" },
		{ "co.IService", "AEB2CF39-393F-05CA-4870E7C0201E4CE0" },
		{ "co.INamespace", "4F82EB1F-A466-E548-0B1CF6DDD19EFDC0" },
		{ "lua.Launcher", "774B9046-07BF-25F8-A248BECDA438A4F4" },
		{ "lua.Component", "2D1365D2-CED5-A5AB-221A6F6DF36F60AC" },
		{ "co.IInterface", "A83EAF23-D3D5-85E5-E26089AC61F63B7E" },
		{ "lua.ModulePartLoader", "8F9C3050-9457-C579-E0EC1F80D37102E7" },
		{ "lua.ILauncher", "DD550A6A-74D3-B578-B5E8FDCC81C81BF9" },
		{ "lua.ScriptedAnnotation", "753A9E8F-3B3D-E552-CAA04892E0A1E6C4" },
		{ "lua.Universe", "08705042-3C50-5580-EFBD3985E2ADE929" },
		{ "co.IStruct", "B1FD9176-0878-55AF-14CDF43E4B8F9213" },
		{ "co.IArray", "CBB66D11-89BD-7542-532E2E7E105779AA" },
		{ "co.IObject", "2CE8BF58-187C-A522-B156CBFA0ACC7C2A" },
		{ "co.IModulePart", "D4A2AA26-42CC-45E7-F0F4A3AE4EE97335" },
		{ "co.IReflector", "7FA325C7-A770-7543-0AC61654E60830CC" },
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
