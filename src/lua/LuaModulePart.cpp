/*
 * Coral - A C++ Component Framework.
 * See Copyright Notice in Coral.h
 */

#include "LuaModulePart.h"
#include "LuaState.h"
#include <co/Module.h>

LuaModulePart::LuaModulePart()
{
	lua_State* L = LuaState::getL();
	assert( lua_type( L, -1 ) == LUA_TTABLE );

	// save the module table in the registry indexed by our 'this' pointer
	lua_pushlightuserdata( L, this );
	lua_pushvalue( L, -2 );
	lua_rawset( L, LUA_REGISTRYINDEX );

	lua_pop( L, 1 );
}

LuaModulePart::~LuaModulePart()
{
	lua_State* L = LuaState::getL();

	// clear our registry reference to the module table
	lua_pushlightuserdata( L, this );
	lua_pushnil( L );
	lua_rawset( L, LUA_REGISTRYINDEX );
}

void LuaModulePart::initialize( co::Module* module )
{
	callScriptMethod( "initialize", module );
}

void LuaModulePart::integrate( co::Module* module )
{
	callScriptMethod( "integrate", module );
}

void LuaModulePart::integratePresentation( co::Module* module )
{
	callScriptMethod( "integratePresentation", module );
}

void LuaModulePart::disintegrate( co::Module* module )
{
	callScriptMethod( "disintegrate", module );
}

void LuaModulePart::dispose( co::Module* module )
{
	callScriptMethod( "dispose", module );
}

void LuaModulePart::callScriptMethod( const char* methodName, co::Module* module )
{
	lua_State* L = LuaState::getL();

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

CORAL_EXPORT_COMPONENT( LuaModulePart, ModulePart );
