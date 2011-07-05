/*
 * Coral - Lightweight C++ Component Framework
 * See Copyright Notice in Coral.h
 */

#include "LuaModulePart.h"
#include "LuaState.h"
#include <co/IModule.h>

namespace lua {

ModulePart::ModulePart()
{
	lua_State* L = LuaState::getL();
	assert( lua_type( L, -1 ) == LUA_TTABLE );

	// save the module table in the registry indexed by our own pointer
	lua_pushlightuserdata( L, this );
	lua_pushvalue( L, -2 );
	lua_rawset( L, LUA_REGISTRYINDEX );

	lua_pop( L, 1 );
}

ModulePart::~ModulePart()
{
	// if the Lua module was not torn down yet...
	lua_State* L = LuaState::getL();
	if( L )
	{
		// clean our registry entry (i.e. the module table)
		lua_pushlightuserdata( L, this );
		lua_pushnil( L );
		lua_rawset( L, LUA_REGISTRYINDEX );
	}
}

void ModulePart::initialize( co::IModule* module )
{
	callScriptMethod( "initialize", module );
}

void ModulePart::integrate( co::IModule* module )
{
	callScriptMethod( "integrate", module );
}

void ModulePart::integratePresentation( co::IModule* module )
{
	callScriptMethod( "integratePresentation", module );
}

void ModulePart::disintegrate( co::IModule* module )
{
	callScriptMethod( "disintegrate", module );
}

void ModulePart::dispose( co::IModule* )
{
	/*
		Ignored. For simplicity, Lua modules don't directly handle 'dispose'
		events, as it could cause major inter-dependence problems during the
		system tear-down sequence. Not a real problem since the 'dispose'
		event is easily replaced by the 'disintegrate' event in combination
		with the Lua garbage collector.
	 */
}

void ModulePart::callScriptMethod( const char* methodName, co::IModule* module )
{
	lua_State* L = LuaState::getL();
	assert( L );

	// reserve stack space for the method function
	lua_pushnil( L );

	// get the module table
	lua_pushlightuserdata( L, this );
	lua_rawget( L, LUA_REGISTRYINDEX );

	assert( lua_type( L, -1 ) == LUA_TTABLE );

	// try to get the method function
	lua_getfield( L, -1, methodName );
	if( !lua_isfunction( L, -1 ) )
	{
		// method not defined, skip this call
		lua_pop( L, 3 );
		return;
	}

	// move the method function below the module table
	lua_replace( L, -3 );

	// push the 'module' argument and call the method
	LuaState::push( L, module );
	LuaState::call( L, 2, 0 );
}

CORAL_EXPORT_COMPONENT( ModulePart, ModulePart );

} // namespace lua
