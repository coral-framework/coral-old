/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#include "TestHelper.h"
#include <co/Uuid.h>
#include <co/Coral.h>
#include <co/ISystem.h>
#include <co/CSLError.h>
#include <co/INamespace.h>
#include <co/IReflector.h>
#include <co/IMethod.h>
#include <co/ITypeBuilder.h>
#include <co/ITypeManager.h>
#include <co/ICompositeType.h>
#include <co/IField.h>
#include <co/IPort.h>
#include <co/IllegalCastException.h>
#include <co/MissingInputException.h>
#include <co/ITypeTransaction.h>
#include <co/IllegalArgumentException.h>
#include <co/NotSupportedException.h>
#include <gtest/gtest.h>

TEST( ReflectorTests, basicReflectors )
{
	static const char* BASIC_TYPE_NAMES[] = {
		"any", "bool", "int8", "uint8", "int16", "uint16", "int32", "uint32",
		"int64", "uint64", "float", "double", "string",
		"co.TypeKind",	// an enum
	};

	const int NUM_TYPES = CORAL_ARRAY_LENGTH( BASIC_TYPE_NAMES );

	co::IType* types[NUM_TYPES];
	for( int i = 0; i < NUM_TYPES; ++i )
	{
		types[i] = co::getType( BASIC_TYPE_NAMES[i] );
		ASSERT_TRUE( types[i] != NULL );
	}

	// dummy arguments
	co::Any any;
	co::Range<co::Any const> anyRange;

	co::uint8 buffer1[sizeof(co::Any)];
	co::uint8 buffer2[sizeof(co::Any)];

	// make sure all types have a BasicReflector
	for( int i = 0; i < NUM_TYPES; ++i )
	{
		co::IReflector* reflector = types[i]->getReflector();
		ASSERT_TRUE( reflector != NULL );

		EXPECT_EQ( types[i], reflector->getType() );
		EXPECT_GT( reflector->getSize(), 0 );

		ASSERT_NO_THROW( reflector->createValue( buffer1 ) );
		ASSERT_NO_THROW( reflector->createValue( buffer2 ) );

		ASSERT_NO_THROW( reflector->copyValue( buffer2, buffer1 ) );

		ASSERT_NO_THROW( reflector->destroyValue( buffer1 ) );
		ASSERT_NO_THROW( reflector->destroyValue( buffer2 ) );

		EXPECT_THROW( reflector->newInstance(), co::NotSupportedException );
		EXPECT_THROW( reflector->newDynamicProxy( NULL ), co::NotSupportedException );
		EXPECT_THROW( reflector->getField( any, NULL, any ), co::NotSupportedException );
		EXPECT_THROW( reflector->setField( any, NULL, any ), co::NotSupportedException );
		EXPECT_THROW( reflector->invoke( any, NULL, anyRange, any ), co::NotSupportedException );
	}
}

TEST( ReflectorTests, arrayReflectors )
{
	co::IType* type = co::getType( "co.IType[]" );
	ASSERT_TRUE( type != NULL );
	ASSERT_EQ( type->getKind(), co::TK_ARRAY );

	co::IReflector* reflector = type->getReflector();
	ASSERT_TRUE( reflector != NULL );

	EXPECT_EQ( type, reflector->getType() );
	EXPECT_GT( reflector->getSize(), 0 );

	EXPECT_THROW( reflector->createValue( NULL ), co::NotSupportedException );
	EXPECT_THROW( reflector->copyValue( NULL, NULL ), co::NotSupportedException );
	EXPECT_THROW( reflector->destroyValue( NULL ), co::NotSupportedException );	
	EXPECT_THROW( reflector->newInstance(), co::NotSupportedException );
	EXPECT_THROW( reflector->newDynamicProxy( NULL ), co::NotSupportedException );

	// dummy arguments
	co::Any any;
	co::Range<co::Any const> anyRange;

	EXPECT_THROW( reflector->getField( any, NULL, any ), co::NotSupportedException );
	EXPECT_THROW( reflector->setField( any, NULL, any ), co::NotSupportedException );
	EXPECT_THROW( reflector->invoke( any, NULL, anyRange, any ), co::NotSupportedException );
}

