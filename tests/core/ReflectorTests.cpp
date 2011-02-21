/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#include "TestHelper.h"
#include <co/Uuid.h>
#include <co/Coral.h>
#include <co/System.h>
#include <co/CSLError.h>
#include <co/Namespace.h>
#include <co/Reflector.h>
#include <co/MethodInfo.h>
#include <co/TypeBuilder.h>
#include <co/TypeManager.h>
#include <co/CompoundType.h>
#include <co/AttributeInfo.h>
#include <co/InterfaceInfo.h>
#include <co/IllegalCastException.h>
#include <co/MissingInputException.h>
#include <co/TypeCreationTransaction.h>
#include <co/IllegalArgumentException.h>
#include <co/NotSupportedException.h>
#include <gtest/gtest.h>

TEST( ReflectorTests, basicTypes )
{
	static const char* BASIC_TYPE_NAMES[] = {
		"any", "bool", "int8", "uint8", "int16", "uint16", "int32", "uint32",
		"int64", "uint64", "float", "double", "string",
		"co.Type[]",	// an array
		"co.TypeKind",	// an enum
		"co.IllegalNameException" // an exception
	};
	
	const int NUM_TYPES = CORAL_ARRAY_LENGTH( BASIC_TYPE_NAMES );
	
	co::Type* types[NUM_TYPES];
	for( int i = 0; i < NUM_TYPES; ++i )
	{
		types[i] = co::getType( BASIC_TYPE_NAMES[i] );
		ASSERT_TRUE( types[i] != NULL );
	}

	// dummy co::Any instance
	co::Any any;
	co::ArrayRange<co::Any const> anyRange;

	// make sure all types have a BasicReflector
	for( int i = 0; i < NUM_TYPES; ++i )
	{
		co::Reflector* reflector = types[i]->getReflector();
		ASSERT_TRUE( reflector != NULL );

		EXPECT_EQ( types[i], reflector->getType() );
		EXPECT_GT( reflector->getSize(), 0 );

		EXPECT_THROW( reflector->createValue( NULL, 0 ), co::NotSupportedException );
		EXPECT_THROW( reflector->destroyValue( NULL ), co::NotSupportedException );
		EXPECT_THROW( reflector->newInstance(), co::NotSupportedException );
		EXPECT_THROW( reflector->newProxy( NULL ), co::NotSupportedException );
		EXPECT_THROW( reflector->getAttribute( any, NULL, any ), co::NotSupportedException );
		EXPECT_THROW( reflector->setAttribute( any, NULL, any ), co::NotSupportedException );
		EXPECT_THROW( reflector->invokeMethod( any, NULL, anyRange, any ), co::NotSupportedException );
	}
}

TEST( ReflectorTests, reflectorComponent )
{
	co::Type* type = co::getType( "co.Namespace" );
	co::Reflector* reflector = type->getReflector();
	co::Component* reflectorComponent = reflector->getInterfaceOwner();

	// test the reflector component's ComponentType
	co::ComponentType* componentType = reflectorComponent->getComponentType();
	EXPECT_EQ( 1, componentType->getInterfaces().getSize() );
	EXPECT_EQ( "reflector", componentType->getInterfaces().getFirst()->getName() );

	// tests the reflector component's interface
	EXPECT_EQ( reflector, reflectorComponent->getInterface( componentType->getInterfaces().getFirst() ) );
}

