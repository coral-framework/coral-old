/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#include "TestHelper.h"

#include <co/Coral.h>
#include <co/RefPtr.h>
#include <co/System.h>
#include <co/EnumType.h>
#include <co/ArrayType.h>
#include <co/Namespace.h>
#include <co/StructType.h>
#include <co/TypeBuilder.h>
#include <co/TypeManager.h>
#include <co/AttributeInfo.h>
#include <co/InterfaceInfo.h>
#include <co/ComponentType.h>
#include <co/MethodBuilder.h>
#include <co/InterfaceType.h>
#include <co/NativeClassType.h>
#include <co/SemanticException.h>
#include <co/IllegalNameException.h>
#include <co/MissingInputException.h>
#include <co/IllegalArgumentException.h>
#include <co/UnsupportedOperationException.h>

#include <gtest/gtest.h>

/*
	TypeBuilder general tests:
	The above tests are concerned in testing TypeBuilder main features.
 */

TEST( TypeBuilderTests, cyclicDependencies )
{
	co::RefPtr<co::TypeCreationTransaction> tct = createTypeCreationTransaction();
	co::RefPtr<co::TypeBuilder> structABuilder = TestHelper::createBuilder( co::TK_STRUCT, "ciclicDepTest.structs.A", tct.get() );

	// type struct A depends on type struct B, which must be loaded if not loaded yet
	// check if B exists : B should not exist yet
	EXPECT_TRUE( TestHelper::type( "ciclicDepTest.structs.B" ) == NULL );

	// create B
	co::RefPtr<co::TypeBuilder> structBBuilder = TestHelper::createBuilder( co::TK_STRUCT, "ciclicDepTest.structs.B", tct.get() );

	// B struct depends on type A, creating a dependency cycle
	// check if A exists: although type A hasn't finished being created, it must be
	// available from the moment its builder is created, so its type should exist by now
	co::Type* typeA = TestHelper::type( "ciclicDepTest.structs.A" );

	EXPECT_TRUE( typeA != NULL );

	EXPECT_NO_THROW( structBBuilder->defineAttribute( "attributeOfTypeA", typeA, false ) );

	// createType the B type
	co::Type* typeB = structBBuilder->createType();
	EXPECT_NO_THROW( structABuilder->defineAttribute( "attributeOfTypeB", typeB, false ) );

	// create type A
	co::Type* createdTypeA = structABuilder->createType();

	EXPECT_TRUE( typeA == createdTypeA );

	EXPECT_NO_THROW( tct->commit() );
}


TEST( TypeBuilderTests, typeNameInvalidIdentifier )
{
	co::RefPtr<co::TypeCreationTransaction> tct = createTypeCreationTransaction();
	EXPECT_THROW( co::getSystem()->getTypes()->getRootNS()->defineType( "A.Dot.Separated.TypeName", co::TK_STRUCT, tct.get() ), co::IllegalNameException );

	tct->rollback();
}

/*
	TypeBuilder tests by types:
	The above tests are concerned in testing TypeBuilder with main Coral Types.
	There is at least one test for each of the above types, divided in sections:

		ComponentType
		EnumType
		ExceptionType
		InterfaceType
		NativeClassType
		StructType
 */

// ComponentType:

TEST( TypeBuilderTests, componentInvalidDefinitions )
{
	co::RefPtr<co::TypeCreationTransaction> tct = createTypeCreationTransaction();

	// a dummy type to be used in the test
	co::Type* anyType = co::getSystem()->getTypes()->findType( "any" );

	// create builder
	co::RefPtr<co::TypeBuilder> cbuilder = TestHelper::createBuilder( co::TK_COMPONENT, "TypeBuilderTests_aComponent", tct.get() );

	// a dummy interface to be used in the tests
	co::RefPtr<co::TypeBuilder> auxbuilder = TestHelper::createBuilder( co::TK_INTERFACE, "TypeBuilderTests_componentInterface", tct.get() );
	auxbuilder->defineAttribute( "test", anyType, true );

	co::InterfaceType* testInterfaceType = dynamic_cast<co::InterfaceType*>( auxbuilder->createType() );

	// invalid component class definition
	EXPECT_THROW( cbuilder->defineIdentifier( "identifier" ), co::UnsupportedOperationException );
	EXPECT_THROW( cbuilder->defineAttribute( "attributeName", anyType, false ), co::UnsupportedOperationException );
	EXPECT_THROW( cbuilder->defineSuperType( testInterfaceType ), co::UnsupportedOperationException );
	EXPECT_THROW( cbuilder->defineInterface( "testIntMember", NULL, false ), co::IllegalArgumentException );
	EXPECT_THROW( cbuilder->defineMethod( "testMethod" ), co::UnsupportedOperationException );
	EXPECT_THROW( cbuilder->defineNativeClass( "testHeader", "testName" ), co::UnsupportedOperationException );

	EXPECT_NO_THROW( tct->rollback() );
}

