/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#include "TestHelper.h"
#include <co/Uuid.h>
#include <co/IType.h>
#include <co/Coral.h>
#include <co/RefPtr.h>
#include <co/ISystem.h>
#include <co/INamespace.h>
#include <co/ITypeBuilder.h>
#include <co/ITypeManager.h>
#include <co/IExceptionType.h>
#include <co/IInterfaceType.h>
#include <co/IMethodBuilder.h>
#include <co/IllegalNameException.h>
#include <co/IllegalArgumentException.h>
#include <co/MissingInputException.h>
#include <gtest/gtest.h>

static co::IInterfaceType* createDummyInterface( const std::string& name, co::ITypeCreationTransaction* tct )
{
	co::RefPtr<co::ITypeBuilder> builder = TestHelper::createBuilder( co::TK_INTERFACE, name, tct );
	builder->defineAttribute( "dummyAtr", TestHelper::type( "string" ), false );

	return dynamic_cast<co::IInterfaceType*>( builder->createType() );
}

TEST( SignatureCalculatorTests, binarySignatureIndependsOnAttributeDefineOrder )
{
	co::IType* stringType = TestHelper::type( "string" );

	co::RefPtr<co::ITypeCreationTransaction> tct = createTypeCreationTransaction();
	co::RefPtr<co::ITypeBuilder> builder = TestHelper::createBuilder( co::TK_INTERFACE, "createBinarySignatureTests.First", tct.get() );

	builder->defineAttribute( "a", stringType, false );
	builder->defineAttribute( "b", stringType, true );
	builder->defineAttribute( "c", stringType, true );
	builder->defineAttribute( "d", stringType, false );

	co::IType* first = builder->createType();

	co::RefPtr<co::ITypeBuilder> builder2 = TestHelper::createBuilder( co::TK_INTERFACE, "createBinarySignatureTests.Second", tct.get() );

	builder2->defineAttribute( "c", stringType, true );
	builder2->defineAttribute( "b", stringType, true );
	builder2->defineAttribute( "d", stringType, false );
	builder2->defineAttribute( "a", stringType, false );

	co::IType* second = builder2->createType();

	ASSERT_NO_THROW( tct->commit() );

	EXPECT_FALSE( first->getBinarySignature().isNull() );
	EXPECT_TRUE( second->getBinarySignature() == second->getBinarySignature() );
}

