/*
 * Coral - A C++ Component Framework.
 * See Copyright Notice in Coral.h
 */

#include "CSLTester.h"
#include <gtest/gtest.h>

TEST( ArrayTypeTests, invalidArrayTypes )
{
	CSL_TEST_BEGIN( "ArrayTypeTests.invalidExceptionArray" )
	CSL_EXPECT_ERROR( "arrays of exceptions are illegal", "invalidExceptionArray.csl", 6 )
	CSL_TEST_END()

	CSL_TEST_BEGIN( "ArrayTypeTests.invalidComponentArray" )
	CSL_EXPECT_ERROR( "arrays of components are illegal", "invalidComponentArray.csl", 6 )
	CSL_TEST_END()
}