TEST( TypeBuilderTests, componentDefinition )
{
	co::RefPtr<co::TypeCreationTransaction> tct = createTypeCreationTransaction();
	co::RefPtr<co::TypeBuilder> componentBuilder = TestHelper::createBuilder( co::TK_COMPONENT, "TypeBuilderTests.ComponentType", tct.get() );
	co::RefPtr<co::TypeBuilder> interfaceBuilder = TestHelper::createBuilder( co::TK_INTERFACE, "TypeBuilderTests.TestInterfaceType", tct.get() );

	interfaceBuilder->defineAttribute( "testName", TestHelper::type( "string" ), true);

	co::InterfaceType* interfaceTest = dynamic_cast<co::InterfaceType*>( interfaceBuilder->createType() );

	componentBuilder->defineInterface( "Receptacle", interfaceTest, false );
	componentBuilder->defineInterface( "Facet", interfaceTest, true );

	EXPECT_THROW( componentBuilder->defineInterface( "NullInterface", NULL, false ), co::IllegalArgumentException );
	EXPECT_THROW( componentBuilder->defineInterface( "Receptacle", interfaceTest, false ), co::IllegalNameException );

	EXPECT_NO_THROW( tct->commit() );

	co::ComponentType* componentType = dynamic_cast<co::ComponentType*>( componentBuilder->createType() );
	ASSERT_TRUE( componentType != NULL );

	ASSERT_TRUE( componentType->getInterfaces().getSize() == 2 );

	co::MemberInfo* memberInfo = componentType->getMember( "Receptacle" );
	ASSERT_TRUE( memberInfo != NULL );
	co::InterfaceInfo* receptacle = dynamic_cast<co::InterfaceInfo*>( memberInfo );
	ASSERT_TRUE( receptacle != NULL );

	memberInfo = componentType->getMember( "Facet" );
	ASSERT_TRUE( memberInfo != NULL );
	co::InterfaceInfo* facet = dynamic_cast<co::InterfaceInfo*>( memberInfo );
	ASSERT_TRUE( facet != NULL );

	ASSERT_TRUE( receptacle->getName() == "Receptacle" );
	ASSERT_TRUE( receptacle->getType() != NULL );
	ASSERT_TRUE( receptacle->getType() == interfaceTest );
	ASSERT_TRUE( receptacle->getIsFacet() == false );

	ASSERT_TRUE( facet->getName() == "Facet" );
	ASSERT_TRUE( facet->getType() != NULL );
	ASSERT_TRUE( facet->getType() == interfaceTest );
	ASSERT_TRUE( facet->getIsFacet() == true );
}

TEST( TypeBuilderTests, componentMissingInput )
{
	co::RefPtr<co::TypeCreationTransaction> tct = createTypeCreationTransaction();
	co::RefPtr<co::TypeBuilder> componentBuilder = TestHelper::createBuilder( co::TK_COMPONENT, "TypeBuilderTests.InvalidComponentType", tct.get() );

	EXPECT_THROW( tct->commit(), co::MissingInputException );
	EXPECT_NO_THROW( tct->rollback() );

	EXPECT_TRUE( TestHelper::type( "TypeBuilderTests.InvalidComponentType" ) == NULL );
}

