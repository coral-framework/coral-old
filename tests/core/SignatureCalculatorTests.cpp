/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#include "TestHelper.h"
#include <co/Uuid.h>
#include <co/Type.h>
#include <co/Coral.h>
#include <co/RefPtr.h>
#include <co/System.h>
#include <co/Namespace.h>
#include <co/TypeBuilder.h>
#include <co/TypeManager.h>
#include <co/ExceptionType.h>
#include <co/InterfaceType.h>
#include <co/MethodBuilder.h>
#include <co/IllegalNameException.h>
#include <co/IllegalArgumentException.h>
#include <co/MissingInputException.h>
#include <gtest/gtest.h>

static co::InterfaceType* createDummyInterface( const std::string& name, co::TypeCreationTransaction* tct )
{
	co::RefPtr<co::TypeBuilder> builder = TestHelper::createBuilder( co::TK_INTERFACE, name, tct );
	builder->defineAttribute( "dummyAtr", TestHelper::type( "string" ), false );

	return dynamic_cast<co::InterfaceType*>( builder->createType() );
}

TEST( SignatureCalculatorTests, binarySignatureIndependsOnAttributeDefineOrder )
{
	co::Type* stringType = TestHelper::type( "string" );

	co::RefPtr<co::TypeCreationTransaction> tct = createTypeCreationTransaction();
	co::RefPtr<co::TypeBuilder> builder = TestHelper::createBuilder( co::TK_INTERFACE, "createBinarySignatureTests.First", tct.get() );

	builder->defineAttribute( "a", stringType, false );
	builder->defineAttribute( "b", stringType, true );
	builder->defineAttribute( "c", stringType, true );
	builder->defineAttribute( "d", stringType, false );

	co::Type* first = builder->createType();

	co::RefPtr<co::TypeBuilder> builder2 = TestHelper::createBuilder( co::TK_INTERFACE, "createBinarySignatureTests.Second", tct.get() );

	builder2->defineAttribute( "c", stringType, true );
	builder2->defineAttribute( "b", stringType, true );
	builder2->defineAttribute( "d", stringType, false );
	builder2->defineAttribute( "a", stringType, false );

	co::Type* second = builder2->createType();

	ASSERT_NO_THROW( tct->commit() );

	EXPECT_FALSE( first->getBinarySignature().isNull() );
	EXPECT_TRUE( second->getBinarySignature() == second->getBinarySignature() );
}

TEST( SignatureCalculatorTests, arraySignatures )
{
	co::RefPtr<co::TypeCreationTransaction> transaction = createTypeCreationTransaction();

	co::Type* stringType = TestHelper::type( "string" );
	co::Type* int32Type = TestHelper::type( "int32" );
	co::Type* interfaceTest = createDummyInterface( "ArraySignaturesTest.Interface", transaction.get() );
	co::Type* interfaceCopy = createDummyInterface( "ArraySignaturesTest.InterfaceCopy", transaction.get() );

	co::TypeManager* tm = co::getSystem()->getTypes();
	tm->getArrayOf( stringType );
	tm->getArrayOf( int32Type );
	tm->getArrayOf( interfaceTest );
	tm->getArrayOf( interfaceCopy );

	ASSERT_NO_THROW( transaction->commit() );

	co::Type* first = tm->findType( "string[]" );
	co::Type* second = tm->findType( "int32[]" );
	co::Type* third = tm->findType( "ArraySignaturesTest.Interface[]" );
	co::Type* fourth = tm->findType( "ArraySignaturesTest.InterfaceCopy[]" );

	EXPECT_FALSE( first->getBinarySignature().isNull() );
	EXPECT_FALSE( second->getBinarySignature().isNull() );
	EXPECT_FALSE( third->getBinarySignature().isNull() );
	EXPECT_FALSE( fourth->getBinarySignature().isNull() );

	EXPECT_TRUE( first->getBinarySignature() != second->getBinarySignature() );
	EXPECT_TRUE( first->getBinarySignature() != third->getBinarySignature() );
	EXPECT_TRUE( second->getBinarySignature() != third->getBinarySignature() );

	EXPECT_TRUE( interfaceTest->getBinarySignature() != interfaceCopy->getBinarySignature() );
	EXPECT_TRUE( interfaceTest->getFullSignature() != interfaceCopy->getFullSignature() );

	EXPECT_TRUE( third->getBinarySignature() != fourth->getBinarySignature() );
	EXPECT_TRUE( third->getFullSignature() != fourth->getFullSignature() );
}

