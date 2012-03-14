/*
 * Coral - Lightweight C++ Component Framework
 * See Copyright Notice in Coral.h
 */

#include <gtest/gtest.h>
#include <co/Coral.h>
#include <co/IObject.h>
#include <co/ISystem.h>
#include <co/IModuleManager.h>
#include <co/ModuleLoadException.h>
#include <co/MissingInputException.h>
#include <co/IllegalStateException.h>
#include <co/IllegalArgumentException.h>
#include <lua/IState.h>
#include <lua/Exception.h>

#include <moduleA/IHuman.h>

#define ASSERT_SUCCESS( moduleName ) \
	try { co::getSystem()->getModules()->load( moduleName ); } \
	catch( const std::exception& e ) { FAIL() << e.what(); }

#define ASSERT_EXCEPTION( code, expectedType, expectedMsg ) \
	try { code; FAIL() << "an exception should have been raised"; } \
	catch( const expectedType & e ) { \
		std::string raisedMsg( e.what() ); \
		if( raisedMsg.find( expectedMsg ) == std::string::npos ) \
			FAIL() << "raised message (\"" << raisedMsg << "\") does not contain the expected message (\"" \
				<< expectedMsg << "\")"; \
	} catch( std::exception& e ) { FAIL() << "wrong exception type (" #expectedType " expected): " << e.what(); }

#define ASSERT_LUA_EXCEPTION( code, expectedMsg ) \
	ASSERT_EXCEPTION( code, lua::Exception, expectedMsg )

#define ASSERT_ERROR( moduleName, expectedMsg ) \
	ASSERT_EXCEPTION( co::getSystem()->getModules()->load( moduleName ), co::ModuleLoadException, expectedMsg )


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
	ASSERT_ERROR( "lua.assertErrorNoError",
					"ASSERT_ERROR failed: no error was raised, though one was expected" );
}

