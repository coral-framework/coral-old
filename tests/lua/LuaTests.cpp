/*
 * Coral - A C++ Component Framework.
 * See Copyright Notice in Coral.h
 */

#include <gtest/gtest.h>
#include <co/Coral.h>
#include <co/System.h>
#include <co/ModuleManager.h>
#include <co/ModuleLoadException.h>

#define EXPECT_NO_ERROR( moduleName ) \
	try { co::getSystem()->getModules()->load( moduleName ); } \
	catch( const std::exception& e ) { FAIL() << "Unexpected exception: " << e.what(); }

#define EXPECT_ERROR( moduleName, expectedMsg ) \
	try { \
		co::getSystem()->getModules()->load( moduleName ); \
		FAIL() << "an exception should have been raised"; \
	} catch( const std::exception& e ) { \
		std::string raisedMsg( e.what() ); \
		if( raisedMsg.find( expectedMsg ) == std::string::npos ) \
			FAIL() << "Raised message (\"" << raisedMsg << "\") does not contain the expected message (\"" \
				<< expectedMsg << "\")"; \
	}

TEST( LuaTests, setup )
{
	// list of required modules
	std::vector<std::string> requiredModules;
	requiredModules.push_back( "lua" );

	try
	{
		co::getSystem()->setup( requiredModules );
	}
	catch( std::exception& e )
	{
		FAIL() << "Could not setup() the system: " << e.what();
	}
}

TEST( LuaTests, moduleInvalidScriptName )
{
	EXPECT_ERROR( "lua.moduleInvalidScriptName",
					"none of the module loaders recognized 'lua.moduleInvalidScriptName' as a module" );
}

TEST( LuaTests, moduleInvalidReturn )
{
	EXPECT_ERROR( "lua.moduleInvalidReturn", "the module script must return a table" );
}

TEST( LuaTests, moduleValid )
{
	EXPECT_NO_ERROR( "lua.moduleValid" );
}

TEST( LuaTests, assertionTest )
{
	EXPECT_ERROR( "lua.assertionTest", "my assertion message" );
}

TEST( LuaTests, coPackage )
{
	EXPECT_NO_ERROR( "lua.coPackage" );
}

TEST( LuaTests, interfaceAttributes )
{
	EXPECT_NO_ERROR( "lua.interfaceAttributes" );
}

TEST( LuaTests, interfaceAttribReadOnly )
{
	EXPECT_ERROR( "lua.interfaceAttribReadOnly", "attribute 'state' is read-only and cannot be changed" );
}

TEST( LuaTests, interfaceMethods )
{
	EXPECT_NO_ERROR( "lua.interfaceMethods" );
}