TEST( SignatureCalculatorTests, enumSignature )
{
	co::RefPtr<co::TypeCreationTransaction> tct = createTypeCreationTransaction();
	co::RefPtr<co::TypeBuilder> builder = TestHelper::createBuilder( co::TK_ENUM, "EnumSignatureTest.First", tct.get() );

	builder->defineIdentifier( "Test_1" );
	builder->defineIdentifier( "Test_2" );
	builder->defineIdentifier( "Test_3" );

	co::Type* first = builder->createType();

	co::RefPtr<co::TypeBuilder> builder2 = TestHelper::createBuilder( co::TK_ENUM, "EnumSignatureTest.FirstCopy", tct.get() );
	builder2->defineIdentifier( "Test_1" );
	builder2->defineIdentifier( "Test_2" );
	builder2->defineIdentifier( "Test_3" );

	co::Type* firstCopy = builder2->createType();

	co::RefPtr<co::TypeBuilder> builder3 = TestHelper::createBuilder( co::TK_ENUM, "EnumSignatureTest.FirstSwitch", tct.get() );
	builder3->defineIdentifier( "Test_1" );
	builder3->defineIdentifier( "Test_3" );
	builder3->defineIdentifier( "Test_2" );
	builder3->createType();

	co::Type* firstSwitchIdentifiers = builder3->createType();

	co::RefPtr<co::TypeBuilder> builder4 = TestHelper::createBuilder( co::TK_ENUM, "EnumSignatureTest.FirstShift", tct.get() );
	builder4->defineIdentifier( "Test_1" );
	builder4->defineIdentifier( "Tes" );
	builder4->defineIdentifier( "t_2Test_3" );
	builder4->createType();

	co::Type* firstIdentifersShift = builder4->createType();

	tct->commit();

	EXPECT_TRUE( first->getBinarySignature() != firstIdentifersShift->getBinarySignature() );
	EXPECT_TRUE( first->getFullSignature() != firstIdentifersShift->getFullSignature() );
	EXPECT_TRUE( first->getFullSignature() != first->getBinarySignature() );
	EXPECT_TRUE( first->getBinarySignature() != firstSwitchIdentifiers->getBinarySignature() );
	EXPECT_TRUE( first->getFullSignature() != firstSwitchIdentifiers->getFullSignature() );
	EXPECT_TRUE( first->getBinarySignature() == firstCopy->getBinarySignature() );
	EXPECT_TRUE( first->getFullSignature() != firstCopy->getFullSignature() );
}

TEST( SignatureCalculatorTests, methodSignature )
{
	co::RefPtr<co::TypeCreationTransaction> tct = createTypeCreationTransaction();

	co::RefPtr<co::TypeBuilder> builder1 = TestHelper::createBuilder( co::TK_INTERFACE, "MethodSignatureTest.First", tct.get() );
	co::RefPtr<co::MethodBuilder> methodBuilder1 = builder1->defineMethod( "Ask" );
	methodBuilder1->defineParameter( "question", TestHelper::type( "string" ), true, false );
	methodBuilder1->createMethod();
	co::InterfaceType* first = dynamic_cast<co::InterfaceType*>( builder1->createType() );

	// variations with diferent signature
	co::RefPtr<co::TypeBuilder> var1builder = TestHelper::createBuilder( co::TK_INTERFACE, "MethodSignatureTest.Variation1", tct.get() );
	co::RefPtr<co::MethodBuilder> var1methodBuilder = var1builder->defineMethod( "Ask" );
	var1methodBuilder->defineParameter( "question", TestHelper::type( "string" ), true, true );
	var1methodBuilder->createMethod();
	co::InterfaceType* variation1 = dynamic_cast<co::InterfaceType*>( var1builder->createType() );

	EXPECT_TRUE( first->getBinarySignature() != variation1->getBinarySignature() );
	EXPECT_TRUE( first->getFullSignature() != variation1->getFullSignature() );

	co::RefPtr<co::TypeBuilder> var2builder = TestHelper::createBuilder( co::TK_INTERFACE, "MethodSignatureTest.Variation2", tct.get() );
	co::RefPtr<co::MethodBuilder> var2methodBuilder = var2builder->defineMethod( "Ask" );
	var2methodBuilder->defineParameter( "question", TestHelper::type( "string" ), false, true );
	var2methodBuilder->createMethod();
	co::InterfaceType* variation2 = dynamic_cast<co::InterfaceType*>( var2builder->createType() );

	EXPECT_TRUE( first->getBinarySignature() != variation2->getBinarySignature() );
	EXPECT_TRUE( first->getFullSignature() != variation2->getFullSignature() );

	co::RefPtr<co::TypeBuilder> var3builder = TestHelper::createBuilder( co::TK_INTERFACE, "MethodSignatureTest.Variation3", tct.get() );
	co::RefPtr<co::MethodBuilder> var3methodBuilder = var3builder->defineMethod( "Ask" );
	var3methodBuilder->defineParameter( "question", TestHelper::type( "int32" ), true, false );
	var3methodBuilder->createMethod();
	co::InterfaceType* variation3 = dynamic_cast<co::InterfaceType*>( var3builder->createType() );

	EXPECT_TRUE( first->getBinarySignature() != variation3->getBinarySignature() );
	EXPECT_TRUE( first->getFullSignature() != variation3->getFullSignature() );

	co::RefPtr<co::TypeBuilder> var4builder = TestHelper::createBuilder( co::TK_INTERFACE, "MethodSignatureTest.Variation4", tct.get() );
	co::RefPtr<co::MethodBuilder> var4methodBuilder = var4builder->defineMethod( "Ask" );
	var4methodBuilder->defineParameter( "question", TestHelper::type( "string" ), true, false );
	var4methodBuilder->defineReturnType( TestHelper::type("string") );
	var4methodBuilder->createMethod();
	co::InterfaceType* variation4 = dynamic_cast<co::InterfaceType*>( var4builder->createType() );

	EXPECT_TRUE( first->getBinarySignature() != variation4->getBinarySignature() );
	EXPECT_TRUE( first->getFullSignature() != variation4->getFullSignature() );
	
	co::RefPtr<co::TypeBuilder> var5builder = TestHelper::createBuilder( co::TK_INTERFACE, "MethodSignatureTest.Variation5", tct.get() );
	co::RefPtr<co::MethodBuilder> var5methodBuilder = var5builder->defineMethod( "Ask" );
	var5methodBuilder->defineParameter( "question", TestHelper::type( "string" ), true, false );
	var5methodBuilder->defineReturnType( TestHelper::type("int32") );
	var5methodBuilder->createMethod();
	co::InterfaceType* variation5 = dynamic_cast<co::InterfaceType*>( var5builder->createType() );

	EXPECT_TRUE( first->getBinarySignature() != variation5->getBinarySignature() );
	EXPECT_TRUE( first->getFullSignature() != variation5->getFullSignature() );
	EXPECT_TRUE( variation4->getBinarySignature() != variation5->getBinarySignature() );
	
	EXPECT_NO_THROW( tct->commit() );
}