TEST( ReflectorTests, exceptions )
{
	// test raised exception types
	ASSERT_THROW( co::getType( "co.Namespace" )->getReflector()->raise( "msg" ), co::NotSupportedException );
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
T* getMember( co::Type* type, const char* memberName )
{
	co::CompoundType* ct = dynamic_cast<co::CompoundType*>( type );
	assert( ct );

	T* mi = dynamic_cast<T*>( ct->getMember( memberName ) );
	assert( mi );

	return mi;
}

co::AttributeInfo* getAttributeInfo( co::Type* type, const char* memberName )
{
	return getMember<co::AttributeInfo>( type, memberName );
}

co::MethodInfo* getMethodInfo( co::Type* type, const char* memberName )
{
	return getMember<co::MethodInfo>( type, memberName );
}

TEST( ReflectorTests, structSimple )
{
	co::Type* type = co::getType( "co.CSLError" );
	co::Reflector* reflector = type->getReflector();
	ASSERT_TRUE( reflector != NULL );

	// --- sanity checks:
	EXPECT_EQ( type, reflector->getType() );
	EXPECT_EQ( sizeof(co::CSLError), reflector->getSize() );

	// --- in-place allocation:
	co::uint8 memoryArea[sizeof(co::CSLError) * 2];

	co::CSLError* s1 = reinterpret_cast<co::CSLError*>( memoryArea );
	co::CSLError* s2 = reinterpret_cast<co::CSLError*>( memoryArea + sizeof(co::CSLError) );

	reflector->createValue( s1, sizeof(co::CSLError) );
	reflector->createValue( s2, sizeof(co::CSLError) );

	EXPECT_EQ( 0, s1->line );
	EXPECT_EQ( 0, s1->message.length() );
	EXPECT_EQ( 0, s1->filename.length() );

	EXPECT_EQ( 0, s2->line );
	EXPECT_EQ( 0, s2->message.length() );
	EXPECT_EQ( 0, s2->filename.length() );

	// --- obtain the necessary attribute infos:
	co::AttributeInfo* lineAttrib = getAttributeInfo( type, "line" );
	ASSERT_TRUE( lineAttrib != NULL );

	co::AttributeInfo* messageAttrib = getAttributeInfo( type, "message" );
	ASSERT_TRUE( messageAttrib != NULL );

	co::AttributeInfo* filenameAttrib = getAttributeInfo( type, "filename" );
	ASSERT_TRUE( filenameAttrib != NULL );

	// --- attribute setting:
	reflector->setAttribute( s1, lineAttrib, 7 );
	EXPECT_EQ( 7, s1->line );

	try
	{
		// try to set a string attribute with an int value (there's no coercion)
		reflector->setAttribute( s1, messageAttrib, 3 );
		EXPECT_FALSE( true );
	}
	catch( co::IllegalCastException& e )
	{
		EXPECT_EQ( "illegal cast from 'co::int32' to 'const std::string&'", e.getMessage() );
	}

	co::Any a1;
	std::string str( "my message" );
	a1.set<const std::string&>( str );

	reflector->setAttribute( s1, messageAttrib, a1 );
	EXPECT_EQ( "my message", s1->message );

	str = "my filename";
	reflector->setAttribute( s1, filenameAttrib, a1 );
	EXPECT_EQ( "my filename", s1->filename );

	// --- attribute getting:
	co::Any res;

	reflector->getAttribute( s1, lineAttrib, a1 );
	reflector->getAttribute( s2, lineAttrib, res );
	EXPECT_EQ( 7, a1.get<co::int32>() );
	EXPECT_EQ( 0, res.get<co::int32>() );

	reflector->getAttribute( s1, messageAttrib, a1 );
	reflector->getAttribute( s2, messageAttrib, res );
	EXPECT_EQ( "my message", a1.get<const std::string&>() );
	EXPECT_EQ( "", res.get<const std::string&>() );

	// --- in-place destruction:
	reflector->destroyValue( s1 );
	reflector->destroyValue( s2 );
}

TEST( ReflectorTests, structExceptions )
{
	co::Type* type = co::getType( "co.CSLError" );
	co::Reflector* reflector = type->getReflector();
	ASSERT_TRUE( reflector != NULL );

	co::uint8 memoryArea[sizeof(co::CSLError)];
	co::CSLError* s1 = reinterpret_cast<co::CSLError*>( memoryArea );

	EXPECT_THROW( reflector->createValue( s1, 1337 ), co::IllegalArgumentException );
	
	co::Any a1;
	co::AttributeInfo* lineAttrib = getAttributeInfo( type, "line" );

	EXPECT_THROW( reflector->getAttribute( type, lineAttrib, a1 ), co::IllegalArgumentException );
	EXPECT_THROW( reflector->getAttribute( s1, NULL, a1 ), co::IllegalArgumentException );
}

TEST( ReflectorTests, interfaceNamespace )
{
	co::Type* type = co::getType( "co.Namespace" );
	co::Reflector* reflector = type->getReflector();
	ASSERT_TRUE( reflector != NULL );

	// --- sanity checks:
	EXPECT_EQ( type, reflector->getType() );
	EXPECT_EQ( CORAL_POINTER_SIZE, reflector->getSize() );

	// --- obtain the necessary member infos:

	// readonly attribute string name;
	co::AttributeInfo* nameAttrib = getAttributeInfo( type, "name" );
	ASSERT_TRUE( nameAttrib != NULL );

	// readonly attribute Type[] types;
	co::AttributeInfo* typesAttrib = getAttributeInfo( type, "types" );
	ASSERT_TRUE( typesAttrib != NULL );

	// Type getType( in string name );
	co::MethodInfo* getTypeMethod = getMethodInfo( type, "getType" );
	ASSERT_TRUE( getTypeMethod != NULL );

	// TypeBuilder defineType( in string name, in TypeKind typeKind, in TypeCreationTransaction transaction )
	co::MethodInfo* defineTypeMethod = getMethodInfo( type, "defineType" );
	ASSERT_TRUE( defineTypeMethod != NULL );

	// --- obtain a 'co.Namespace' instance:
	co::Namespace* coNS = co::getSystem()->getTypes()->findNamespace( "co" );

	// --- attribute getting:
	co::Any a1;

	reflector->getAttribute( coNS, nameAttrib, a1 );
	EXPECT_EQ( "co", a1.get<const std::string&>() );

	// cannot 'set' a read-only attribute
	EXPECT_THROW( reflector->setAttribute( coNS, nameAttrib, a1 ), co::IllegalArgumentException );

	reflector->getAttribute( coNS, typesAttrib, a1 );
	EXPECT_TRUE( a1.get< co::ArrayRange<co::Type* const> >().getSize() > 10 );
	EXPECT_EQ( "co.ArrayType", a1.get< co::ArrayRange<co::Type* const> >().getFirst()->getFullName() );

	// --- calling method getType():
	co::Any res;

	std::string str( "Namespace" );
	a1.set<std::string&>( str );

	reflector->invokeMethod( coNS, getTypeMethod, co::ArrayRange<co::Any const>( &a1, 1 ), res );
	EXPECT_EQ( type, res.get<co::Type*>() );

	// calling getType() with no argument should generate an exception
	EXPECT_THROW( reflector->invokeMethod( coNS, getTypeMethod, co::ArrayRange<co::Any const>(), res ),
					co::MissingInputException );

	try
	{
		// try to call getType() passing a bool instead of a string
		a1.set<bool>( false );
		reflector->invokeMethod( coNS, getTypeMethod, co::ArrayRange<co::Any const>( &a1, 1 ), res );
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
	EXPECT_THROW( reflector->invokeMethod( coNS, defineTypeMethod, co::ArrayRange<co::Any const>( args, 2 ), res ),
				 co::MissingInputException );

	// passing 3 args, but the third one is a null co::Any
	try
	{
		reflector->invokeMethod( coNS, defineTypeMethod, co::ArrayRange<co::Any const>( args, 3 ), res );
	}
	catch( co::IllegalCastException& e )
	{
		EXPECT_EQ( "invalid argument #3 to method defineType(): illegal cast from '<NONE>' to 'co::TypeCreationTransaction*'", e.getMessage() );
	}

	// ok, now we call the method properly, but with a 4th, unecessary argument (it should work)
	co::RefPtr<co::TypeCreationTransaction> tct = createTypeCreationTransaction();
	args[2].set( tct.get() );
	args[3].set( "dummy arg" );

	reflector->invokeMethod( coNS, defineTypeMethod, co::ArrayRange<co::Any const>( args, 4 ), res );

	// alright, we should be able to retrieve a TypeBuilder from res
	co::RefPtr<co::TypeBuilder> builder = res.get<co::TypeBuilder*>();
	EXPECT_TRUE( builder.isValid() );

	// cleanup
	tct->rollback();
}

TEST( ReflectorTests, nativeClass )
{
	co::Type* type = co::getType( "co.Uuid" );
	co::Reflector* reflector = type->getReflector();
	ASSERT_TRUE( reflector != NULL );

	// --- sanity checks:
	EXPECT_EQ( type, reflector->getType() );
	EXPECT_EQ( sizeof(co::Uuid), reflector->getSize() );

	// --- in-place allocation:
	co::Uuid u1;
	co::uint8 memoryArea[sizeof(co::Uuid)] = { 0xAB, 0xCD, 0xEF };

	co::Uuid* u2 = reinterpret_cast<co::Uuid*>( memoryArea );

	reflector->createValue( u2, sizeof(co::Uuid) );

	EXPECT_EQ( u1, *u2 );

	// --- obtain the necessary member infos:
	co::AttributeInfo* isNullAttrib = getAttributeInfo( type, "isNull" );
	ASSERT_TRUE( isNullAttrib != NULL );

	co::MethodInfo* createRandomMethod = getMethodInfo( type, "createRandom" );
	ASSERT_TRUE( createRandomMethod != NULL );

	co::MethodInfo* clearMethod = getMethodInfo( type, "clear" );
	ASSERT_TRUE( clearMethod != NULL );

	co::MethodInfo* getStringMethod = getMethodInfo( type, "getString" );
	ASSERT_TRUE( getStringMethod != NULL );

	co::MethodInfo* setStringMethod = getMethodInfo( type, "setString" );
	ASSERT_TRUE( setStringMethod != NULL );

	// --- both freshly constructed Uuids should be null; check this using 'isNull' and 'getString'
	co::Any a1, res;

	reflector->getAttribute( &u1, isNullAttrib, a1 );
	EXPECT_TRUE( a1.get<bool>() );

	std::string str;
	a1.set<std::string&>( str );
	reflector->invokeMethod( u2, getStringMethod, co::ArrayRange<co::Any const>( &a1, 1 ), res );
	EXPECT_EQ( "00000000-0000-0000-0000000000000000", str );

	// --- randomize u1, then copy its value to u2 via get/setString
	reflector->invokeMethod( &u1, createRandomMethod, co::ArrayRange<co::Any const>(), res );
	reflector->getAttribute( &u1, isNullAttrib, a1 );
	EXPECT_FALSE( a1.get<bool>() );

	EXPECT_NE( u1, *u2 );

	a1.set<std::string&>( str );
	reflector->invokeMethod( &u1, getStringMethod, co::ArrayRange<co::Any const>( &a1, 1 ), res );
	reflector->invokeMethod( u2, setStringMethod, co::ArrayRange<co::Any const>( &a1, 1 ), res );

	EXPECT_EQ( u1, *u2 );

	// --- clear u1 and it should be null again
	reflector->invokeMethod( &u1, clearMethod, co::ArrayRange<co::Any const>(), res );
	reflector->getAttribute( &u1, isNullAttrib, a1 );
	EXPECT_TRUE( a1.get<bool>() );
	EXPECT_NE( u1, *u2 );
	
	// --- set u2 with an invalid string and it should become null as well
	str = "{invalid}";
	a1.set<std::string&>( str );
	reflector->invokeMethod( u2, setStringMethod, co::ArrayRange<co::Any const>( &a1, 1 ), res );
	EXPECT_EQ( u1, *u2 );

	// --- in-place destruction:
	reflector->destroyValue( u2 );
}