TEST( TypeBuilderTests, componentGetInterfaces )
{
	co::RefPtr<co::TypeCreationTransaction> tct = createTypeCreationTransaction();
	co::RefPtr<co::TypeBuilder> componentBuilder = TestHelper::createBuilder( co::TK_COMPONENT, "ComponentTypeTests.secondComponentType", tct.get() );
	co::RefPtr<co::TypeBuilder> interfaceBuilder = TestHelper::createBuilder( co::TK_INTERFACE, "ComponentTypeTests.secondTestInterfaceType", tct.get() );

	interfaceBuilder->defineAttribute( "testName",  TestHelper::type( "string" ), true);
	co::InterfaceType* interfaceTest = dynamic_cast<co::InterfaceType*>( interfaceBuilder->createType() );

	componentBuilder->defineInterface( "Client_1", interfaceTest, false );
	componentBuilder->defineInterface( "Provid_1", interfaceTest, true );
	componentBuilder->defineInterface( "Provid_2", interfaceTest, true );
	componentBuilder->defineInterface( "Client_2", interfaceTest, false );
	componentBuilder->defineInterface( "Provid_3", interfaceTest, true );
	componentBuilder->defineInterface( "Client_3", interfaceTest, false );

	EXPECT_NO_THROW( tct->commit() );

	co::ComponentType* componentType = dynamic_cast<co::ComponentType*>( componentBuilder->createType() );
	ASSERT_TRUE( componentType != NULL );

	ASSERT_TRUE( componentType->getInterfaces().getSize() == 6 );

	co::ArrayRange<co::InterfaceInfo* const> facets = componentType->getFacets();
	ASSERT_TRUE( facets.getSize() == 3 );
	for( ; facets; facets.popFirst() )
		ASSERT_TRUE( facets.getFirst()->getIsFacet() );

	co::ArrayRange<co::InterfaceInfo* const> receptacles = componentType->getReceptacles();
	ASSERT_TRUE( receptacles.getSize() == 3 );
	for( ; receptacles; receptacles.popFirst() )
		ASSERT_FALSE( receptacles.getFirst()->getIsFacet() );
}

// EnumType:

TEST( TypeBuilderTests, enumInvalidDefinition )
{
	co::RefPtr<co::TypeCreationTransaction> tct = createTypeCreationTransaction();
	co::RefPtr<co::TypeBuilder> ebuilder = TestHelper::createBuilder( co::TK_ENUM, "TypeBuilderTests_anEnum", tct.get() );

	// dummy type to be used in the test
	co::Type* anyType = co::getSystem()->getTypes()->findType( "any" );

	// a dummy interface to be used in the tests
	co::RefPtr<co::TypeBuilder> auxbuilder = TestHelper::createBuilder( co::TK_INTERFACE, "TypeBuilderTests_enumSuperInterface", tct.get() );
	auxbuilder->defineAttribute( "test", anyType, true );

	co::InterfaceType* testInterfaceType = dynamic_cast<co::InterfaceType*>( auxbuilder->createType() );

	EXPECT_THROW( ebuilder->defineIdentifier( "3identifier" ), co::IllegalNameException );
	EXPECT_THROW( ebuilder->defineAttribute( "attributeName", anyType, false ), co::UnsupportedOperationException );
	EXPECT_THROW( ebuilder->defineSuperType( testInterfaceType ), co::UnsupportedOperationException );

	EXPECT_NO_THROW( tct->rollback() );
}

TEST( TypeBuilderTests, enumDefinition )
{
	co::RefPtr<co::TypeCreationTransaction> tct = createTypeCreationTransaction();
	co::RefPtr<co::TypeBuilder> builder = TestHelper::createBuilder( co::TK_ENUM, "EnumTypeTests.BuilderTestEnum", tct.get() );

	EXPECT_THROW( builder->defineIdentifier( "" ), co::IllegalNameException );

	builder->defineIdentifier( "VALUE_0" );
	builder->defineIdentifier( "VALUE_1" );

	EXPECT_NO_THROW( tct->commit() );

	co::Type* type = builder->createType();
	ASSERT_TRUE( type != NULL );

	// check if the enum got the correct identifiers
	co::EnumType* enumType = dynamic_cast<co::EnumType*>( type );
	ASSERT_TRUE( enumType != NULL );

	EXPECT_EQ( enumType->getIdentifiers().getFirst(), "VALUE_0" );
	EXPECT_EQ( enumType->getIdentifiers().getLast(), "VALUE_1" );
}

