/*
 * Coral - Lightweight C++ Component Framework
 * See Copyright Notice in Coral.h
 */

#include "TestHelper.h"

#include <co/RefPtr.h>
#include <co/ISystem.h>
#include <co/INamespace.h>
#include <co/IStruct.h>
#include <co/ITypeBuilder.h>
#include <co/ITypeManager.h>
#include <co/MissingInputException.h>
#include <co/NotSupportedException.h>

#include <gtest/gtest.h>

TEST( TypeTransactionTests, throwOnCommitAfterSuccessfullCommit )
{
	co::RefPtr<co::ITypeTransaction> transaction = createTypeTransaction();
	co::RefPtr<co::ITypeBuilder> builder = TestHelper::createBuilder( co::TK_ENUM, "TypeTransactionTests.throwOnCommitAfterSuccessfullCommit.IEnum", transaction.get() );

	builder->defineIdentifier( "foo" );

	EXPECT_NO_THROW( transaction->commit() );

	// cannot commit nor rollback
	EXPECT_THROW( transaction->commit(), co::NotSupportedException );
	EXPECT_THROW( transaction->rollback(), co::NotSupportedException );
}

TEST( TypeTransactionTests, throwOnCommitAfterUnsuccessfullCommit )
{
	co::RefPtr<co::ITypeTransaction> transaction = createTypeTransaction();
	co::RefPtr<co::ITypeBuilder> builder = TestHelper::createBuilder( co::TK_ENUM, "TypeTransactionTests.throwOnCommitAfterUnsuccessfullCommit.IEnum", transaction.get() );

	// invalid type creation since the enum has no identifiers
	EXPECT_THROW( transaction->commit(), co::MissingInputException );

	// try to fix it
	builder->defineIdentifier( "foo" );

	// cannot commit again even if the las commit was unsuccessful and the builder was corrected
	EXPECT_THROW( transaction->commit(), co::NotSupportedException );
	EXPECT_NO_THROW( transaction->rollback() );
}

TEST( TypeTransactionTests, rollbackWithoutCommit )
{
	co::RefPtr<co::ITypeTransaction> transaction = createTypeTransaction();
	co::RefPtr<co::ITypeBuilder> enumBuilder = TestHelper::createBuilder( co::TK_ENUM, "TypeTransactionTests.transactionRollbackTest.IEnum", transaction.get() );
	co::RefPtr<co::ITypeBuilder> structBuilder = TestHelper::createBuilder( co::TK_STRUCT, "TypeTransactionTests.transactionRollbackTest.IStruct", transaction.get() );

	ASSERT_NO_THROW( enumBuilder->defineIdentifier( "foo" ) );
	ASSERT_NO_THROW( structBuilder->defineField( "name", enumBuilder->createType(), false ) );

	// force the creation of an array for the struct
	ASSERT_NO_THROW( co::getSystem()->getTypes()->getArrayOf( structBuilder->createType() ) );
	
	ASSERT_TRUE( TestHelper::type( "TypeTransactionTests.transactionRollbackTest.IEnum" ) != NULL );
	ASSERT_TRUE( TestHelper::type( "TypeTransactionTests.transactionRollbackTest.IStruct" ) != NULL );
	ASSERT_TRUE( TestHelper::type( "TypeTransactionTests.transactionRollbackTest.IStruct[]" ) != NULL );

	EXPECT_NO_THROW( transaction->rollback() );

	EXPECT_TRUE( TestHelper::type( "TypeTransactionTests.transactionRollbackTest.IEnum" ) == NULL );
	EXPECT_TRUE( TestHelper::type( "TypeTransactionTests.transactionRollbackTest.IStruct" ) == NULL );
	EXPECT_TRUE( TestHelper::type( "TypeTransactionTests.transactionRollbackTest.IStruct[]" ) == NULL );
}
