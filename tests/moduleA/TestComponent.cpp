/*
 * Coral - A C++ Component Framework.
 * See Copyright Notice in Coral.h
 */

#include "TestComponent.h"
#include <moduleA/TestStruct.h>
#include <moduleA/DummyInterface.h>
#include <cassert>

TestComponent::TestComponent() : _name( "NOT SET" ), _readOnlyString( "READONLY" )
{
	_testEnum = moduleA::First;
}

TestComponent::~TestComponent()
{
	// empty
}

moduleA::DummyInterface* TestComponent::getDummyInterfaceAttribute()
{
	return _dummyInterface.get();
}

void TestComponent::setDummyInterfaceAttribute( moduleA::DummyInterface* dummyInterfaceAttribute )
{
	_dummyInterface = dummyInterfaceAttribute;
}

co::ArrayRange<moduleA::DummyInterface* const> TestComponent::getDummyInterfaces()
{
	return _dummyInterfaces;
}
void TestComponent::setDummyInterfaces( co::ArrayRange<moduleA::DummyInterface* const> dummyInterfaces )
{
	_dummyInterfaces.clear();
	for( ; dummyInterfaces; dummyInterfaces.popFirst() )
		_dummyInterfaces.push_back( dummyInterfaces.getFirst() );
}

const std::string& TestComponent::getName()
{
	return _name;
}

void TestComponent::setName( const std::string& name )
{
	_name = name;
}

const std::string& TestComponent::getNameReadonly()
{
	return _readOnlyString;
}

co::ArrayRange<std::string const> TestComponent::getNames()
{
	return co::ArrayRange<std::string const>( _names );
}

void TestComponent::setNames( co::ArrayRange<std::string const> names )
{
	_names.clear();

	for( ; names; names.popFirst() )
		_names.push_back( names.getFirst() );
}

moduleA::TestInterface* TestComponent::getSelfReferenceAttrib()
{
	return this;
}

moduleA::TestEnum TestComponent::getTestEnum()
{
	return _testEnum;
}

void TestComponent::setTestEnum( moduleA::TestEnum value )
{
	_testEnum = value;
}

const moduleA::TestStruct& TestComponent::getTestStruct()
{
	return _testStruct;
}

void TestComponent::setTestStruct( const moduleA::TestStruct& value )
{
	_testStruct = value;
}

co::ArrayRange<moduleA::TestStruct const> TestComponent::getTestStructArray()
{
	return _testStructArray;
}

void TestComponent::setTestStructArray( co::ArrayRange<moduleA::TestStruct const> range )
{
	_testStructArray.clear();
	_testStructArray.reserve( range.getSize() );
	for( ; range; range.popFirst() )
		_testStructArray.push_back( range.getFirst() );
}

void TestComponent::testInParameters( float size, moduleA::TestEnum enumValue,
		const std::string& text, const moduleA::TestStruct& testStruct,
		moduleA::DummyInterface* dummyInterface, co::ArrayRange<co::int32 const> intList,
		co::ArrayRange<moduleA::DummyInterface* const> interfaces )
{
	size = -1;
	enumValue = moduleA::First;
	assert( text != "" );

	assert( testStruct.anInt16 > 0 );

	dummyInterface = NULL;

	assert( !intList.isEmpty() );

	co::int32 myFirstInt = intList.getFirst();
	assert( myFirstInt > 0 );
	myFirstInt = -1;

	assert( !interfaces.isEmpty() );
	moduleA::DummyInterface* firstInterface = interfaces.getFirst();
	firstInterface = NULL;

	// suppress warnings in release mode
	CORAL_UNUSED( text );
	CORAL_UNUSED( testStruct );
	CORAL_UNUSED( interfaces );
}

void TestComponent::testOutParameters( float& size, moduleA::TestEnum& enumValue,
		std::string& text, moduleA::TestStruct& testStruct,
		moduleA::DummyInterface*& dummyInterface, std::vector<co::int32>* intList,
		co::RefVector<moduleA::DummyInterface>* interfaces )
{
	size = -1;
	enumValue = moduleA::First;
	assert( text != "" );

	assert( testStruct.anInt16 > 0 );
	testStruct.anInt16 = -1;
	text = "";

	dummyInterface = NULL;
	
	assert( intList != NULL );
	assert( !intList->empty() );

	co::int32 myFirstInt = intList->at( 0 );
	assert( myFirstInt > 0 );
	myFirstInt = -1;
	
	assert( interfaces != NULL );
	assert( !interfaces->empty() );
	CORAL_UNUSED( interfaces );
}

co::ArrayRange<co::TypeBuilder* const> TestComponent::getTypeBuilders()
{
	return co::ArrayRange<co::TypeBuilder* const>();
}

void TestComponent::commit()
{
	// empty
}

void TestComponent::rollback()
{
	// empty
}

co::InterfaceType* TestComponent::getRequiredItfType()
{
	return _itfType.get();
}

void TestComponent::setRequiredItfType( co::InterfaceType* itfType )
{
	_itfType = itfType;
}

co::Type* TestComponent::getRequiredType()
{
	return _type.get();
}

void TestComponent::setRequiredType( co::Type* type )
{
	_type = type;
}

CORAL_EXPORT_COMPONENT( TestComponent, TestComponent );
