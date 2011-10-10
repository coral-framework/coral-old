/*
 * Coral - Lightweight C++ Component Framework
 * See Copyright Notice in Coral.h
 */

#include "CSLTester.h"
#include <co/ICppBlock.h>
#include <co/IDocumentation.h>
#include <gtest/gtest.h>

TEST( AnnotationTests, CppBlock )
{
	co::IType* t = CSL_TEST( "AnnotationTests.CppBlock" );
	ASSERT_TRUE( t != NULL );

	co::ICppBlock* cppBlock = t->getAnnotation<co::ICppBlock>();
	co::IDocumentation* doc = t->getAnnotation<co::IDocumentation>();

	ASSERT_TRUE( cppBlock != NULL );
	ASSERT_TRUE( doc == NULL );

	EXPECT_EQ( "C++ code goes here", cppBlock->getValue() );
}

TEST( AnnotationTests, Documentation )
{
	co::IType* t = CSL_TEST( "AnnotationTests.Documentation" );
	ASSERT_TRUE( t != NULL );

	co::ICppBlock* cppBlock = t->getAnnotation<co::ICppBlock>();
	co::IDocumentation* doc = t->getAnnotation<co::IDocumentation>();

	ASSERT_TRUE( cppBlock == NULL );
	ASSERT_TRUE( doc != NULL );

	EXPECT_EQ( "Here goes the documentation", doc->getValue() );
}