TEST( TypeBuilderTests, enumMissingInput )
{
	co::RefPtr<co::TypeCreationTransaction> tct = createTypeCreationTransaction();
	co::RefPtr<co::TypeBuilder> builder = TestHelper::createBuilder( co::TK_ENUM, "EnumTypeTests.MissingInputEnum", tct.get() );

	EXPECT_THROW( tct->commit(), co::MissingInputException );
	EXPECT_NO_THROW( tct->rollback() );

	EXPECT_TRUE( TestHelper::type( "EnumTypeTests.MissingInputEnum" )  == NULL );
}

TEST( TypeBuilderTests, enumCheckIdentifiers )
{
	co::RefPtr<co::TypeCreationTransaction> tct = createTypeCreationTransaction();
	co::RefPtr<co::TypeBuilder> builder = TestHelper::createBuilder( co::TK_ENUM, "EnumTypeTests.CheckIdentifiersTestEnum", tct.get() );

	builder->defineIdentifier( "VALUE_0" );
	builder->defineIdentifier( "VALUE_1" );
	builder->defineIdentifier( "VALUE_2" );

	EXPECT_NO_THROW( tct->commit() );

	co::Type* type = builder->createType();
	co::EnumType* enumType = dynamic_cast<co::EnumType*>( type );

	ASSERT_EQ( enumType->getValueOf( "" ) , -1 );
	ASSERT_EQ( enumType->getValueOf( "ND" ) , -1 );
	ASSERT_EQ( enumType->getValueOf( "VALUE_0" ) , 0 );
	ASSERT_EQ( enumType->getValueOf( "VALUE_1" ) , 1 );
	ASSERT_EQ( enumType->getValueOf( "VALUE_2" ) , 2 );
}

TEST( TypeBuilderTests, enumInsertDuplicateIdentifier )
{
	co::RefPtr<co::TypeCreationTransaction> tct = createTypeCreationTransaction();
	co::RefPtr<co::TypeBuilder> builder = TestHelper::createBuilder( co::TK_ENUM, "EnumTypeTests.InsertDuplicateIdentifierTestEnum", tct.get() );

	builder->defineIdentifier( "VALUE_0" );

	EXPECT_THROW( builder->defineIdentifier( "VALUE_0" ), co::IllegalNameException );

	EXPECT_NO_THROW( tct->commit() );
}

TEST( TypeBuilderTests, enumCheckValidIdentifiers )
{
	co::RefPtr<co::TypeCreationTransaction> tct = createTypeCreationTransaction();
	co::RefPtr<co::TypeBuilder> builder = TestHelper::createBuilder( co::TK_ENUM, "EnumTypeTests.checkValidIdentifiersEnum", tct.get() );

	EXPECT_NO_THROW( builder->defineIdentifier( "VALUE" ) );
	EXPECT_NO_THROW( builder->defineIdentifier( "LALALA_123_FFF" ) );
	EXPECT_NO_THROW( builder->defineIdentifier( "AnotherValue" ) );

	EXPECT_THROW( builder->defineIdentifier( "1VAL" ) , co::IllegalNameException );
	EXPECT_THROW( builder->defineIdentifier( "VAL!@#$" ) , co::IllegalNameException );
	EXPECT_THROW( builder->defineIdentifier( "VAL~" ) , co::IllegalNameException );
	EXPECT_THROW( builder->defineIdentifier( "" ) , co::IllegalNameException );
	EXPECT_THROW( builder->defineIdentifier( " VAL" ) , co::IllegalNameException );

	EXPECT_NO_THROW( tct->commit() );
}

// ExceptionType:

