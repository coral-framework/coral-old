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

static void fillArgs( co::AnyValue* any, size_t count, size_t dim )
{
	const int NUM_DIMS = 2;
	static std::string sStrings[NUM_DIMS];
	static co::CSLError sCSLErrors[NUM_DIMS];
	static co::Uuid sUuids[NUM_DIMS];

	assert( count <= MAX_ARGS && dim < NUM_DIMS );

	if( count-- ) *any++ = 3.14;
	if( count-- ) *any++ = true;
	if( count-- ) *any++ = -8;
	if( count-- ) *any++ = sStrings[dim];
	if( count-- ) *any++ = co::getSystem()->getTypes();
	if( count-- ) *any++ = sCSLErrors[dim];
	if( count-- ) *any++ = co::getPaths();
	if( count-- ) *any++ = sUuids[dim];
}

TEST( PerformanceTests, callLuaFunction )
{
	const int NUM_ARGS = MAX_ARGS;
	co::AnyValue args[NUM_ARGS];
	co::AnyValue results[NUM_ARGS];

	std::string moduleName( "PerformanceTests" );
	std::string functionName( "passthrough" );

	co::getSystem()->getModules()->load( "lua" );

	fillArgs( args, NUM_ARGS, 0 );
	fillArgs( results, NUM_ARGS, 1 );

	lua::IState* state = co::getService<lua::IState>();

	co::Range<co::AnyValue> argsRange( args, NUM_ARGS );
	co::Range<co::AnyValue> resultsRange( results, NUM_ARGS );

	for( int i = 0; i < 1000; ++i )
		state->call( moduleName, functionName, argsRange, resultsRange );
}

// --- Sanity Tests --- //

TEST( PerformanceTests, test )
{
	// dummy
}