TEST( ReflectorTests, reflectorComponent )
{
	co::IType* type = co::getType( "co.INamespace" );
	co::IReflector* reflector = type->getReflector();
	co::IObject* reflectorObject = reflector->getProvider();

	// test the reflector's component type
	co::IComponent* component = reflectorObject->getComponent();
	EXPECT_EQ( 1, component->getPorts().getSize() );
	EXPECT_EQ( "reflector", component->getPorts().getFirst()->getName() );

	// test the reflector component's facet
	EXPECT_EQ( reflector, reflectorObject->getService( component->getPorts().getFirst() ) );
}

TEST( ReflectorTests, exceptions )
{
	// test raised exception types
	ASSERT_THROW( co::getType( "co.INamespace" )->getReflector()->raise( "msg" ), co::NotSupportedException );
	ASSERT_THROW( co::getType( "co.IllegalCastException" )->getReflector()->raise( "msg" ), co::IllegalCastException );
	ASSERT_THROW( co::getType( "co.MissingInputException" )->getReflector()->raise( "msg" ), co::MissingInputException );

	// test raised exception messages
	try
	{
		co::getType( "co.IllegalArgumentException" )->getReflector()->raise( "my exception message!" );
		FAIL() << "an exception should have been raised";
	}
	catch( co::IllegalArgumentException& e )
	{
		ASSERT_EQ( "my exception message!", e.getMessage() );
	}
	catch( ... )
	{
		FAIL() << "wrong exception type";
	}
}

template<typename T>
T* getMember( co::IType* type, const char* memberName )
{
	assert( type );
	co::ICompositeType* ct = co::cast<co::ICompositeType>( type );
	T* mi = co::cast<T>( ct->getMember( memberName ) );
	assert( mi );
	return mi;
}

co::IField* getField( co::IType* type, const char* memberName )
{
	return getMember<co::IField>( type, memberName );
}

co::IMethod* getMethod( co::IType* type, const char* memberName )
{
	return getMember<co::IMethod>( type, memberName );
}

TEST( ReflectorTests, structSimple )
{
	co::IType* type = co::getType( "co.CSLError" );
	co::IReflector* reflector = type->getReflector();
	ASSERT_TRUE( reflector != NULL );

	// --- sanity checks:
	EXPECT_EQ( type, reflector->getType() );
	EXPECT_EQ( sizeof(co::CSLError), reflector->getSize() );

	// --- in-place allocation:
	co::uint8 memoryArea[sizeof(co::CSLError) * 2];

	co::CSLError* s1 = reinterpret_cast<co::CSLError*>( memoryArea );
	co::CSLError* s2 = reinterpret_cast<co::CSLError*>( memoryArea + sizeof(co::CSLError) );

	reflector->createValue( s1 );
	reflector->createValue( s2 );

	EXPECT_EQ( 0, s1->line );
	EXPECT_EQ( 0, s1->message.length() );
	EXPECT_EQ( 0, s1->filename.length() );

	EXPECT_EQ( 0, s2->line );
	EXPECT_EQ( 0, s2->message.length() );
	EXPECT_EQ( 0, s2->filename.length() );

	// --- obtain the necessary fields:
	co::IField* lineField = getField( type, "line" );
	ASSERT_TRUE( lineField != NULL );

	co::IField* messageField = getField( type, "message" );
	ASSERT_TRUE( messageField != NULL );

	co::IField* filenameField = getField( type, "filename" );
	ASSERT_TRUE( filenameField != NULL );

	// --- field setting:
	reflector->setField( s1, lineField, 7 );
	EXPECT_EQ( 7, s1->line );

	try
	{
		// try to set a string field with an int value (there's no coercion)
		reflector->setField( s1, messageField, 3 );
		EXPECT_FALSE( true );
	}
	catch( co::IllegalCastException& e )
	{
		EXPECT_EQ( "illegal cast from 'co::int32' to 'const std::string&'", e.getMessage() );
	}

	co::Any a1;
	std::string str( "my message" );
	a1.set<const std::string&>( str );

	reflector->setField( s1, messageField, a1 );
	EXPECT_EQ( "my message", s1->message );

	str = "my filename";
	reflector->setField( s1, filenameField, a1 );
	EXPECT_EQ( "my filename", s1->filename );

	// --- field getting:
	co::Any res;

	reflector->getField( s1, lineField, a1 );
	reflector->getField( s2, lineField, res );
	EXPECT_EQ( 7, a1.get<co::int32>() );
	EXPECT_EQ( 0, res.get<co::int32>() );

	reflector->getField( s1, messageField, a1 );
	reflector->getField( s2, messageField, res );
	EXPECT_EQ( "my message", a1.get<const std::string&>() );
	EXPECT_EQ( "", res.get<const std::string&>() );

	// --- in-place destruction:
	reflector->destroyValue( s1 );
	reflector->destroyValue( s2 );
}

