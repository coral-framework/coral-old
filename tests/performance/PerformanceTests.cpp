/*
 * Coral - Lightweight C++ Component Framework
 * See Copyright Notice in Coral.h
 */

#include <co/Uuid.h>
#include <co/Coral.h>
#include <co/ISystem.h>
#include <co/CSLError.h>
#include <co/ITypeManager.h>
#include <co/IModuleManager.h>
#include <co/reserved/Uuid.h>
#include <lua/IState.h>
#include <lua/Exception.h>
#include <gtest/gtest.h>

const int MAX_ARGS = 8;

static void fillArgs( co::Any* any, size_t count, size_t dim )
{
	const int NUM_DIMS = 2;
	static std::string sStrings[NUM_DIMS];
	static co::CSLError sCSLErrors[NUM_DIMS];
	static co::Uuid sUuids[NUM_DIMS];

	assert( count <= MAX_ARGS && dim < NUM_DIMS );

	if( count-- ) any++->set( 3.14 );
	if( count-- ) any++->set( true );
	if( count-- ) any++->set<short>( -8 );
	if( count-- ) any++->set<std::string&>( sStrings[dim] );
	if( count-- ) any++->set( co::getSystem()->getTypes() );
	if( count-- ) any++->set<co::CSLError&>( sCSLErrors[dim] );
	if( count-- ) any++->set( co::getPaths() );
	if( count-- ) any++->set<co::Uuid&>( sUuids[dim] );
}

TEST( PerformanceTests, callLuaFunction )
{
	const int NUM_ARGS = MAX_ARGS;
	co::Any args[NUM_ARGS];
	co::Any results[NUM_ARGS];

	std::string moduleName( "PerformanceTests" );
	std::string functionName( "passthrough" );

	co::getSystem()->getModules()->load( "lua" );

	fillArgs( args, NUM_ARGS, 0 );
	fillArgs( results, NUM_ARGS, 1 );

	lua::IState* state = co::getService<lua::IState>();
	for( int i = 0; i < 1000; ++i )
	{
		state->callFunction( moduleName, functionName,
				co::Range<co::Any>( args, NUM_ARGS ),
				co::Range<co::Any>( results, NUM_ARGS ) );
	}
}

// --- Sanity Tests --- //

TEST( PerformanceTests, test )
{
	// dummy
}

