/*
 * Coral - A C++ Component Framework.
 * See Copyright Notice in Coral.h
 */

#include "ModulePart.h"
#include "LuaState.h"
#include "ModuleInstaller.h"
#include <co/System.h>
#include <co/ModuleManager.h>

ModulePart::ModulePart()
{
	LuaState::setup();
}

ModulePart::~ModulePart()
{
	LuaState::tearDown();
}

void ModulePart::initialize( co::Module* )
{
	lua::ModuleInstaller::instance().install();

	_luaModulePartLoader = new LuaModulePartLoader;
	co::getSystem()->getModules()->installLoader( _luaModulePartLoader.get() );

	/*
		Install the co.packageLoader function into Lua's package.loaders table,
		in order to enable 'require' to load Lua scripts in the CORAL_PATH.
	 */
	LuaState::doString( LuaState::getL(), "table.insert( package.loaders, 2, co.packageLoader )" );

	// force loading of our own package (lua/init.lua)
	LuaState::doString( LuaState::getL(), "require 'lua'" );
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
	// force a full garbage-collection cycle
	lua_gc( LuaState::getL(), LUA_GCCOLLECT, 0 );

	lua::ModuleInstaller::instance().uninstall();
}

CORAL_EXPORT_COMPONENT( ModulePart, lua );
CORAL_EXPORT_MODULE_PART( ModulePart );
