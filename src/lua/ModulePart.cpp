/*
 * Coral - Lightweight C++ Component Framework
 * See Copyright Notice in Coral.h
 */

#include "lua_Base.h"
#include "LuaState.h"
#include "ModuleInstaller.h"
#include "LuaModulePartLoader.h"
#include <co/RefPtr.h>
#include <co/ISystem.h>
#include <co/IModule.h>
#include <co/IModuleManager.h>
#include <co/IServiceManager.h>
#include <lua/Exception.h>

namespace lua {

/*!
	The Lua module's co.IModulePart.
 */
class ModulePartComponent : public lua::lua_Base
{
public:
    ModulePartComponent()
	{
		LuaState::setup();
	}

	virtual ~ModulePartComponent()
	{
		// empty
	}

	void initialize( co::IModule* module )
	{
		lua::ModuleInstaller::instance().install();

		// generally update this module AFTER all other modules
		module->setRank( 100000 );

		// install our LuaModulePartLoader
		_luaModulePartLoader = new IModulePartLoader;
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
		co::getSystem()->getServices()->addServiceProvider( co::typeOf<lua::IState>::get(), "lua.Universe" );
	}

	void integrate( co::IModule* )
	{
		// empty
	}

	void integratePresentation( co::IModule* )
	{
		// empty
	}

	void disintegrate( co::IModule* )
	{
		co::getSystem()->getModules()->uninstallLoader( _luaModulePartLoader.get() );

		/*
			This is our last chance to call __gc metamethods that require
			reflectors, since reflectors are no longer available in the
			dispose() phase.			
		 */
		lua_gc( LuaState::getL(), LUA_GCCOLLECT, 0 );
	}

	void dispose( co::IModule* )
	{
		lua::ModuleInstaller::instance().uninstall();
		LuaState::tearDown();
	}

private:
	co::RefPtr<IModulePartLoader> _luaModulePartLoader;
};
	
CORAL_EXPORT_COMPONENT( ModulePartComponent, lua );
CORAL_EXPORT_MODULE_PART( ModulePartComponent );

} // namespace lua
