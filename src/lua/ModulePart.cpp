/*
 * Coral - A C++ Component Framework.
 * See Copyright Notice in Coral.h
 */

#include "ModulePart.h"
#include "LuaState.h"
#include "ModuleInstaller.h"
#include <co/System.h>
#include <co/Module.h>
#include <co/ModuleManager.h>
#include <lua/Exception.h>

ModulePart::ModulePart()
{
	LuaState::setup();
}

ModulePart::~ModulePart()
{
	// empty
}

void ModulePart::initialize( co::Module* module )
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
}

void ModulePart::integrate( co::Module* )
{
	// empty
}

void ModulePart::integratePresentation( co::Module* )
{
	// empty
}

void ModulePart::disintegrate( co::Module* )
{
	co::getSystem()->getModules()->uninstallLoader( _luaModulePartLoader.get() );
}

void ModulePart::dispose( co::Module* )
{
	lua::ModuleInstaller::instance().uninstall();
	LuaState::tearDown();
}

CORAL_EXPORT_COMPONENT( ModulePart, lua );

namespace lua {
	CORAL_EXPORT_MODULE_PART( ModulePart );
}
