/*
 * Coral - A C++ Component Framework.
 * See Copyright Notice in Coral.h
 */

#include <core/tools/Dir.h>
#include <gtest/gtest.h>

#ifdef CORAL_OS_WIN32
	static std::string TEST_ABSOLUTE_PATH( "c:\\folder\\subfolder" );
	static std::string TEST_ABSOLUTE_PATH_NORMALIZED( "c:/folder/subfolder" );
	static std::string TEST_RELATIVE_PATH( "folder\\subfolder" );
#else
	static std::string TEST_ABSOLUTE_PATH( "/folder/subfolder" );
	static std::string TEST_ABSOLUTE_PATH_NORMALIZED( "/folder/subfolder" );
	static std::string TEST_RELATIVE_PATH( "folder/subfolder" );
#endif

static std::string TEST_RELATIVE_PATH_NORMALIZED( "folder/subfolder" );

TEST( DirTests, dirExists )
{
    co::Dir dir( TESTS_DATA_DIR );
    EXPECT_TRUE( dir.exists() );

	dir = co::Dir( "inexistant_dir" );
	EXPECT_FALSE( dir.exists() );

#ifdef CORAL_OS_WIN32
	dir = co::Dir( "c:" );
	EXPECT_TRUE( dir.exists() );
	EXPECT_TRUE( dir.isRoot() );

	dir = co::Dir( "c:\\" );
	EXPECT_TRUE( dir.exists() );
	EXPECT_TRUE( dir.isRoot() );

	dir = co::Dir( "c:/" );
	EXPECT_TRUE( dir.exists() );
	EXPECT_TRUE( dir.isRoot() );

	dir = co::Dir( "c://" );
	EXPECT_TRUE( dir.exists() );
	EXPECT_TRUE( dir.isRoot() );
#else
	dir = co::Dir( "/" );
	EXPECT_TRUE( dir.exists() );
	EXPECT_TRUE( dir.isRoot() );
#endif
}

TEST( DirTests, pathGetters )
{
	// Test setPath( const std::string& path ) and std::string getPath()
	co::Dir absoluteDir( TEST_ABSOLUTE_PATH );
	std::string string = absoluteDir.getPath();
	EXPECT_TRUE( co::Dir::isAbsolutePath( string ) );
	EXPECT_EQ( string.compare( TEST_ABSOLUTE_PATH_NORMALIZED ), 0 );

	co::Dir relativeDir( TEST_RELATIVE_PATH );
	string = relativeDir.getPath();
	EXPECT_EQ( string.compare( TEST_RELATIVE_PATH_NORMALIZED ), 0 );
}

TEST( DirTests, makesAndRemoves )
{
	// Test bool makeDir( const std::string& dirName ) const
	EXPECT_FALSE( co::Dir( "newDir1" ).exists() );
	EXPECT_TRUE( co::Dir::makeDir( "newDir1" ) );
	EXPECT_TRUE( co::Dir( "newDir1" ).exists() );

	// Test bool removeDir( const std::string& dirName ) const
	EXPECT_TRUE( co::Dir::removeDir( "newDir1" ) );
	EXPECT_FALSE(  co::Dir( "newDir1" ).exists() );

	// Test bool makePath( const std::string& dirPath ) const
	EXPECT_FALSE( co::Dir( "d1/d2" ).exists() );
	EXPECT_TRUE( co::Dir::makePath( "d1/d2" ) );
	EXPECT_TRUE( co::Dir( "d1/d2" ).exists() );

	// Test bool removePath( const std::string& dirPath ) const
	EXPECT_TRUE( co::Dir::removeDir( "d1/d2") );
	EXPECT_FALSE( co::Dir( "d1/d2" ).exists() );
	EXPECT_TRUE( co::Dir::removeDir( "d1") );
	EXPECT_FALSE( co::Dir( "d1" ).exists() );
}

TEST( DirTests, entryList )
{
	// test entry list
	co::Dir dir( TESTS_DATA_DIR "/csl/Global" );

	std::vector<std::string> allEntries;
	dir.getEntryList( allEntries );
	EXPECT_TRUE( allEntries.size() != 0 );

	dir = co::Dir( "inexistant_dir" );
	dir.getEntryList( allEntries );
	EXPECT_TRUE( allEntries.size() == 0 );
}
