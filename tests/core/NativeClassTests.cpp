/*
 * Coral - Lightweight C++ Component Framework
 * See Copyright Notice in Coral.h
 */

#include "CSLTester.h"

#include <gtest/gtest.h>

TEST( NativeClassTests, validNativeClasses )
{
	CSL_TEST( "NativeClassTests.TestNativeClass1" );
	CSL_TEST( "NativeClassTests.TestNativeClass2" );
	CSL_TEST( "NativeClassTests.TestNativeClass3" );
	CSL_TEST( "NativeClassTests.TestNativeClass4" );
	CSL_TEST( "NativeClassTests.TestNativeClass5" );
	CSL_TEST( "NativeClassTests.TestNativeClass6" );
	CSL_TEST( "NativeClassTests.TestNativeClass7" );
	CSL_TEST( "NativeClassTests.TestNativeClass8" );
	CSL_TEST( "NativeClassTests.TestNativeClass9" );
	CSL_TEST( "NativeClassTests.TestNativeClass10" );
}

TEST( NativeClassTests, emptyNativeClass )
{
	CSL_TEST( "NativeClassTests.EmptyNativeClass" );
}

TEST( NativeClassTests, invalidNativeClasses )
{
	CSL_TEST_BEGIN( "NativeClassTests.TestNativeClass_invalid" )
	CSL_EXPECT_SYNTAX_ERROR( "TestNativeClass_invalid.csl", 1 )
	CSL_TEST_END()

	CSL_TEST_BEGIN( "NativeClassTests.TestNativeClass_invalid2" )
	CSL_EXPECT_SYNTAX_ERROR( "TestNativeClass_invalid2.csl", 1 )
	CSL_TEST_END()

	CSL_TEST_BEGIN( "NativeClassTests.TestNativeClass_invalid3" )
	CSL_EXPECT_SYNTAX_ERROR( "TestNativeClass_invalid3.csl", 1 )
	CSL_TEST_END()

	CSL_TEST_BEGIN( "NativeClassTests.TestNativeClass_invalid4" )
	CSL_EXPECT_SYNTAX_ERROR( "TestNativeClass_invalid4.csl", 1 )
	CSL_TEST_END()

	CSL_TEST_BEGIN( "NativeClassTests.TestNativeClass_invalid5" )
	CSL_EXPECT_SYNTAX_ERROR( "TestNativeClass_invalid5.csl", 1 )
	CSL_TEST_END()

	CSL_TEST_BEGIN( "NativeClassTests.TestNativeClass_invalid6" )
	CSL_EXPECT_SYNTAX_ERROR( "TestNativeClass_invalid6.csl", 1 )
	CSL_TEST_END()

	CSL_TEST_BEGIN( "NativeClassTests.TestNativeClass_invalid7" )
	CSL_EXPECT_SYNTAX_ERROR( "TestNativeClass_invalid7.csl", 1 )
	CSL_TEST_END()

	CSL_TEST_BEGIN( "NativeClassTests.TestNativeClass_invalid8" )
	CSL_EXPECT_SYNTAX_ERROR( "TestNativeClass_invalid8.csl", 1 )
	CSL_TEST_END()

	CSL_TEST_BEGIN( "NativeClassTests.TestNativeClass_invalid9" )
	CSL_EXPECT_SYNTAX_ERROR( "TestNativeClass_invalid9.csl", 1 )
	CSL_TEST_END()

	CSL_TEST_BEGIN( "NativeClassTests.TestNativeClass_invalid10" )
	CSL_EXPECT_SYNTAX_ERROR( "TestNativeClass_invalid10.csl", 7 )
	CSL_TEST_END()

	CSL_TEST_BEGIN( "NativeClassTests.TestNativeClass_invalid11" )
	CSL_EXPECT_SYNTAX_ERROR( "TestNativeClass_invalid11.csl", 1 )
	CSL_TEST_END()

	CSL_TEST_BEGIN( "NativeClassTests.TestNativeClass_invalid12" )
	CSL_EXPECT_SYNTAX_ERROR( "TestNativeClass_invalid12.csl", 1 )
	CSL_TEST_END()

	CSL_TEST_BEGIN( "NativeClassTests.TestNativeClass_invalid13" )
	CSL_EXPECT_SYNTAX_ERROR( "TestNativeClass_invalid13.csl", 7 )
	CSL_TEST_END()
}