TEST( TypeBuilderTests, exceptionCreation )
{
	co::Namespace* rootNS = co::getSystem()->getTypes()->getRootNS();

	co::RefPtr<co::TypeCreationTransaction> tct = createTypeCreationTransaction();
	co::RefPtr<co::TypeBuilder> builder = rootNS->defineType( "aTypeName", co::TK_EXCEPTION, tct.get() );
	ASSERT_TRUE( builder.isValid() );
	EXPECT_EQ( builder->getNamespace(), rootNS );
	EXPECT_EQ( builder->getKind(), co::TK_EXCEPTION );
	EXPECT_EQ( builder->getTypeName(), "aTypeName" );

	EXPECT_NO_THROW( tct->commit() );
}

TEST( TypeBuilderTests, exceptionsAfterCreation )
{
	co::Namespace* rootNS = co::getSystem()->getTypes()->getRootNS();

	co::RefPtr<co::TypeCreationTransaction> tct = createTypeCreationTransaction();
	co::RefPtr<co::TypeBuilder> builder = rootNS->defineType( "exceptionsAfterCreationTestType", co::TK_STRUCT, tct.get() );
	builder->defineAttribute( "name", TestHelper::type( "string" ), false );

	EXPECT_NO_THROW( builder->createType() );

	// after the creation of the type, no changes can be made to the typeBuilder.
	EXPECT_THROW( builder->defineAttribute( "fullName", TestHelper::type( "string" ), false ), co::UnsupportedOperationException );

	EXPECT_NO_THROW( tct->commit() );
}

// InterfaceType:

TEST( TypeBuilderTests, interfaceInvalidDefinition )
{
	co::RefPtr<co::TypeCreationTransaction> tct = createTypeCreationTransaction();
	co::RefPtr<co::TypeBuilder> ibuilder = TestHelper::createBuilder( co::TK_INTERFACE, "TypeBuilderTests_anInvalidInterface", tct.get() );

	// dummy type to be used in the test
	co::Type* anyType = co::getSystem()->getTypes()->findType( "any" );

	// a dummy interface to be used in the tests
	co::RefPtr<co::TypeBuilder> auxbuilder = TestHelper::createBuilder( co::TK_INTERFACE, "TypeBuilderTests_superInterface", tct.get() );
	auxbuilder->defineAttribute( "test", anyType, true );

	co::InterfaceType* testInterfaceType = dynamic_cast<co::InterfaceType*>( auxbuilder->createType() );

	// invalid interface definitions
	EXPECT_THROW( ibuilder->defineIdentifier( "identifier" ), co::UnsupportedOperationException );
	EXPECT_THROW( ibuilder->defineAttribute( "2attributeName", anyType, false ), co::IllegalNameException );
	EXPECT_THROW( ibuilder->defineSuperType( NULL ), co::IllegalArgumentException );
	EXPECT_THROW( ibuilder->defineInterface( "testIntMember", testInterfaceType, false ), co::UnsupportedOperationException );
	EXPECT_THROW( ibuilder->defineMethod( "$testMethod" ), co::IllegalNameException );
	EXPECT_THROW( ibuilder->defineMethod( "" ), co::IllegalNameException );
	EXPECT_THROW( ibuilder->defineNativeClass( "testHeader", "testName" ), co::UnsupportedOperationException );

	EXPECT_NO_THROW( tct->rollback() );
}

