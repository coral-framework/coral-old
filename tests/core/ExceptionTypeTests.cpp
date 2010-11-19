/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#include "CSLTester.h"
#include <gtest/gtest.h>

TEST( ExceptionTypeTests, simpleValidException )
{
	CSL_TEST( "ExceptionTypeTests.exception_valid" )
}

TEST( ExceptionTypeTests, invalidExceptions )
{
	CSL_TEST_BEGIN( "ExceptionTypeTests.exception_invalid1" )
	CSL_EXPECT_SYNTAX_ERROR( "exception_invalid1.csl", 2 )
	CSL_TEST_END()

	CSL_TEST_BEGIN( "ExceptionTypeTests.exception_invalid2" )
	CSL_EXPECT_SYNTAX_ERROR( "exception_invalid2.csl", 1 )
	CSL_TEST_END()

	CSL_TEST_BEGIN( "ExceptionTypeTests.exception_invalid3" )
	CSL_EXPECT_SYNTAX_ERROR( "exception_invalid3.csl", 3 )
	CSL_TEST_END()
}

TEST( ExceptionTypeTests, exceptionInheritance )
{
	CSL_TEST_BEGIN( "ExceptionTypeTests.exception_inheritance" )
	CSL_EXPECT_SYNTAX_ERROR( "exception_inheritance.csl", 1 )
	CSL_TEST_END()
}
