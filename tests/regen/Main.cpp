/*
 * Coral - A C++ Component Framework.
 * See Copyright Notice in Coral.h
 */

#include <co/Coral.h>
#include <gtest/gtest.h>

int main( int argc, char** argv )
{
	testing::InitGoogleTest( &argc, argv );

	co::addPath( CORAL_PATH );

	int res = RUN_ALL_TESTS();
	co::shutdown();

	return res;
}
