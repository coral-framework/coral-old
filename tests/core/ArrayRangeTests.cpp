/*
 * Coral - A C++ Component Framework.
 * See Copyright Notice in Coral.h
 */

#include <co/ArrayRange.h>
#include <gtest/gtest.h>

TEST( ArrayRangeTests, staticIntArray )
{
	int array[] = { 3, 5, 8, 13, 20 };

	int i = 0;
	for( co::ArrayRange<int> r( array, CORAL_ARRAY_LENGTH( array ) ); r; r.popFirst() )
	{
		EXPECT_EQ( array[i++], r.getFirst() );
	}
}

TEST( ArrayRangeTests, staticConstIntArray )
{
	const int array[] = { 3, 5, 8, 13, 20 };
	
	co::ArrayRange<const int> r( array, CORAL_ARRAY_LENGTH( array ) );
	EXPECT_EQ( r.getSize(), 5 );
	
	int i = 0;
	for( ; r; r.popFirst() )
	{
		EXPECT_EQ( array[i++], r.getFirst() );
	}
}

TEST( ArrayRangeTests, intStdVector )
{
	std::vector<int> v;
	v.push_back( 1 );
	v.push_back( 13 );
	v.push_back( 20 );
	v.push_back( 40 );

	int i = 0;
	for( co::ArrayRange<int> r( v ); r; r.popFirst() )
	{
		EXPECT_EQ( v[i++], r.getFirst() );
	}
}

TEST( ArrayRangeTests, constIntStdVector )
{
	std::vector<int> v;
	v.push_back( 1 );
	v.push_back( 13 );
	v.push_back( 20 );
	v.push_back( 40 );
	
	const std::vector<int> cv( v );
	
	int i = 0;
	for( co::ArrayRange<const int> r( cv ); r; r.popFirst() )
	{
		EXPECT_EQ( v[i++], r.getFirst() );
	}
}

TEST( ArrayRangeTests, stringStdVector )
{
	std::vector<std::string> strList;
	strList.push_back( "Um" );
	strList.push_back( "Dois" );
	strList.push_back( "Tres" );

	co::ArrayRange<std::string> r( strList );

	EXPECT_EQ( r.getLast(), "Tres" );
	r.popLast();

	EXPECT_EQ( r.getLast(), "Dois" );
	r.popLast();

	EXPECT_EQ( r.getLast(), "Um" );
	r.popLast();

	EXPECT_TRUE( r.isEmpty() );
}
