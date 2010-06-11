/*
 * Coral - A C++ Component Framework.
 * See Copyright Notice in Coral.h
 */

#include <co/Coral.h>
#include <gtest/gtest.h>
#include <glog/logging.h>
#include <gflags/gflags.h>

int main( int argc, char** argv )
{
	testing::InitGoogleTest( &argc, argv );

	google::ParseCommandLineFlags( &argc, &argv, true );

	// initialize Google's logging library
	google::InitGoogleLogging( argv[0] );

	co::addPath( CORAL_PATH );

	int res = RUN_ALL_TESTS();
	co::shutdown();

	return res;
}
