/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#include "LuaModulePartLoader.h"
#include "LuaState.h"
#include <co/Coral.h>
#include <co/ModuleLoadException.h>
#include <co/IllegalArgumentException.h>
#include <co/reserved/OS.h>
#include <lua/Exception.h>

namespace lua {

IModulePartLoader::IModulePartLoader()
{
	// empty
}

IModulePartLoader::~IModulePartLoader()
{
	// empty
}

bool IModulePartLoader::canLoadModulePart( const std::string& moduleName )
{
	return locateModuleLibrary( moduleName );
}

co::IModulePart* IModulePartLoader::loadModulePart( const std::string& moduleName )
{
	std::string libraryFilename;
	if( !locateModuleLibrary( moduleName, &libraryFilename ) )
		throw co::ModuleLoadException( "unexpected missing module library file" );

	lua_State* L = LuaState::getL();
	LuaState::loadFile( L, libraryFilename );
	LuaState::call( L, 0, 1 );

	// we expect the module script to return a table
	if( lua_type( L, -1 ) != LUA_TTABLE )
	{
		lua_pop( L, 1 );
		throw lua::Exception( "the module script must return a table" );
	}

	return new ModulePart;
}

bool IModulePartLoader::locateModuleLibrary( const std::string& moduleName, std::string* filename )
{
	const char* moduleBaseName = moduleName.c_str();
	size_t lastDotPos = moduleName.rfind( '.' );
	if( lastDotPos != std::string::npos )
		moduleBaseName += ( lastDotPos + 1 );

	std::string filePath;
	filePath.reserve( moduleName.length() + ( moduleBaseName - moduleName.c_str() ) + 5 );
	filePath = moduleName;
	co::OS::convertDotsToDirSeps( filePath );
	filePath.push_back( CORAL_OS_DIR_SEP );
	filePath.append( moduleBaseName );
	filePath.append( ".lua" );

	return co::OS::searchFile2( co::getPaths(), co::Range<const std::string>( &filePath, 1 ),
									filename ? *filename : filePath );
}

CORAL_EXPORT_COMPONENT( IModulePartLoader, ModulePartLoader );

} // namespace lua
