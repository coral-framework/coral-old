/*
 * Coral - A C++ Component Framework.
 * See Copyright Notice in Coral.h
 */

#include "LuaLauncher.h"
#include "LuaState.h"
#include <lua/Exception.h>
#include <sstream>

LuaLauncher::LuaLauncher()
{
	// empty
}

LuaLauncher::~LuaLauncher()
{
	// empty
}

void LuaLauncher::main( co::ArrayRange<std::string const> args )
{
	if( args.isEmpty() )
		throw lua::Exception( "lua.Launcher must receive a Lua script name as argument" );

	std::string filename;
	if( !LuaState::searchScriptFile( args.getFirst(), filename ) )
		CORAL_THROW( lua::Exception, "launch script '" << args.getFirst() << "' could not be found in the path" );

	lua_State* L = LuaState::getL();
	LuaState::loadFile( L, filename );
	
	args.popFirst();
	co::int32 numArgs = args.getSize();

	for( ; args; args.popFirst() )
		LuaState::push( L, args.getFirst() );

	LuaState::call( L, numArgs, 0 );
}

CORAL_EXPORT_COMPONENT( LuaLauncher, Launcher );