TEST( ReflectorTests, structExceptions )
{
	co::IType* type = co::getType( "co.CSLError" );
	co::IReflector* reflector = type->getReflector();
	ASSERT_TRUE( reflector != NULL );

	co::uint8 memoryArea[sizeof(co::CSLError)];
	co::CSLError* s1 = reinterpret_cast<co::CSLError*>( memoryArea );

	co::Any a1;
	co::IField* lineField = getField( type, "line" );

	EXPECT_THROW( reflector->getField( type, lineField, a1 ), co::IllegalArgumentException );
	EXPECT_THROW( reflector->getField( s1, NULL, a1 ), co::IllegalArgumentException );
}

TEST( ReflectorTests, interfaceNamespace )
{
	co::IType* type = co::getType( "co.INamespace" );
	co::IReflector* reflector = type->getReflector();
	ASSERT_TRUE( reflector != NULL );

	// --- sanity checks:
	EXPECT_EQ( type, reflector->getType() );
	EXPECT_EQ( CORAL_POINTER_SIZE, reflector->getSize() );

	// --- obtain the necessary member infos:

	// readonly string name;
	co::IField* nameField = getField( type, "name" );
	ASSERT_TRUE( nameField != NULL );

	// readonly IType[] types;
	co::IField* typesField = getField( type, "types" );
	ASSERT_TRUE( typesField != NULL );

	// IType getType( in string name );
	co::IMethod* getTypeMethod = getMethod( type, "getType" );
	ASSERT_TRUE( getTypeMethod != NULL );

	// ITypeBuilder defineType( in string name, in TypeKind typeKind, in ITypeTransaction transaction )
	co::IMethod* defineTypeMethod = getMethod( type, "defineType" );
	ASSERT_TRUE( defineTypeMethod != NULL );

	// --- obtain a 'co.INamespace' instance:
	co::INamespace* coNS = co::getSystem()->getTypes()->findNamespace( "co" );

	// --- field getting:
	co::Any a1;

	reflector->getField( coNS, nameField, a1 );
	EXPECT_EQ( "co", a1.get<const std::string&>() );

	// cannot 'set' a read-only field
	EXPECT_THROW( reflector->setField( coNS, nameField, a1 ), co::IllegalArgumentException );

	reflector->getField( coNS, typesField, a1 );
	EXPECT_TRUE( a1.get< co::Range<co::IType* const> >().getSize() > 10 );
	EXPECT_EQ( "co.ArrayType", a1.get< co::Range<co::IType* const> >().getFirst()->getFullName() );

	// --- calling method getType():
	co::Any res;

	std::string str( "INamespace" );
	a1.set<std::string&>( str );

	reflector->invoke( coNS, getTypeMethod, co::Range<co::Any const>( &a1, 1 ), res );
	EXPECT_EQ( type, res.get<co::IType*>() );

	// calling getType() with no argument should generate an exception
	EXPECT_THROW( reflector->invoke( coNS, getTypeMethod, co::Range<co::Any const>(), res ),
					co::MissingInputException );

	try
	{
		// try to call getType() passing a bool instead of a string
		a1.set<bool>( false );
		reflector->invoke( coNS, getTypeMethod, co::Range<co::Any const>( &a1, 1 ), res );
		EXPECT_FALSE( true );
	}
	catch( co::IllegalCastException& e )
	{
		EXPECT_EQ( "invalid argument #1 to method getType(): illegal cast from 'bool' to 'const std::string&'", e.getMessage() );
	}

	// --- calling method defineType():
	co::Any args[4];

	str = "MyTypeName";
	args[0].set<std::string&>( str );
	args[1].set( co::TK_STRUCT );

	// passing only 2 args, when the method requires 3
	EXPECT_THROW( reflector->invoke( coNS, defineTypeMethod, co::Range<co::Any const>( args, 2 ), res ),
				 co::MissingInputException );

	// passing 3 args, but the third one is a null co::Any
	try
	{
		reflector->invoke( coNS, defineTypeMethod, co::Range<co::Any const>( args, 3 ), res );
	}
	catch( co::IllegalCastException& e )
	{
		EXPECT_EQ( "invalid argument #3 to method defineType(): illegal cast from '<NONE>' to 'co::ITypeTransaction*'", e.getMessage() );
	}

	// ok, now we call the method properly, but with a 4th, unecessary argument (it should work)
	co::RefPtr<co::ITypeTransaction> tct = createTypeTransaction();
	args[2].set( tct.get() );
	args[3].set( "dummy arg" );

	reflector->invoke( coNS, defineTypeMethod, co::Range<co::Any const>( args, 4 ), res );

	// alright, we should be able to retrieve a ITypeBuilder from res
	co::RefPtr<co::ITypeBuilder> builder = res.get<co::ITypeBuilder*>();
	EXPECT_TRUE( builder.isValid() );

	// cleanup
	tct->rollback();
}

