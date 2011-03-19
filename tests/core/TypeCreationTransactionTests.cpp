/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#include "TestHelper.h"

#include <co/RefPtr.h>
#include <co/ISystem.h>
#include <co/INamespace.h>
#include <co/IStructType.h>
#include <co/ITypeBuilder.h>
#include <co/ITypeManager.h>
#include <co/MissingInputException.h>
#include <co/NotSupportedException.h>

#include <gtest/gtest.h>

TEST( TypeCreationTransactionTests, throwOnCommitAfterSuccessfullCommit )
{
	co::RefPtr<co::ITypeCreationTransaction> transaction = createTypeCreationTransaction();
	co::RefPtr<co::ITypeBuilder> builder = TestHelper::createBuilder( co::TK_ENUM, "TypeCreationTransactionTests.throwOnCommitAfterSuccessfullCommit.IEnumType", transaction.get() );

	builder->defineIdentifier( "foo" );

	EXPECT_NO_THROW( transaction->commit() );

	// cannot commit nor rollback
	EXPECT_THROW( transaction->commit(), co::NotSupportedException );
	EXPECT_THROW( transaction->rollback(), co::NotSupportedException );
}

TEST( TypeCreationTransactionTests, throwOnCommitAfterUnsuccessfullCommit )
{
	co::RefPtr<co::ITypeCreationTransaction> transaction = createTypeCreationTransaction();
	co::RefPtr<co::ITypeBuilder> builder = TestHelper::createBuilder( co::TK_ENUM, "TypeCreationTransactionTests.throwOnCommitAfterUnsuccessfullCommit.IEnumType", transaction.get() );

	// invalid type creation since the enum has no identifiers
	EXPECT_THROW( transaction->commit(), co::MissingInputException );

	// try to fix it
	builder->defineIdentifier( "foo" );

	// cannot commit again even if the las commit was unsuccessful and the builder was corrected
	EXPECT_THROW( transaction->commit(), co::NotSupportedException );
	EXPECT_NO_THROW( transaction->rollback() );
}

TEST( TypeCreationTransactionTests, rollbackWithoutCommit )
{
	co::RefPtr<co::ITypeCreationTransaction> transaction = createTypeCreationTransaction();
	co::RefPtr<co::ITypeBuilder> enumBuilder = TestHelper::createBuilder( co::TK_ENUM, "TypeCreationTransactionTests.transactionRollbackTest.IEnumType", transaction.get() );
	co::RefPtr<co::ITypeBuilder> structBuilder = TestHelper::createBuilder( co::TK_STRUCT, "TypeCreationTransactionTests.transactionRollbackTest.IStructType", transaction.get() );

	ASSERT_NO_THROW( enumBuilder->defineIdentifier( "foo" ) );
	ASSERT_NO_THROW( structBuilder->defineAttribute( "name", enumBuilder->createType(), false ) );

	// force the creation of an array for the struct
	ASSERT_NO_THROW( co::getSystem()->getTypes()->getArrayOf( structBuilder->createType() ) );
	
	ASSERT_TRUE( TestHelper::type( "TypeCreationTransactionTests.transactionRollbackTest.IEnumType" ) != NULL );
	ASSERT_TRUE( TestHelper::type( "TypeCreationTransactionTests.transactionRollbackTest.IStructType" ) != NULL );
	ASSERT_TRUE( TestHelper::type( "TypeCreationTransactionTests.transactionRollbackTest.IStructType[]" ) != NULL );

	EXPECT_NO_THROW( transaction->rollback() );

	EXPECT_TRUE( TestHelper::type( "TypeCreationTransactionTests.transactionRollbackTest.IEnumType" ) == NULL );
	EXPECT_TRUE( TestHelper::type( "TypeCreationTransactionTests.transactionRollbackTest.IStructType" ) == NULL );
	EXPECT_TRUE( TestHelper::type( "TypeCreationTransactionTests.transactionRollbackTest.IStructType[]" ) == NULL );
}
