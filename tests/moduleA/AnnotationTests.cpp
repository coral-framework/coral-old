/*
 * Coral - Lightweight C++ Component Framework
 * See Copyright Notice in Coral.h
 */

#include <co/Coral.h>
#include <co/Exception.h>
#include <moduleA/ITestAnnotation.h>
#include <gtest/gtest.h>

TEST( AnnotationTests, tonsOfDataTests )
{
	co::IType* t = co::getType( "moduleA.tests.AnnotationData" );

	moduleA::ITestAnnotation* ta = t->getAnnotation<moduleA::ITestAnnotation>();
	ASSERT_TRUE( ta != NULL );

	EXPECT_EQ( 5.5, ta->getValue() );
	EXPECT_TRUE( ta->getB() );
	EXPECT_EQ( 0, ta->getI8() );
	EXPECT_EQ( static_cast<co::uint8>( 300 ), ta->getU8() );
	EXPECT_EQ( -0.00075f, ta->getFlt() );
	EXPECT_EQ( std::string( "Hello\nWorld\0This is awesome! More.", 34 ), ta->getStr() );
	EXPECT_EQ( 9, ta->getI32() );
}

TEST( AnnotationTests, deepNestedErrors )
{
	try
	{
		co::getType( "moduleA.tests.DeepErrors" );
		FAIL() << "exception now thrown";
	}
	catch( co::Exception& e )
	{
		EXPECT_EQ( "could not load type 'moduleA.tests.DeepErrors':\n"
			"From /Users/tbastos/Projects/coral/tests/moduleA/moduleA/tests/DeepErrors.csl:1:18:"
			" error setting annotation field 'value':\n"
			"\tIn file /Users/tbastos/Projects/coral/tests/moduleA/moduleA/tests/DeepErrors.csl:1:18:"
			" could not load type 'moduleA.tests.FaultyType':\n"
			"In file /Users/tbastos/Projects/coral/tests/moduleA/moduleA/tests/FaultyType.csl:1:18:"
			" annotation type 'co.ICppBlock' has no field named 'raise'", e.getMessage() );
	}
}
