/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#include "lua_Base.h"
#include "LuaState.h"
#include "ModuleInstaller.h"
#include "LuaModulePartLoader.h"
#include <co/RefPtr.h>
#include <co/System.h>
#include <co/Module.h>
#include <co/ModuleManager.h>
#include <co/ServiceManager.h>
#include <lua/Exception.h>

/*!
	The Lua module's co.ModulePart.
 */
class ModulePart : public lua::lua_Base
{
public:
    ModulePart()
	{
		LuaState::setup();
	}

	virtual ~ModulePart()
	{
		// empty
	}

	void initialize( co::Module* module )
	{
		lua::ModuleInstaller::instance().install();
		
		// generally update this module AFTER all other modules
		module->setRank( 100000 );
		
		// install our LuaModulePartLoader
		_luaModulePartLoader = new LuaModulePartLoader;
		co::getSystem()->getModules()->installLoader( _luaModulePartLoader.get() );
		
		/*
			Manually load our Lua module ("lua/__init.lua"), which will in turn
			install our loader function into Lua's package.loaders table, in order
			to enable 'require' to load Lua scripts in the CORAL_PATH.
		 */
		std::string initScriptFileName;
		lua_State* L = LuaState::getL();
		if( !LuaState::findScript( L, "lua", initScriptFileName ) )
			throw lua::Exception( "could not find the Lua module's init script in the Coral path" );
		
		// call the 'lua/__init.lua' script passing in its filename as an arg
		LuaState::loadFile( L, initScriptFileName );
		LuaState::push( L, initScriptFileName );
		LuaState::call( L, 1, 0 );
		
		// create and register the global lua.IState service
		co::getSystem()->getServices()->addServiceImplementation( co::typeOf<lua::IState>::get(), "lua.Universe" );
	}

	void integrate( co::Module* )
	{
		// empty
	}

	void integratePresentation( co::Module* )
	{
		// empty
	}

	void disintegrate( co::Module* )
	{
		co::getSystem()->getModules()->uninstallLoader( _luaModulePartLoader.get() );
	}

	void dispose( co::Module* )
	{
		lua::ModuleInstaller::instance().uninstall();
		LuaState::tearDown();
	}

private:
	co::RefPtr<LuaModulePartLoader> _luaModulePartLoader;
};

CORAL_EXPORT_COMPONENT( ModulePart, lua );

namespace lua {
	CORAL_EXPORT_MODULE_PART( ModulePart );
}
