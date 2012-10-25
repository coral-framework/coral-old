/*
 * Coral - Lightweight C++ Component Framework
 * See Copyright Notice in Coral.h
 */

#include <co/Slice.h>
#include <gtest/gtest.h>

TEST( RangeTests, staticIntArray )
{
	int array[] = { 3, 5, 8, 13, 20 };

	int i = 0;
	for( co::Slice<int> r( array ); r; r.popFirst() )
	{
		EXPECT_EQ( array[i++], r.getFirst() );
	}
}

TEST( RangeTests, staticConstIntArray )
{
	const int array[] = { 3, 5, 8, 13, 20 };
	
	co::Slice<int> r( array );
	EXPECT_EQ( r.getSize(), 5 );

	int i = 0;
	for( ; r; r.popFirst() )
	{
		EXPECT_EQ( array[i++], r.getFirst() );
	}
}

TEST( RangeTests, intStdVector )
{
	std::vector<int> v;
	v.push_back( 1 );
	v.push_back( 13 );
	v.push_back( 20 );
	v.push_back( 40 );

	int i = 0;
	for( co::Slice<int> r( v ); r; r.popFirst() )
	{
		EXPECT_EQ( v[i++], r.getFirst() );
	}
}

TEST( RangeTests, constIntStdVector )
{
	std::vector<int> v;
	v.push_back( 1 );
	v.push_back( 13 );
	v.push_back( 20 );
	v.push_back( 40 );
	
	const std::vector<int> cv( v );
	
	int i = 0;
	for( co::Slice<int> r( cv ); r; r.popFirst() )
	{
		EXPECT_EQ( v[i++], r.getFirst() );
	}
}

TEST( RangeTests, stringStdVector )
{
	std::vector<std::string> strList;
	strList.push_back( "Um" );
	strList.push_back( "Dois" );
	strList.push_back( "Tres" );

	co::Slice<std::string> r( strList );

	EXPECT_EQ( r.getLast(), "Tres" );
	r.popLast();

	EXPECT_EQ( r.getLast(), "Dois" );
	r.popLast();

	EXPECT_EQ( r.getLast(), "Um" );
	r.popLast();

	EXPECT_TRUE( r.isEmpty() );
}

/*
	Test the use of Ranges with incomplete types.
 */
struct IncompleteType;
static co::Slice<IncompleteType> create();
static co::Slice<IncompleteType> passthrough( co::Slice<IncompleteType> range );
static void test( co::Slice<IncompleteType> res );

TEST( RangeTests, incompleteType )
{
	test( passthrough( create() ) );
}

struct IncompleteType { double d; IncompleteType( double d ) : d( d ) {} };

static co::Slice<IncompleteType> create()
{
	static IncompleteType array[] = { IncompleteType( 1 ), IncompleteType( 2 ), IncompleteType( 3 ) };
	return co::Slice<IncompleteType>( array, 3 );
}

static co::Slice<IncompleteType> passthrough( co::Slice<IncompleteType> input )
{
	return input;
}

static void test( co::Slice<IncompleteType> res )
{
	EXPECT_EQ( 3, res.getSize() );
}
