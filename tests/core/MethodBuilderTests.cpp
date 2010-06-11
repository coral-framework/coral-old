/*
 * Coral - A C++ Component Framework.
 * See Copyright Notice in Coral.h
 */

#include "TestHelper.h"

#include <core/MethodInfo.h>
#include <core/MethodBuilder.h>

#include <co/Coral.h>
#include <co/RefPtr.h>
#include <co/System.h>
#include <co/Namespace.h>
#include <co/TypeBuilder.h>
#include <co/InterfaceType.h>
#include <co/IllegalNameException.h>
#include <co/MissingInputException.h>
#include <co/IllegalArgumentException.h>
#include <co/UnsupportedOperationException.h>

#include <gtest/gtest.h>

TEST( MethodBuilderTests, theTest )
{
	co::RefPtr<co::TypeCreationTransaction> tct = createTypeCreationTransaction();
	co::RefPtr<co::TypeBuilder> typeBuilder = TestHelper::createBuilder( co::TK_INTERFACE, "MethodBuilderbuilderTest.NewInterface", tct.get() );

	co::Type* stringType = 	TestHelper::type( "string" );

	co::RefPtr<co::TypeBuilder> exbuilder = TestHelper::createBuilder( co::TK_EXCEPTION, "MethodBuilderbuilderTest.NewException", tct.get() );
	co::ExceptionType* testException = dynamic_cast<co::ExceptionType*>( exbuilder->createType() );

	co::RefPtr<co::MethodBuilder> mb = typeBuilder->defineMethod( "testMethod" );

	EXPECT_THROW( mb->defineException( NULL ), co::IllegalArgumentException );
	EXPECT_THROW( mb->defineReturnType( NULL ), co::IllegalArgumentException );
	EXPECT_THROW( mb->defineParameter( "ok", NULL, true, true ) , co::IllegalArgumentException );
	EXPECT_THROW( mb->defineParameter( "ok", stringType, false, false ) , co::IllegalArgumentException );

	mb->defineParameter( "p1", stringType, false, true );
	mb->defineParameter( "p2", stringType, true, true );
	mb->defineParameter( "p3", stringType, true, false );

	mb->defineException( testException );

	mb->defineReturnType( stringType );

	EXPECT_THROW( mb->defineParameter( "p1", stringType, true, false ) , co::IllegalNameException ); // clash

	EXPECT_NO_THROW( mb->createMethod() );

	co::InterfaceType* interface = dynamic_cast<co::InterfaceType*>( typeBuilder->createType() );
	ASSERT_TRUE( interface != NULL );

	co::MethodInfo* mInfo = dynamic_cast<co::MethodInfo*>( interface->getMember( "testMethod" ) );
	ASSERT_TRUE( mInfo != NULL );

	ASSERT_EQ( 3, mInfo->getParameters().getSize() );

	co::ArrayRange<co::ParameterInfo* const> params = mInfo->getParameters();
	co::ParameterInfo* p = params.getFirst();
	ASSERT_EQ( "p1", p->getName() );
	ASSERT_EQ( stringType, p->getType() );
	ASSERT_EQ( false, p->getIsIn() );
	ASSERT_EQ( true, p->getIsOut() );

	params.popFirst();
	p = params.getFirst();
	ASSERT_EQ( "p2", p->getName() );
	ASSERT_EQ( stringType, p->getType() );
	ASSERT_EQ( true, p->getIsIn() );
	ASSERT_EQ( true, p->getIsOut() );

	params.popFirst();
	p = params.getFirst();
	ASSERT_EQ( "p3", p->getName() );
	ASSERT_EQ( stringType, p->getType() );
	ASSERT_EQ( true, p->getIsIn() );
	ASSERT_EQ( false, p->getIsOut() );

	ASSERT_EQ( stringType, mInfo->getReturnType() );

	ASSERT_EQ( testException , mInfo->getExceptions().getFirst() );

	EXPECT_NO_THROW( tct->commit() );

	EXPECT_THROW( mb->createMethod(), co::UnsupportedOperationException );
}