TEST( SignatureCalculatorTests, attributeSignature )
{
	co::RefPtr<co::TypeCreationTransaction> tct = createTypeCreationTransaction();

	co::RefPtr<co::TypeBuilder> builder1 = TestHelper::createBuilder( co::TK_INTERFACE, "AttributeSignatureTest.First", tct.get() );
	builder1->defineAttribute( "name", TestHelper::type( "string" ), true );
	co::InterfaceType* first = dynamic_cast<co::InterfaceType*>( builder1->createType() );

	//variations with the same binary signature
	co::RefPtr<co::TypeBuilder> builder2 = TestHelper::createBuilder( co::TK_INTERFACE, "AttributeSignatureTest.SameBinary1", tct.get() );
	builder2->defineAttribute( "name", TestHelper::type( "string" ), true );
	co::InterfaceType* sameBinary1 = dynamic_cast<co::InterfaceType*>( builder2->createType() );

	co::RefPtr<co::TypeBuilder> builder3 = TestHelper::createBuilder( co::TK_INTERFACE, "AttributeSignatureTest.SameBinary2", tct.get() );
	builder3->defineAttribute( "fullName", TestHelper::type( "string" ), true );
	co::InterfaceType* sameBinary2 = dynamic_cast<co::InterfaceType*>( builder3->createType() );

	// variations with diferent signature
	co::RefPtr<co::TypeBuilder> builder4 = TestHelper::createBuilder( co::TK_INTERFACE, "AttributeSignatureTest.Variation1", tct.get() );
	builder4->defineAttribute( "fullName", TestHelper::type( "string" ), false );
	co::InterfaceType* variation1 = dynamic_cast<co::InterfaceType*>( builder4->createType() );

	co::RefPtr<co::TypeBuilder> builder5 = TestHelper::createBuilder( co::TK_INTERFACE, "AttributeSignatureTest.Variation2", tct.get() );
	builder5->defineAttribute( "name", TestHelper::type( "int32" ), false );
	co::InterfaceType* variation2 = dynamic_cast<co::InterfaceType*>( builder5->createType() );

	ASSERT_NO_THROW( tct->commit() );

	EXPECT_TRUE( first->getBinarySignature() != sameBinary1->getBinarySignature() );
	EXPECT_TRUE( first->getFullSignature() != sameBinary1->getFullSignature() );
	EXPECT_TRUE( first->getBinarySignature() != sameBinary2->getBinarySignature() );
	EXPECT_TRUE( first->getFullSignature() != sameBinary2->getFullSignature() );

	EXPECT_TRUE( first->getFullSignature() != variation1->getFullSignature() );
	EXPECT_TRUE( first->getBinarySignature() != variation1->getBinarySignature() );
	EXPECT_TRUE( first->getBinarySignature() != variation2->getBinarySignature() );
	EXPECT_TRUE( first->getFullSignature() != variation2->getFullSignature() );
}
