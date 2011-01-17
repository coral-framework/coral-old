/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#include "TestHelper.h"
#include <co/reserved/OS.h>
#include <gtest/gtest.h>

TEST( OSTests, isFile )
{
	EXPECT_TRUE( co::OS::isFile( TESTS_DATA_DIR "/misc/file1.jpg" ) );
	EXPECT_FALSE( co::OS::isFile( TESTS_DATA_DIR "/misc" ) );
}

TEST( OSTests, isDir )
{
	EXPECT_FALSE( co::OS::isDir( TESTS_DATA_DIR "/misc/file1.jpg" ) );
	EXPECT_TRUE( co::OS::isDir( TESTS_DATA_DIR "/misc" ) );
}

TEST( OSTests, isAbs )
{
	EXPECT_FALSE( co::OS::isAbs( "something" ) );
	EXPECT_FALSE( co::OS::isAbs( "../something" ) );

	std::string cwd;
	co::OS::getCurrentWorkingDir( cwd );
	EXPECT_TRUE( co::OS::isAbs( cwd ) );
}

TEST( OSTests, getApplicationDir )
{
	std::string appPath;
	co::OS::getApplicationDir( appPath );

	appPath.push_back( CORAL_OS_DIR_SEP );
	appPath.append( "tests_core" );
#ifndef CORAL_NDEBUG
	appPath.append( "_debug" );
#endif

	EXPECT_TRUE( co::OS::isFile( appPath ) );
}

TEST( OSTests, normalizePath )
{
	std::string path, expectedRes( "A" CORAL_OS_DIR_SEP_STR "B" );

	path = "A//B";
	co::OS::normalizePath( path );
	EXPECT_EQ( path, expectedRes );

	path = "A/B/";
	co::OS::normalizePath( path );
	EXPECT_EQ( path, expectedRes );

	path = "A/./B";
	co::OS::normalizePath( path );
	EXPECT_EQ( path, expectedRes );

	path = "A/foo/../B";
	co::OS::normalizePath( path );
	EXPECT_EQ( path, expectedRes );
}

TEST( OSTests, makeAbs )
{
	std::string path( "./foo/.." );
	std::string cwd;
	co::OS::makeAbs( path );
	co::OS::getCurrentWorkingDir( cwd );
	EXPECT_EQ( path, cwd );
}

TEST( OSTests, searchFile2 )
{
	std::vector<std::string> coralPath;
	coralPath.push_back( TESTS_DATA_DIR "/misc" );

	std::string filename, res;

	filename = "file1.csl";
	EXPECT_TRUE( co::OS::searchFile2( coralPath, co::ArrayRange<const std::string>( &filename, 1 ), res ) );
	EXPECT_TRUE( TestHelper::stringEndsWith( res,  "/file1.csl" ) );

	filename = "/file1.csl";
	EXPECT_TRUE( co::OS::searchFile2( coralPath, co::ArrayRange<const std::string>( &filename, 1 ), res ) );
	EXPECT_TRUE( TestHelper::stringEndsWith( res,  "/file1.csl" ) );

	filename = "/file1.jpg";
	EXPECT_TRUE( co::OS::searchFile2( coralPath, co::ArrayRange<const std::string>( &filename, 1 ), res ) );
	EXPECT_TRUE( TestHelper::stringEndsWith( res,  "/file1.jpg" ) );

	filename = "dot.separated.folder/file1.csl";
	EXPECT_TRUE( co::OS::searchFile2( coralPath, co::ArrayRange<const std::string>( &filename, 1 ), res ) );

	filename = "dot.separated.folder.file1.csl";
	EXPECT_FALSE( co::OS::searchFile2( coralPath, co::ArrayRange<const std::string>( &filename, 1 ), res ) );

	filename = "innerFolder/file1.csl";
	EXPECT_TRUE( co::OS::searchFile2( coralPath, co::ArrayRange<const std::string>( &filename, 1 ), res ) );

	filename = "innerFolder/folder.csl";
	EXPECT_FALSE( co::OS::searchFile2( coralPath, co::ArrayRange<const std::string>( &filename, 1 ), res ) );
}

TEST( STests, searchFile2WithAmbiguities )
{
	std::vector<std::string> coralPath;
	coralPath.push_back( TESTS_DATA_DIR "/misc" );
	coralPath.push_back( TESTS_DATA_DIR "/misc/innerFolder" );

	std::string filename, res;

	filename = "file1.csl";
	EXPECT_TRUE( co::OS::searchFile2( coralPath, co::ArrayRange<const std::string>( &filename, 1 ), res ) );
	EXPECT_TRUE( TestHelper::stringEndsWith( res,  "misc/file1.csl" ) );

	filename = "file1.jpg";
	EXPECT_TRUE( co::OS::searchFile2( coralPath, co::ArrayRange<const std::string>( &filename, 1 ), res ) );
	EXPECT_TRUE( TestHelper::stringEndsWith( res,  "misc/file1.jpg" ) );

	filename = "file2.csl";
	EXPECT_TRUE( co::OS::searchFile2( coralPath, co::ArrayRange<const std::string>( &filename, 1 ), res ) );
	EXPECT_TRUE( TestHelper::stringEndsWith( res,  "innerFolder/file2.csl" ) );

	// invert the path order
	std::swap( coralPath[0], coralPath[1] );
	filename = "file1.csl";
	EXPECT_TRUE( co::OS::searchFile2( coralPath, co::ArrayRange<const std::string>( &filename, 1 ), res ) );
	EXPECT_TRUE( TestHelper::stringEndsWith( res,  "innerFolder/file1.csl" ) );
}