TEST( LuaTests, assertErrorMismatch )
{
	ASSERT_ERROR( "lua.assertErrorMismatch",
					"does not contain the expected message (\"expected message\")" );
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

// --- Array Binding Tests --- //

TEST( LuaTests, array )
{
	ASSERT_SUCCESS( "lua.array" );
}

// --- Component Binding Tests --- //

TEST( LuaTests, componentManipulation )
{
	ASSERT_SUCCESS( "lua.componentManipulation" );
}

// --- Interface Binding Tests --- //

TEST( LuaTests, interfaceFields )
{
	ASSERT_SUCCESS( "lua.interfaceFields" );
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

// --- Lua Component Tests --- //

TEST( LuaTests, component )
{
	ASSERT_SUCCESS( "lua.component" );

	// test if __init() is called when instantiating the component from C++
	co::RefPtr<co::IObject> object = co::newInstance( "lua.bat.Component" );
	moduleA::IHuman* batman = object->getService<moduleA::IHuman>();
	ASSERT_TRUE( batman != NULL );
	EXPECT_EQ( "Batman", batman->getName() );
}

// --- In/Out Any/Array/Struct Parameter Tests --- //

TEST( LuaTests, inOutParams )
{
	ASSERT_SUCCESS( "lua.inOutParams" );
}

// --- Services Tests --- //

TEST( LuaTests, services )
{
	ASSERT_SUCCESS( "lua.services" );
}

// --- lua::IState Tests --- //

TEST( LuaTests, IState )
{
	ASSERT_TRUE( co::getService<lua::IState>() != NULL );
}

TEST( LuaTests, findScript )
{
	co::RefPtr<lua::IState> luaState = co::getService<lua::IState>();
	std::string filename;

	ASSERT_TRUE( luaState->findScript( "lua.test", filename ) );
	ASSERT_FALSE( luaState->findScript( "lua.doesNotExist", filename ) );
}

TEST( LuaTests, callFunction )
{
	co::RefPtr<lua::IState> luaState = co::getService<lua::IState>();

	co::int32 intVec[4] = { -1, -1, -1, -1 };

	co::int32 numRes;
	co::Any args[4];
	co::Any results[6];
	co::Range<co::Any> resRange( results, 6 );

	/*
		Test calling a function returned by a module.
		lua.plain.sum returns the summation of its arguments.
	 */
	args[0].set( 1 );
	args[1].set( 2 );
	args[2].set( 3 );
	args[3].set( 5 );
	results[0].set<co::int32&>( intVec[0] );
	results[1].set<co::int32&>( intVec[1] );
	results[2].set<co::int32&>( intVec[2] );
	results[3].set<co::int32&>( intVec[3] );

	ASSERT_NO_THROW( numRes = luaState->callFunction( "lua.scripts.sum", "",
									co::Range<co::Any>( args, 4 ), resRange ) );

	ASSERT_EQ( 1, numRes );
	ASSERT_EQ( 11, intVec[0] );

	/*
		Test calling functions from a table returned by a module.
	 */

	// passthrough() simply returns all of its args.
	ASSERT_NO_THROW( numRes = luaState->callFunction( "lua.scripts.manyFunctions", "passthrough",
									co::Range<co::Any>( args, 4 ), resRange ) );

	ASSERT_EQ( 4, numRes );
	ASSERT_EQ( 1, intVec[0] );
	ASSERT_EQ( 2, intVec[1] );
	ASSERT_EQ( 3, intVec[2] );
	ASSERT_EQ( 5, intVec[3] );

	// constants() returns some constants.
	bool b1, b2;
	double d1;
	std::string str1, str2;
	co::Any any1, any2;

	results[0].set<co::Any&>( any1 );
	results[1].set<bool&>( b1 );
	results[2].set<bool&>( b2 );
	results[3].set<double&>( d1 );
	results[4].set<std::string&>( str1 );
	results[5].set<co::Any&>( any2 );

	ASSERT_NO_THROW( numRes = luaState->callFunction( "lua.scripts.manyFunctions", "constants",
									co::Range<co::Any>(), resRange ) );

	ASSERT_EQ( 6, numRes );
	ASSERT_TRUE( !any1.isValid() );
	ASSERT_EQ( true, b1 );
	ASSERT_EQ( false, b2 );
	ASSERT_EQ( 3.14, d1 );
	ASSERT_EQ( "hello", str1 );
	ASSERT_EQ( co::getSystem(), any2.get<co::IService*>() );

	// test prunning the results
	results[0].set<std::string&>( str1 );
	results[1].set<std::string&>( str2 );

	ASSERT_NO_THROW( numRes = luaState->callFunction( "lua.scripts.manyFunctions", "constants2",
						 co::Range<co::Any>(), co::Range<co::Any>( results, 2 ) ) );

	ASSERT_EQ( 5, numRes );
	ASSERT_EQ( "one", str1 );
	ASSERT_EQ( "two", str2 );
}

TEST( LuaTests, callFunctionErrors )
{
	co::RefPtr<lua::IState> luaState = co::getService<lua::IState>();
	co::Range<co::Any> empty;

	ASSERT_LUA_EXCEPTION( luaState->callFunction( "lua.scripts.doesNotExist", "nope", empty, empty ),
							"module 'lua.scripts.doesNotExist' not found" );

	ASSERT_LUA_EXCEPTION( luaState->callFunction( "lua.scripts.sum", "notATable", empty, empty ),
							"module did not return a table" );

	ASSERT_LUA_EXCEPTION( luaState->callFunction( "lua.scripts.manyFunctions", "", empty, empty ),
							"attempt to call a table value" );

	ASSERT_LUA_EXCEPTION( luaState->callFunction( "lua.scripts.manyFunctions", "doesNotExist", empty, empty ),
							"no such function" );

	ASSERT_LUA_EXCEPTION( luaState->callFunction( "lua.scripts.manyFunctions", "raiseError", empty, empty ),
							"here is the error" );
}

TEST( LuaTests, exceptionTypes )
{
	co::RefPtr<lua::IState> luaState = co::getService<lua::IState>();

	std::string exceptionType;
	std::string message;
	co::Any args[2];
	args[0].set<const std::string&>( exceptionType );
	args[1].set<const std::string&>( message );

	co::Range<co::Any> empty;
	co::Range<co::Any> argRange( args, 2 );

	exceptionType = "no type given";
	message = "ignored";
	ASSERT_EXCEPTION( luaState->callFunction( "lua.scripts.manyFunctions", "raiseError", argRange, empty ),
						lua::Exception, "no type given" );

	exceptionType = "co.IllegalArgumentException";
	message = "and here is the msg";
	ASSERT_EXCEPTION( luaState->callFunction( "lua.scripts.manyFunctions", "raise", argRange, empty ),
						co::IllegalArgumentException, "and here is the msg" );

	exceptionType = "co.MissingInputException";
	message = "message here";
	ASSERT_EXCEPTION( luaState->callFunction( "lua.scripts.manyFunctions", "raise", argRange, empty ),
						co::MissingInputException, "message here" );

	exceptionType = "invalid type";
	message = "message";
	ASSERT_EXCEPTION( luaState->callFunction( "lua.scripts.manyFunctions", "raise", argRange, empty ),
						lua::Exception, "error throwing an exception of type 'invalid type' from Lua" );
}

TEST( LuaTests, preserveExceptionType )
{
	co::RefPtr<lua::IState> luaState = co::getService<lua::IState>();

	co::Range<co::Any> empty;

	ASSERT_EXCEPTION( luaState->callFunction( "lua.scripts.manyFunctions",
		"causeIllegalStateException", empty, empty ), co::IllegalStateException, "not SystemState_None" );
}