TEST( ReflectorTests, nativeClass )
{
	co::IType* type = co::getType( "co.Uuid" );
	co::IReflector* reflector = type->getReflector();
	ASSERT_TRUE( reflector != NULL );

	// --- sanity checks:
	EXPECT_EQ( type, reflector->getType() );
	EXPECT_EQ( sizeof(co::Uuid), reflector->getSize() );

	// --- in-place allocation:
	co::Uuid u1;
	co::uint8 memoryArea[sizeof(co::Uuid)] = { 0xAB, 0xCD, 0xEF };

	co::Uuid* u2 = reinterpret_cast<co::Uuid*>( memoryArea );

	reflector->createValue( u2 );

	EXPECT_EQ( u1, *u2 );

	// --- obtain the necessary member infos:
	co::IField* isNullField = getField( type, "isNull" );
	ASSERT_TRUE( isNullField != NULL );

	co::IMethod* createRandomMethod = getMethod( type, "createRandom" );
	ASSERT_TRUE( createRandomMethod != NULL );

	co::IMethod* clearMethod = getMethod( type, "clear" );
	ASSERT_TRUE( clearMethod != NULL );

	co::IMethod* getStringMethod = getMethod( type, "getString" );
	ASSERT_TRUE( getStringMethod != NULL );

	co::IMethod* setStringMethod = getMethod( type, "setString" );
	ASSERT_TRUE( setStringMethod != NULL );

	// --- both freshly constructed Uuids should be null; check this using 'isNull' and 'getString'
	co::Any a1, res;

	reflector->getField( &u1, isNullField, a1 );
	EXPECT_TRUE( a1.get<bool>() );

	std::string str;
	a1.set<std::string&>( str );
	reflector->invoke( u2, getStringMethod, co::Range<co::Any const>( &a1, 1 ), res );
	EXPECT_EQ( "00000000-0000-0000-0000000000000000", str );

	// --- randomize u1, then copy its value to u2 via get/setString
	reflector->invoke( &u1, createRandomMethod, co::Range<co::Any const>(), res );
	reflector->getField( &u1, isNullField, a1 );
	EXPECT_FALSE( a1.get<bool>() );

	EXPECT_NE( u1, *u2 );

	a1.set<std::string&>( str );
	reflector->invoke( &u1, getStringMethod, co::Range<co::Any const>( &a1, 1 ), res );
	reflector->invoke( u2, setStringMethod, co::Range<co::Any const>( &a1, 1 ), res );

	EXPECT_EQ( u1, *u2 );

	// --- clear u1 and it should be null again
	reflector->invoke( &u1, clearMethod, co::Range<co::Any const>(), res );
	reflector->getField( &u1, isNullField, a1 );
	EXPECT_TRUE( a1.get<bool>() );
	EXPECT_NE( u1, *u2 );

	// --- set u2 with an invalid string and it should become null as well
	str = "{invalid}";
	a1.set<std::string&>( str );
	reflector->invoke( u2, setStringMethod, co::Range<co::Any const>( &a1, 1 ), res );
	EXPECT_EQ( u1, *u2 );

	// --- in-place destruction:
	reflector->destroyValue( u2 );
}