TEST( SignatureCalculatorTests, arraySignatures )
{
	co::RefPtr<co::ITypeCreationTransaction> transaction = createTypeCreationTransaction();

	co::IType* stringType = TestHelper::type( "string" );
	co::IType* int32Type = TestHelper::type( "int32" );
	co::IType* interfaceTest = createDummyInterface( "ArraySignaturesTest.Interface", transaction.get() );
	co::IType* interfaceCopy = createDummyInterface( "ArraySignaturesTest.InterfaceCopy", transaction.get() );

	co::ITypeManager* tm = co::getSystem()->getTypes();
	tm->getArrayOf( stringType );
	tm->getArrayOf( int32Type );
	tm->getArrayOf( interfaceTest );
	tm->getArrayOf( interfaceCopy );

	ASSERT_NO_THROW( transaction->commit() );

	co::IType* first = tm->findType( "string[]" );
	co::IType* second = tm->findType( "int32[]" );
	co::IType* third = tm->findType( "ArraySignaturesTest.Interface[]" );
	co::IType* fourth = tm->findType( "ArraySignaturesTest.InterfaceCopy[]" );

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
	co::RefPtr<co::ITypeCreationTransaction> tct = createTypeCreationTransaction();
	co::RefPtr<co::ITypeBuilder> builder = TestHelper::createBuilder( co::TK_ENUM, "EnumSignatureTest.First", tct.get() );

	builder->defineIdentifier( "Test_1" );
	builder->defineIdentifier( "Test_2" );
	builder->defineIdentifier( "Test_3" );

	co::IType* first = builder->createType();

	co::RefPtr<co::ITypeBuilder> builder2 = TestHelper::createBuilder( co::TK_ENUM, "EnumSignatureTest.FirstCopy", tct.get() );
	builder2->defineIdentifier( "Test_1" );
	builder2->defineIdentifier( "Test_2" );
	builder2->defineIdentifier( "Test_3" );

	co::IType* firstCopy = builder2->createType();

	co::RefPtr<co::ITypeBuilder> builder3 = TestHelper::createBuilder( co::TK_ENUM, "EnumSignatureTest.FirstSwitch", tct.get() );
	builder3->defineIdentifier( "Test_1" );
	builder3->defineIdentifier( "Test_3" );
	builder3->defineIdentifier( "Test_2" );
	builder3->createType();

	co::IType* firstSwitchIdentifiers = builder3->createType();

	co::RefPtr<co::ITypeBuilder> builder4 = TestHelper::createBuilder( co::TK_ENUM, "EnumSignatureTest.FirstShift", tct.get() );
	builder4->defineIdentifier( "Test_1" );
	builder4->defineIdentifier( "Tes" );
	builder4->defineIdentifier( "t_2Test_3" );
	builder4->createType();

	co::IType* firstIdentifersShift = builder4->createType();

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
	co::RefPtr<co::ITypeCreationTransaction> tct = createTypeCreationTransaction();

	co::RefPtr<co::ITypeBuilder> builder1 = TestHelper::createBuilder( co::TK_INTERFACE, "MethodSignatureTest.First", tct.get() );
	co::RefPtr<co::IMethodBuilder> methodBuilder1 = builder1->defineMethod( "Ask" );
	methodBuilder1->defineParameter( "question", TestHelper::type( "string" ), true, false );
	methodBuilder1->createMethod();
	co::IInterfaceType* first = dynamic_cast<co::IInterfaceType*>( builder1->createType() );

	// variations with diferent signature
	co::RefPtr<co::ITypeBuilder> var1builder = TestHelper::createBuilder( co::TK_INTERFACE, "MethodSignatureTest.Variation1", tct.get() );
	co::RefPtr<co::IMethodBuilder> var1methodBuilder = var1builder->defineMethod( "Ask" );
	var1methodBuilder->defineParameter( "question", TestHelper::type( "string" ), true, true );
	var1methodBuilder->createMethod();
	co::IInterfaceType* variation1 = dynamic_cast<co::IInterfaceType*>( var1builder->createType() );

	EXPECT_TRUE( first->getBinarySignature() != variation1->getBinarySignature() );
	EXPECT_TRUE( first->getFullSignature() != variation1->getFullSignature() );

	co::RefPtr<co::ITypeBuilder> var2builder = TestHelper::createBuilder( co::TK_INTERFACE, "MethodSignatureTest.Variation2", tct.get() );
	co::RefPtr<co::IMethodBuilder> var2methodBuilder = var2builder->defineMethod( "Ask" );
	var2methodBuilder->defineParameter( "question", TestHelper::type( "string" ), false, true );
	var2methodBuilder->createMethod();
	co::IInterfaceType* variation2 = dynamic_cast<co::IInterfaceType*>( var2builder->createType() );

	EXPECT_TRUE( first->getBinarySignature() != variation2->getBinarySignature() );
	EXPECT_TRUE( first->getFullSignature() != variation2->getFullSignature() );

	co::RefPtr<co::ITypeBuilder> var3builder = TestHelper::createBuilder( co::TK_INTERFACE, "MethodSignatureTest.Variation3", tct.get() );
	co::RefPtr<co::IMethodBuilder> var3methodBuilder = var3builder->defineMethod( "Ask" );
	var3methodBuilder->defineParameter( "question", TestHelper::type( "int32" ), true, false );
	var3methodBuilder->createMethod();
	co::IInterfaceType* variation3 = dynamic_cast<co::IInterfaceType*>( var3builder->createType() );

	EXPECT_TRUE( first->getBinarySignature() != variation3->getBinarySignature() );
	EXPECT_TRUE( first->getFullSignature() != variation3->getFullSignature() );

	co::RefPtr<co::ITypeBuilder> var4builder = TestHelper::createBuilder( co::TK_INTERFACE, "MethodSignatureTest.Variation4", tct.get() );
	co::RefPtr<co::IMethodBuilder> var4methodBuilder = var4builder->defineMethod( "Ask" );
	var4methodBuilder->defineParameter( "question", TestHelper::type( "string" ), true, false );
	var4methodBuilder->defineReturnType( TestHelper::type("string") );
	var4methodBuilder->createMethod();
	co::IInterfaceType* variation4 = dynamic_cast<co::IInterfaceType*>( var4builder->createType() );

	EXPECT_TRUE( first->getBinarySignature() != variation4->getBinarySignature() );
	EXPECT_TRUE( first->getFullSignature() != variation4->getFullSignature() );
	
	co::RefPtr<co::ITypeBuilder> var5builder = TestHelper::createBuilder( co::TK_INTERFACE, "MethodSignatureTest.Variation5", tct.get() );
	co::RefPtr<co::IMethodBuilder> var5methodBuilder = var5builder->defineMethod( "Ask" );
	var5methodBuilder->defineParameter( "question", TestHelper::type( "string" ), true, false );
	var5methodBuilder->defineReturnType( TestHelper::type("int32") );
	var5methodBuilder->createMethod();
	co::IInterfaceType* variation5 = dynamic_cast<co::IInterfaceType*>( var5builder->createType() );

	EXPECT_TRUE( first->getBinarySignature() != variation5->getBinarySignature() );
	EXPECT_TRUE( first->getFullSignature() != variation5->getFullSignature() );
	EXPECT_TRUE( variation4->getBinarySignature() != variation5->getBinarySignature() );
	
	EXPECT_NO_THROW( tct->commit() );
}

