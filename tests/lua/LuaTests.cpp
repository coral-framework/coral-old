/*
 * Coral - A C++ Component Framework.
 * See Copyright Notice in Coral.h
 */

#include <gtest/gtest.h>
#include <co/Coral.h>
#include <co/System.h>
#include <co/ModuleManager.h>
#include <co/ModuleLoadException.h>

#define ASSERT_SUCCESS( moduleName ) \
	try { co::getSystem()->getModules()->load( moduleName ); } \
	catch( const std::exception& e ) { FAIL() << e.what(); }

#define ASSERT_ERROR( moduleName, expectedMsg ) \
	try { \
		co::getSystem()->getModules()->load( moduleName ); \
		FAIL() << "an exception should have been raised"; \
	} catch( const std::exception& e ) { \
		std::string raisedMsg( e.what() ); \
		if( raisedMsg.find( expectedMsg ) == std::string::npos ) \
			FAIL() << "raised message (\"" << raisedMsg << "\") does not contain the expected message (\"" \
				<< expectedMsg << "\")"; \
	}

// --- Initialization --- //

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

// --- Sanity Tests --- //

TEST( LuaTests, moduleInvalidScriptName )
{
	ASSERT_ERROR( "lua.moduleInvalidScriptName",
					"none of the module loaders recognized 'lua.moduleInvalidScriptName' as a module" );
}

TEST( LuaTests, moduleInvalidReturn )
{
	ASSERT_ERROR( "lua.moduleInvalidReturn", "the module script must return a table" );
}

TEST( LuaTests, moduleValid )
{
	ASSERT_SUCCESS( "lua.moduleValid" );
}

TEST( LuaTests, assertEq )
{
	ASSERT_ERROR( "lua.assertEq", "ASSERT_EQ failed: 1 != 2" );
}

TEST( LuaTests, assertErrorNoError )
{
	ASSERT_ERROR( "lua.assertErrorNoError", "ASSERT_ERROR failed: no error was raised, though one was expected" );
}

TEST( LuaTests, assertErrorMismatch )
{
	ASSERT_ERROR( "lua.assertErrorMismatch", "does not contain the expected message ('expected message')" );
}

TEST( LuaTests, assertTrue )
{
	ASSERT_ERROR( "lua.assertTrue", "ASSERT_TRUE failed (nil value)" );
}

// --- Package Tests --- //

TEST( LuaTests, coPackage )
{
	ASSERT_SUCCESS( "lua.coPackage" );
}

// --- Component Binding Tests --- //

TEST( LuaTests, componentManipulation )
{
	ASSERT_SUCCESS( "lua.componentManipulation" );
}

// --- Interface Binding Tests --- //

TEST( LuaTests, interfaceAttributes )
{
	ASSERT_SUCCESS( "lua.interfaceAttributes" );
}

TEST( LuaTests, interfaceMethods )
{
	ASSERT_SUCCESS( "lua.interfaceMethods" );
}

// --- Native Class Binding Tests --- //

TEST( LuaTests, nativeClass )
{
	ASSERT_SUCCESS( "lua.nativeClass" );
}

// --- Struct Binding Tests --- //

TEST( LuaTests, struct )
{
	ASSERT_SUCCESS( "lua.struct" );
}
