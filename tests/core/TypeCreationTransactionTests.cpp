/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#include "TestHelper.h"

#include <core/Namespace.h>

#include <co/RefPtr.h>
#include <co/System.h>
#include <co/StructType.h>
#include <co/TypeBuilder.h>
#include <co/TypeManager.h>
#include <co/MissingInputException.h>
#include <co/UnsupportedOperationException.h>

#include <gtest/gtest.h>

TEST( TypeCreationTransactionTests, throwOnCommitAfterSuccessfullCommit )
{
	co::RefPtr<co::TypeCreationTransaction> transaction = createTypeCreationTransaction();
	co::RefPtr<co::TypeBuilder> builder = TestHelper::createBuilder( co::TK_ENUM, "TypeCreationTransactionTests.throwOnCommitAfterSuccessfullCommit.EnumType", transaction.get() );

	builder->defineIdentifier( "foo" );

	EXPECT_NO_THROW( transaction->commit() );

	// cannot commit nor rollback
	EXPECT_THROW( transaction->commit(), co::UnsupportedOperationException );
	EXPECT_THROW( transaction->rollback(), co::UnsupportedOperationException );
}

TEST( TypeCreationTransactionTests, throwOnCommitAfterUnsuccessfullCommit )
{
	co::RefPtr<co::TypeCreationTransaction> transaction = createTypeCreationTransaction();
	co::RefPtr<co::TypeBuilder> builder = TestHelper::createBuilder( co::TK_ENUM, "TypeCreationTransactionTests.throwOnCommitAfterUnsuccessfullCommit.EnumType", transaction.get() );

	// invalid type creation since the enum has no identifiers
	EXPECT_THROW( transaction->commit(), co::MissingInputException );

	// try to fix it
	builder->defineIdentifier( "foo" );

	// cannot commit again even if the las commit was unsuccessful and the builder was corrected
	EXPECT_THROW( transaction->commit(), co::UnsupportedOperationException );
	EXPECT_NO_THROW( transaction->rollback() );
}

TEST( TypeCreationTransactionTests, rollbackWithoutCommit )
{
	co::RefPtr<co::TypeCreationTransaction> transaction = createTypeCreationTransaction();
	co::RefPtr<co::TypeBuilder> enumBuilder = TestHelper::createBuilder( co::TK_ENUM, "TypeCreationTransactionTests.transactionRollbackTest.EnumType", transaction.get() );
	co::RefPtr<co::TypeBuilder> structBuilder = TestHelper::createBuilder( co::TK_STRUCT, "TypeCreationTransactionTests.transactionRollbackTest.StructType", transaction.get() );

	ASSERT_NO_THROW( enumBuilder->defineIdentifier( "foo" ) );
	ASSERT_NO_THROW( structBuilder->defineAttribute( "name", enumBuilder->createType(), false ) );

	// force the creation of an array for the struct
	ASSERT_NO_THROW( co::getSystem()->getTypes()->getArrayOf( structBuilder->createType() ) );
	
	ASSERT_TRUE( TestHelper::type( "TypeCreationTransactionTests.transactionRollbackTest.EnumType" ) != NULL );
	ASSERT_TRUE( TestHelper::type( "TypeCreationTransactionTests.transactionRollbackTest.StructType" ) != NULL );
	ASSERT_TRUE( TestHelper::type( "TypeCreationTransactionTests.transactionRollbackTest.StructType[]" ) != NULL );

	EXPECT_NO_THROW( transaction->rollback() );

	EXPECT_TRUE( TestHelper::type( "TypeCreationTransactionTests.transactionRollbackTest.EnumType" ) == NULL );
	EXPECT_TRUE( TestHelper::type( "TypeCreationTransactionTests.transactionRollbackTest.StructType" ) == NULL );
	EXPECT_TRUE( TestHelper::type( "TypeCreationTransactionTests.transactionRollbackTest.StructType[]" ) == NULL );
}