TEST( SignatureCalculatorTests, attributeSignature )
{
	co::RefPtr<co::ITypeCreationTransaction> tct = createTypeCreationTransaction();

	co::RefPtr<co::ITypeBuilder> builder1 = TestHelper::createBuilder( co::TK_INTERFACE, "AttributeSignatureTest.First", tct.get() );
	builder1->defineAttribute( "name", TestHelper::type( "string" ), true );
	co::IInterfaceType* first = dynamic_cast<co::IInterfaceType*>( builder1->createType() );

	//variations with the same binary signature
	co::RefPtr<co::ITypeBuilder> builder2 = TestHelper::createBuilder( co::TK_INTERFACE, "AttributeSignatureTest.SameBinary1", tct.get() );
	builder2->defineAttribute( "name", TestHelper::type( "string" ), true );
	co::IInterfaceType* sameBinary1 = dynamic_cast<co::IInterfaceType*>( builder2->createType() );

	co::RefPtr<co::ITypeBuilder> builder3 = TestHelper::createBuilder( co::TK_INTERFACE, "AttributeSignatureTest.SameBinary2", tct.get() );
	builder3->defineAttribute( "fullName", TestHelper::type( "string" ), true );
	co::IInterfaceType* sameBinary2 = dynamic_cast<co::IInterfaceType*>( builder3->createType() );

	// variations with diferent signature
	co::RefPtr<co::ITypeBuilder> builder4 = TestHelper::createBuilder( co::TK_INTERFACE, "AttributeSignatureTest.Variation1", tct.get() );
	builder4->defineAttribute( "fullName", TestHelper::type( "string" ), false );
	co::IInterfaceType* variation1 = dynamic_cast<co::IInterfaceType*>( builder4->createType() );

	co::RefPtr<co::ITypeBuilder> builder5 = TestHelper::createBuilder( co::TK_INTERFACE, "AttributeSignatureTest.Variation2", tct.get() );
	builder5->defineAttribute( "name", TestHelper::type( "int32" ), false );
	co::IInterfaceType* variation2 = dynamic_cast<co::IInterfaceType*>( builder5->createType() );

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
