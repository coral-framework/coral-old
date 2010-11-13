/*
 * Coral - Lightweight C++ Component Framework.
 * See Copyright Notice in Coral.h
 */

#include <co/Coral.h>
#include <co/System.h>
#include <gtest/gtest.h>

int main( int argc, char** argv )
{
	testing::InitGoogleTest( &argc, argv );

	// set up the system
	co::addPath( CORAL_PATH );
	co::getSystem()->setup();

	int res = RUN_ALL_TESTS();
	co::shutdown();

	return res;
}
