/*
 * Coral - A C++ Component Framework.
 * See Copyright Notice in Coral.h
 */

#include "LuaModulePartLoader.h"
#include "LuaState.h"
#include <co/Coral.h>
#include <co/ModuleLoadException.h>
#include <co/IllegalArgumentException.h>
#include <co/reserved/FileLookUp.h>
#include <lua/Exception.h>

LuaModulePartLoader::LuaModulePartLoader()
{
	// empty
}

LuaModulePartLoader::~LuaModulePartLoader()
{
	// empty
}

bool LuaModulePartLoader::canLoadModulePart( const std::string& moduleName )
{
	return locateModuleLibrary( moduleName );
}

co::ModulePart* LuaModulePartLoader::loadModulePart( const std::string& moduleName )
{
	std::string libraryFilename;
	if( !locateModuleLibrary( moduleName, &libraryFilename ) )
		throw co::ModuleLoadException( "unexpected missing module library file" );

	LuaState& luaState = LuaState::instance();
	luaState.loadFile( libraryFilename );
	luaState.call( 0, 1 );

	// we expect the module script to return a table
	lua_State* L = luaState.get();
	if( lua_type( L, -1 ) != LUA_TTABLE )
	{
		lua_pop( L, 1 );
		throw lua::Exception( "the module script must return a table" );
	}

	return new LuaModulePart;
}

bool LuaModulePartLoader::locateModuleLibrary( const std::string& moduleName, std::string* filename )
{
	static const std::string s_extension( "lua" );

	const char* moduleBaseName = moduleName.c_str();
	size_t lastDotPos = moduleName.rfind( '.' );
	if( lastDotPos != std::string::npos )
		moduleBaseName += ( lastDotPos + 1 );

	co::FileLookUp fileLookUp( co::getPaths(), co::ArrayRange<const std::string>( &s_extension, 1 ) );

	std::string filePath;
	filePath.reserve( moduleName.length() + ( moduleBaseName - moduleName.c_str() ) + 1 );
	filePath = moduleName;
	filePath.push_back( '.' );
	filePath.append( moduleBaseName );
	fileLookUp.addFilePath( filePath, true );

	return fileLookUp.locate( filename ? *filename : filePath, NULL, NULL );
}

CORAL_EXPORT_COMPONENT( LuaModulePartLoader, ModulePartLoader );
