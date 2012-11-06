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
		{ "lua.Exception", "E494AEF9-CB92-959B-260612E42B651281" },
		{ "lua.Launcher", "7F834A71-EAAB-D59E-B3033EF5A65E5039" },
		{ "co.IField", "9795DB95-8D67-E508-6E5AF800303700E1" },
		{ "lua.Component", "E497DE0A-29D3-751C-AE3781ABED8FF944" },
		{ "lua.ILauncher", "23A662E5-3C8C-8557-87D2FE9D63065340" },
		{ "co.IDynamicTypeProvider", "F28777B9-4350-A550-56A76D2AB610247D" },
		{ "lua.Universe", "9CDB6763-A7BD-F50D-70BD8BC6AD9A9F2C" },
		{ "co.IPort", "65863F78-A86F-C5C9-F9FD1D9FF2DC0C03" },
		{ "lua.IInterceptor", "DDD3E015-D99A-7514-8AD032842617EFFA" },
		{ "co.IMethod", "DD79F0D9-9B81-85BC-F778809E7586E7D4" },
		{ "co.IException", "11848E85-CD76-A57A-4CBAB44044405112" },
		{ "co.IModulePartLoader", "8B980092-72DF-55EC-62E80071B0C8F068" },
		{ "lua.ScriptedAnnotation", "5F5A08CA-6992-25AD-BE3668A0DE92ED66" },
		{ "lua.ModulePart", "01D65DFD-7AA0-052B-DD549599474CDA86" },
		{ "co.IObject", "3D4B59E2-C5B2-853B-A6CCCDAB8623F1D5" },
		{ "co.IService", "9B285842-61DB-6571-316E3EF1BCEE9050" },
		{ "co.IArray", "1264C28F-1DD1-951B-A9672AFD459C3D0F" },
		{ "co.IModulePart", "B9E33D80-2B2C-8569-8DCF30F02B0FA556" },
		{ "co.IDynamicServiceProvider", "0551D753-11FE-55B8-18F2586276A17951" },
		{ "co.INativeClass", "FADE38B0-CF60-95A5-BBAE7A2EA3DD4566" },
		{ "co.IComponent", "0C4F615C-80B2-052E-176E9A2A6C67EB3B" },
		{ "co.IInterface", "764D8BD7-EE9A-5557-4F69DD4A003CABFB" },
		{ "lua.ModulePartLoader", "DDF98C90-C138-35CD-E18539A3465C9798" },
		{ "co.IEnum", "81E5B221-0C25-0581-2FC36F6018D01CAD" },
		{ "co.INamespace", "B8D7E517-710A-65B0-C2F038DEAE33D03C" },
		{ "co.IStruct", "E8C514C7-7FC3-45F5-7B17D74B0E2D2D8C" },
		{ "lua.IState", "EFAF47D2-645C-75F5-CF472FB7BE502604" },
		{ "lua.lua", "01D65DFD-7AA0-052B-DD549599474CDA86" },
		{ "lua.IScripted", "83A12D1D-7486-A5A4-91AB2CD71C3C73B4" },
		{ "co.IReflector", "0C8E90FF-D9DB-259F-EACD322FE8FC2322" },
		{ "co.ISystem", "CB0DACE2-D193-3523-559D84D4449BFC23" },
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