TEST( TypeBuilderTests, interfaceDefinition )
{
	co::RefPtr<co::TypeCreationTransaction> tct = createTypeCreationTransaction();
	co::RefPtr<co::TypeBuilder> builder = TestHelper::createBuilder( co::TK_INTERFACE, "builderTest.AInterfaceType", tct.get() );

	// try creating an incomplete interface
	EXPECT_THROW( builder->createType(), co::MissingInputException );

	// define builderTest.AInterfaceType:
	co::Type* stringType = TestHelper::type( "string" );
	EXPECT_NO_THROW( builder->defineAttribute( "testName", stringType, true ) );
	EXPECT_NO_THROW( builder->defineAttribute( "testSecondName", stringType, false ) );
	co::RefPtr<co::MethodBuilder> mb = builder->defineMethod( "childMethod" );
	EXPECT_NO_THROW( mb->createMethod() );

	// define builderTest.ASuperInterfaceType:
	co::RefPtr<co::TypeBuilder> superBuilder = TestHelper::createBuilder( co::TK_INTERFACE, "builderTest.ASuperInterfaceType", tct.get() );
	superBuilder->defineAttribute( "name", stringType, false );
	co::RefPtr<co::MethodBuilder> mb2;
	EXPECT_NO_THROW( ( mb2 = superBuilder->defineMethod( "parentMethod" ) ) );
	EXPECT_NO_THROW( mb2->createMethod() );

	// get super-type even before finishing creating it (it should be available)
	co::InterfaceType* superInterface = dynamic_cast<co::InterfaceType*>( TestHelper::type( "builderTest.ASuperInterfaceType" ) );
	EXPECT_TRUE( superInterface != NULL );

	// add the super-type to the interface type
	builder->defineSuperType( superInterface );

	EXPECT_THROW( builder->defineSuperType( stringType ), co::IllegalArgumentException );

	EXPECT_NO_THROW( superBuilder->createType() );

	co::InterfaceType* interfaceType = dynamic_cast<co::InterfaceType*>( builder->createType() );
	EXPECT_TRUE( interfaceType != NULL );

	ASSERT_TRUE( interfaceType->getMember( "name" ) != NULL );
	ASSERT_TRUE( interfaceType->getMember( "parentMethod" ) != NULL );
	ASSERT_TRUE( interfaceType->getMember( "childMethod" ) != NULL );
	ASSERT_TRUE( interfaceType->getMember( "testName" ) != NULL );
	ASSERT_TRUE( interfaceType->getMember( "testSecondName" ) != NULL );

	ASSERT_EQ( 1, interfaceType->getMemberMethods().getSize() );
	ASSERT_EQ( 2, interfaceType->getMemberAttributes().getSize() );

	ASSERT_TRUE( interfaceType->getSuperInterfaces().getSize() == 1 );
	ASSERT_TRUE( interfaceType->getSuperInterfaces().getFirst() == superInterface );
	ASSERT_TRUE( interfaceType->getSubInterfaces().getSize() == 0 );

	ASSERT_TRUE( superInterface->getSuperInterfaces().getSize() == 1 );
	ASSERT_TRUE( superInterface->getSubInterfaces().getSize() == 1 );
	ASSERT_TRUE( superInterface->getSubInterfaces().getFirst() == interfaceType );

	EXPECT_NO_THROW( tct->commit() );
}

TEST( TypeBuilderTests, interfaceGetMethods )
{
	co::RefPtr<co::TypeCreationTransaction> tct = createTypeCreationTransaction();
	co::RefPtr<co::TypeBuilder> builder = TestHelper::createBuilder( co::TK_INTERFACE, "interfaceGetMethodsTest.Foo", tct.get() );

	builder->defineAttribute( "test", TestHelper::type( "string" ), true );
	co::InterfaceType* interfaceType = dynamic_cast<co::InterfaceType*>( builder->createType() );

	ASSERT_NO_THROW( interfaceType->getMemberMethods() );

	ASSERT_EQ( 0, interfaceType->getMemberMethods().getSize() );

	EXPECT_NO_THROW( tct->commit() );
}

// NativeClassType:

TEST( TypeBuilderTests, nativeClassInvalidDefinitions )
{
	co::RefPtr<co::TypeCreationTransaction> tct = createTypeCreationTransaction();

	// a dummy type to be used in the test
	co::Type* anyType = co::getSystem()->getTypes()->findType( "any" );

	// create builder
	co::RefPtr<co::TypeBuilder> nbuilder = TestHelper::createBuilder( co::TK_NATIVECLASS, "TypeBuilderTests_anNativeClass", tct.get() );

	// a dummy interface to be used in the tests
	co::RefPtr<co::TypeBuilder> auxbuilder = TestHelper::createBuilder( co::TK_INTERFACE, "TypeBuilderTests_anInterface", tct.get() );
	auxbuilder->defineAttribute( "test", anyType, true );

	co::InterfaceType* testInterfaceType = dynamic_cast<co::InterfaceType*>( auxbuilder->createType() );

	// invalid native class definition
	EXPECT_THROW( nbuilder->defineIdentifier( "identifier" ), co::UnsupportedOperationException );
	EXPECT_THROW( nbuilder->defineAttribute( "0_attributeName", anyType, false ), co::IllegalNameException );
	EXPECT_THROW( nbuilder->defineSuperType( testInterfaceType ), co::UnsupportedOperationException );
	EXPECT_THROW( nbuilder->defineInterface( "testIntMember", testInterfaceType, false ), co::UnsupportedOperationException );
	EXPECT_THROW( nbuilder->defineMethod( "" ), co::IllegalNameException );
	EXPECT_THROW( nbuilder->defineNativeClass( "header", "" ), co::IllegalArgumentException );

	EXPECT_NO_THROW( tct->rollback() );
}

