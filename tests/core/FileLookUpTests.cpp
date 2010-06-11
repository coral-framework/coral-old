/*
 * Coral - A C++ Component Framework.
 * See Copyright Notice in Coral.h
 */

#include "TestHelper.h"
#include <co/reserved/FileLookUp.h>
#include <gtest/gtest.h>

static const std::string DATA_DIR( TESTS_DATA_DIR );
static const std::vector<std::string> CSL_EXTENSION( 1, "csl" );
static const std::vector<std::string> JPG_EXTENSION( 1, "jpg" );
static const std::vector<std::string> PNG_EXTENSION( 1, "png" );

TEST( FileLookUpTests, lookup )
{
	std::vector<std::string> coralPath;
	coralPath.push_back( DATA_DIR + "/FileLookUp" );

	std::string filename;
	std::string relativePath;

	{
		co::FileLookUp fileLookUp( coralPath, CSL_EXTENSION );
		fileLookUp.addFilePath( "file1" );
		ASSERT_TRUE( fileLookUp.locate( filename, NULL, &relativePath ) );
		ASSERT_TRUE( TestHelper::stringEndsWith( filename,  "/file1.csl" ) );
	}
	{
		co::FileLookUp fileLookUp( coralPath, CSL_EXTENSION );
		fileLookUp.addFilePath( "/file1" );
		ASSERT_TRUE( fileLookUp.locate( filename, NULL, &relativePath ) );
		ASSERT_TRUE( TestHelper::stringEndsWith( filename,  "/file1.csl" ) );
	}
	{
		co::FileLookUp fileLookUp( coralPath, JPG_EXTENSION );
		fileLookUp.addFilePath( "file1" );
		ASSERT_TRUE( fileLookUp.locate( filename, NULL, &relativePath ) );
		ASSERT_TRUE( TestHelper::stringEndsWith( filename,  "/file1.jpg" ) );
	}
	{
		co::FileLookUp fileLookUp( coralPath, CSL_EXTENSION );
		fileLookUp.addFilePath( "dot.separated.folder/file1" );
		ASSERT_TRUE( fileLookUp.locate( filename, NULL, &relativePath ) );
	}
	{
		co::FileLookUp fileLookUp( coralPath, CSL_EXTENSION );
		fileLookUp.addFilePath( "dot.separated.folder.file1" );
		ASSERT_FALSE( fileLookUp.locate( filename, NULL, &relativePath ) );
	}
	{
		co::FileLookUp fileLookUp( coralPath, CSL_EXTENSION );
		fileLookUp.addFilePath( "innerFolder.file1", true );
		ASSERT_TRUE( fileLookUp.locate( filename, NULL, &relativePath ) );
	}
	{
		co::FileLookUp fileLookUp( coralPath, CSL_EXTENSION );
		fileLookUp.addFilePath( "innerFolder/file1" );
		ASSERT_TRUE( fileLookUp.locate( filename, NULL, &relativePath ) );
	}
	{
		co::FileLookUp fileLookUp( coralPath, CSL_EXTENSION );
		fileLookUp.addFilePath( "innerFolder/folder" );
		ASSERT_FALSE( fileLookUp.locate( filename, NULL, &relativePath ) );
	}
}

TEST( FileLookUpTests, lookupWithAmbiguities )
{
	std::vector<std::string> coralPath;
	coralPath.push_back( DATA_DIR + "/FileLookUp" );
	coralPath.push_back( DATA_DIR + "/FileLookUp/innerFolder" );

	std::string filename;
	std::string relativePath;
	
	{
		co::FileLookUp fileLookUp( coralPath, CSL_EXTENSION );
		fileLookUp.addFilePath( "file1" );
		ASSERT_TRUE( fileLookUp.locate( filename, NULL, &relativePath ) );
		ASSERT_TRUE( TestHelper::stringEndsWith( filename, "FileLookUp/file1.csl" ) );
	}
	{
		co::FileLookUp fileLookUp( coralPath, JPG_EXTENSION );
		fileLookUp.addFilePath( "file1" );
		ASSERT_TRUE( fileLookUp.locate( filename, NULL, &relativePath ) );
		ASSERT_TRUE( TestHelper::stringEndsWith( filename, "FileLookUp/file1.jpg" ) );
	}
	{
		co::FileLookUp fileLookUp( coralPath, CSL_EXTENSION );
		fileLookUp.addFilePath( "file2" );
		ASSERT_TRUE( fileLookUp.locate( filename, NULL, &relativePath ) );
		ASSERT_TRUE( TestHelper::stringEndsWith( filename, "innerFolder/file2.csl" ) );
	}

	// invert the path order
	std::swap( coralPath[0], coralPath[1] );
	{
		co::FileLookUp fileLookUp( coralPath, CSL_EXTENSION );
		fileLookUp.addFilePath( "file1" );
		ASSERT_TRUE( fileLookUp.locate( filename, NULL, &relativePath ) );
		ASSERT_TRUE( TestHelper::stringEndsWith( filename, "innerFolder/file1.csl" ) );
	}
}
