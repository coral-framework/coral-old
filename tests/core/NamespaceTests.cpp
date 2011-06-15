/*
 * Coral - Lightweight C++ Component Framework
 * See Copyright Notice in Coral.h
 */

#include "TestHelper.h"

#include <co/Coral.h>
#include <co/RefPtr.h>
#include <co/ISystem.h>
#include <co/INamespace.h>
#include <co/ITypeBuilder.h>
#include <co/ITypeManager.h>
#include <co/IllegalNameException.h>
#include <co/MissingInputException.h>
#include <co/IllegalArgumentException.h>
#include <co/NotSupportedException.h>

#include <gtest/gtest.h>

TEST( NamespaceTests, rootNS )
{
	co::INamespace* rootNS = co::getSystem()->getTypes()->getRootNS();

	EXPECT_EQ( rootNS->getName(), "" );
	EXPECT_EQ( rootNS->getFullName(), "" );
	EXPECT_TRUE( rootNS->getParentNamespace() == 0 );
}

TEST( NamespaceTests, defineType )
{
	co::INamespace* rootNS = co::getSystem()->getTypes()->getRootNS();

	// make sure these types are not user-definable

	co::RefPtr<co::ITypeTransaction> transaction = createTypeTransaction();
	EXPECT_THROW( rootNS->defineType( "invalidTK", co::TK_NONE, transaction.get() ), co::IllegalArgumentException );
	EXPECT_THROW( rootNS->defineType( "invalidTK", co::TK_ANY, transaction.get() ), co::IllegalArgumentException );
	EXPECT_THROW( rootNS->defineType( "invalidTK", co::TK_BOOLEAN, transaction.get() ), co::IllegalArgumentException );
	EXPECT_THROW( rootNS->defineType( "invalidTK", co::TK_INT8, transaction.get() ), co::IllegalArgumentException );
	EXPECT_THROW( rootNS->defineType( "invalidTK", co::TK_UINT8, transaction.get() ), co::IllegalArgumentException );
	EXPECT_THROW( rootNS->defineType( "invalidTK", co::TK_INT16, transaction.get() ), co::IllegalArgumentException );
	EXPECT_THROW( rootNS->defineType( "invalidTK", co::TK_UINT16, transaction.get() ), co::IllegalArgumentException );
	EXPECT_THROW( rootNS->defineType( "invalidTK", co::TK_INT32, transaction.get() ), co::IllegalArgumentException );
	EXPECT_THROW( rootNS->defineType( "invalidTK", co::TK_UINT32, transaction.get() ), co::IllegalArgumentException );
	EXPECT_THROW( rootNS->defineType( "invalidTK", co::TK_INT64, transaction.get() ), co::IllegalArgumentException );
	EXPECT_THROW( rootNS->defineType( "invalidTK", co::TK_UINT64, transaction.get() ), co::IllegalArgumentException );
	EXPECT_THROW( rootNS->defineType( "invalidTK", co::TK_FLOAT, transaction.get() ), co::IllegalArgumentException );
	EXPECT_THROW( rootNS->defineType( "invalidTK", co::TK_DOUBLE, transaction.get() ), co::IllegalArgumentException );
	EXPECT_THROW( rootNS->defineType( "invalidTK", co::TK_STRING, transaction.get() ), co::IllegalArgumentException );
	EXPECT_THROW( rootNS->defineType( "invalidTK", co::TK_ARRAY, transaction.get() ), co::IllegalArgumentException );

	// test collision with namespaces
	rootNS->defineChildNamespace( "DummyNamespace" );
	EXPECT_THROW( rootNS->defineType( "DummyNamespace", co::TK_ENUM, transaction.get() ), co::IllegalNameException );

	// test collision with another type
	co::RefPtr<co::ITypeBuilder> tb = rootNS->defineType( "DummyException", co::TK_EXCEPTION, transaction.get() );
	ASSERT_TRUE( TestHelper::type( "DummyException" ) != NULL );

	ASSERT_TRUE( rootNS->getType( "nonexistent" ) == NULL );
	ASSERT_TRUE( rootNS->getType( "DummyException" ) != NULL );
	EXPECT_THROW( rootNS->defineType( "DummyException", co::TK_ENUM, transaction.get() ), co::IllegalNameException );

	// test definability of remaining types
	EXPECT_NO_THROW( tb = rootNS->defineType( "missingInput1", co::TK_ENUM, transaction.get() ) );
	EXPECT_THROW( transaction->commit(), co::MissingInputException );

	EXPECT_NO_THROW( tb = rootNS->defineType( "missingInput2", co::TK_STRUCT, transaction.get() ) );
	EXPECT_THROW( transaction->commit(), co::NotSupportedException );

	EXPECT_NO_THROW( tb = rootNS->defineType( "missingInput3", co::TK_NATIVECLASS, transaction.get() ) );
	EXPECT_NO_THROW( tb = rootNS->defineType( "missingInput4", co::TK_INTERFACE, transaction.get() ) );
	EXPECT_NO_THROW( tb = rootNS->defineType( "missingInput5", co::TK_COMPONENT, transaction.get() ) );

	EXPECT_NO_THROW( transaction->rollback() );
}

TEST( NamespaceTests, defineChildNamespace )
{
	co::INamespace* rootNS = co::getSystem()->getTypes()->getRootNS();

	co::INamespace* levelOneNS = rootNS->defineChildNamespace( "levelOne" );

	EXPECT_EQ( levelOneNS->getName(), "levelOne" );
	EXPECT_EQ( levelOneNS->getFullName(), "levelOne" );
	EXPECT_EQ( levelOneNS->getParentNamespace(), rootNS );
	EXPECT_TRUE( levelOneNS->getTypes().isEmpty() );
	EXPECT_TRUE( levelOneNS->getChildNamespaces().isEmpty() );

	co::INamespace* levelTwoNS = levelOneNS->defineChildNamespace( "levelTwo" );
	
	EXPECT_EQ( levelTwoNS->getName(), "levelTwo" );
	EXPECT_EQ( levelTwoNS->getFullName(), "levelOne.levelTwo" );
	EXPECT_EQ( levelTwoNS->getParentNamespace(), levelOneNS );
	EXPECT_TRUE( levelTwoNS->getTypes().isEmpty() );
	EXPECT_TRUE( levelTwoNS->getChildNamespaces().isEmpty() );

	EXPECT_EQ( levelOneNS->getTypes().getSize(), 0 );
	EXPECT_EQ( levelOneNS->getChildNamespaces().getSize(), 1 );
	EXPECT_EQ( levelOneNS->getType( "levelTwo" ), reinterpret_cast<co::IType*>( NULL ) );
	EXPECT_EQ( levelOneNS->getChildNamespace( "levelTwo" ), levelTwoNS );

	EXPECT_EQ( co::getSystem()->getTypes()->findNamespace( "levelOne.levelTwo" ), levelTwoNS );

	EXPECT_NO_THROW( rootNS->getChildNamespace( "lalala" ) );

	EXPECT_THROW( rootNS->defineChildNamespace( "levelOne" ), co::IllegalNameException );
}