TEST( TypeBuilderTests, nativeClassDefinition )
{
	co::RefPtr<co::TypeCreationTransaction> tct = createTypeCreationTransaction();
	co::RefPtr<co::TypeBuilder> builder = TestHelper::createBuilder( co::TK_NATIVECLASS, "NativeClassbuilderTest.BuilderTestNativeClass", tct.get() );

	EXPECT_THROW( builder->defineNativeClass( "headerName", "" ), co::IllegalArgumentException );
	EXPECT_THROW( builder->defineNativeClass( "", "nativeName" ), co::IllegalArgumentException );

	builder->defineAttribute( "testName", TestHelper::type( "string" ), true );

	builder->defineAttribute( "age", TestHelper::type( "uint32" ), false );

	co::RefPtr<co::MethodBuilder> mb = builder->defineMethod( "childMethod" );
	mb->createMethod();

	builder->defineNativeClass( "MyHeaderName", "myNativeName" );

	co::NativeClassType* nativeType = dynamic_cast<co::NativeClassType*>( builder->createType() );

	ASSERT_TRUE( nativeType->getMember( "testName" ) != NULL );
	ASSERT_TRUE( nativeType->getMember( "age" ) != NULL );
	ASSERT_TRUE( nativeType->getMember( "childMethod" ) != NULL );
	ASSERT_EQ( "myNativeName", nativeType->getNativeName() );
	ASSERT_EQ( "MyHeaderName", nativeType->getNativeHeaderFile() );
	ASSERT_EQ( 1, nativeType->getMemberMethods().getSize() );
	ASSERT_EQ( 2, nativeType->getMemberAttributes().getSize() );

	EXPECT_NO_THROW( tct->commit() );
}

TEST( TypeBuilderTests, nativeClassMemberClash )
{
	co::RefPtr<co::TypeCreationTransaction> tct = createTypeCreationTransaction();
	co::RefPtr<co::TypeBuilder> builder = TestHelper::createBuilder( co::TK_NATIVECLASS, "NativeClassbuilderTest.MemberClashTestNativeClass", tct.get() );

	builder->defineAttribute( "testName", TestHelper::type( "string" ), true );

	co::RefPtr<co::MethodBuilder> mb = builder->defineMethod( "getTestName" );
	mb->createMethod();
	builder->defineNativeClass( "MyHeaderName", "myNativeName" );

	EXPECT_THROW( tct->commit(), co::SemanticException );
	EXPECT_NO_THROW( tct->rollback() );
}

TEST( TypeBuilderTests, nativeClassMissingInput )
{
	co::RefPtr<co::TypeCreationTransaction> tct = createTypeCreationTransaction();
	co::RefPtr<co::TypeBuilder> builder = TestHelper::createBuilder( co::TK_NATIVECLASS, "NativeClassbuilderTest.MissingInputNativeClass", tct.get() );

	EXPECT_THROW( tct->commit(), co::MissingInputException );
	EXPECT_NO_THROW( tct->rollback() );
}

TEST( TypeBuilderTests, nativeClassMissingHeader )
{
	co::RefPtr<co::TypeCreationTransaction> tct = createTypeCreationTransaction();
	co::RefPtr<co::TypeBuilder> builder = TestHelper::createBuilder( co::TK_NATIVECLASS, "NativeClassbuilderTest.MissingHeadersNativeClass", tct.get() );
	builder->defineAttribute( "testName", TestHelper::type( "string" ), true );

	EXPECT_THROW( tct->commit(), co::MissingInputException );
	EXPECT_NO_THROW( tct->rollback() );
}

// StructType:

TEST( TypeBuilderTests, structInvalidDefinition )
{
	co::RefPtr<co::TypeCreationTransaction> tct = createTypeCreationTransaction();
	co::RefPtr<co::TypeBuilder> sbuilder = TestHelper::createBuilder( co::TK_STRUCT, "TypeBuilderTests_anStruct", tct.get() );

	// dummy type to be used in the test
	co::Type* anyType = co::getSystem()->getTypes()->findType( "any" );

	// a dummy interface to be used in the tests
	co::RefPtr<co::TypeBuilder> auxbuilder = TestHelper::createBuilder( co::TK_INTERFACE, "TypeBuilderTests_anInterface", tct.get() );
	auxbuilder->defineAttribute( "test", anyType, true );

	co::InterfaceType* testInterfaceType = dynamic_cast<co::InterfaceType*>( auxbuilder->createType() );

	// invalid struct definitions
	EXPECT_THROW( sbuilder->defineIdentifier( "identifier" ), co::UnsupportedOperationException );
	EXPECT_THROW( sbuilder->defineAttribute( "1attributeName", anyType, false ), co::IllegalNameException );
	EXPECT_THROW( sbuilder->defineSuperType( testInterfaceType ), co::UnsupportedOperationException );
	EXPECT_THROW( sbuilder->defineInterface( "testIntMember", testInterfaceType, false ), co::UnsupportedOperationException );
	EXPECT_THROW( sbuilder->defineMethod( "testMethod" ), co::UnsupportedOperationException );
	EXPECT_THROW( sbuilder->defineNativeClass( "testHeader", "testName" ), co::UnsupportedOperationException );

	EXPECT_NO_THROW( tct->rollback() );
}

TEST( TypeBuilderTests, structMissingInput )
{
	co::RefPtr<co::TypeCreationTransaction> transaction = createTypeCreationTransaction();
	co::RefPtr<co::TypeBuilder> builder = TestHelper::createBuilder( co::TK_STRUCT, "aStructType", transaction.get() );

	EXPECT_THROW( transaction->commit(), co::MissingInputException );

	EXPECT_NO_THROW( transaction->rollback() );

	EXPECT_TRUE( TestHelper::type( "aStructType" ) == NULL );
}

TEST( TypeBuilderTests, structDefinition )
{
	co::RefPtr<co::TypeCreationTransaction> transaction = createTypeCreationTransaction();
	co::RefPtr<co::TypeBuilder> builder = TestHelper::createBuilder( co::TK_STRUCT, "StructTypeTests.BuidlerTestStruct", transaction.get() );

	co::Type* uint32Type = TestHelper::type( "uint32" );

	builder->defineAttribute( "myAtribute", uint32Type, false );

	EXPECT_THROW( builder->defineAttribute( "myReadOnlyAtribute", uint32Type, true ), co::IllegalArgumentException );
	EXPECT_THROW( builder->defineAttribute( "myAtribute", uint32Type, false ), co::IllegalNameException );
	EXPECT_THROW( builder->defineAttribute( "myNullTypeAtribute", NULL, false ), co::IllegalArgumentException );

	EXPECT_NO_THROW( transaction->commit() );

	co::StructType* structType = dynamic_cast<co::StructType*>( TestHelper::type( "StructTypeTests.BuidlerTestStruct" ) );
	ASSERT_TRUE( structType != NULL );

	co::ArrayRange<co::AttributeInfo* const> attibutes = structType->getMemberAttributes();
	ASSERT_TRUE( attibutes.getSize() == 1 );

	co::AttributeInfo* attr = attibutes.getFirst() ;

	ASSERT_TRUE( attr->getName() == "myAtribute" );
	ASSERT_TRUE( attr->getType() == uint32Type );
	ASSERT_TRUE( attr->getIsReadOnly() == false );
}
