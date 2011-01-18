/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#include "TestComponent_Base.h"
#include <moduleA/TestStruct.h>
#include <moduleA/DummyInterface.h>
#include <cassert>

class TestComponent : public moduleA::TestComponent_Base
{
public:
	TestComponent() : _name( "NOT SET" ), _readOnlyString( "READONLY" )
	{
		_testEnum = moduleA::First;
	}

	virtual ~TestComponent()
	{
		// empty
	}

	// moduleA::TestInterface: methods:

	moduleA::DummyInterface* getDummyInterfaceAttribute()
	{
		return _dummyInterface.get();
	}

	void setDummyInterfaceAttribute( moduleA::DummyInterface* dummyInterfaceAttribute )
	{
		_dummyInterface = dummyInterfaceAttribute;
	}

	co::ArrayRange<moduleA::DummyInterface* const> getDummyInterfaces()
	{
		return _dummyInterfaces;
	}

	void setDummyInterfaces( co::ArrayRange<moduleA::DummyInterface* const> dummyInterfaces )
	{
		_dummyInterfaces.clear();
		for( ; dummyInterfaces; dummyInterfaces.popFirst() )
			_dummyInterfaces.push_back( dummyInterfaces.getFirst() );
	}

	const std::string& getName()
	{
		return _name;
	}

	void setName( const std::string& name )
	{
		_name = name;
	}

	const std::string& getNameReadonly()
	{
		return _readOnlyString;
	}

	co::ArrayRange<std::string const> getNames()
	{
		return co::ArrayRange<std::string const>( _names );
	}

	void setNames( co::ArrayRange<std::string const> names )
	{
		_names.clear();
		for( ; names; names.popFirst() )
			_names.push_back( names.getFirst() );
	}

	moduleA::TestInterface* getSelfReferenceAttrib()
	{
		return this;
	}

	moduleA::TestEnum getTestEnum()
	{
		return _testEnum;
	}

	void setTestEnum( moduleA::TestEnum value )
	{
		_testEnum = value;
	}

	const moduleA::TestStruct& getTestStruct()
	{
		return _testStruct;
	}

	void setTestStruct( const moduleA::TestStruct& value )
	{
		_testStruct = value;
	}

	co::ArrayRange<moduleA::TestStruct const> getTestStructArray()
	{
		return _testStructArray;
	}

	void setTestStructArray( co::ArrayRange<moduleA::TestStruct const> range )
	{
		_testStructArray.clear();
		_testStructArray.reserve( range.getSize() );
		for( ; range; range.popFirst() )
			_testStructArray.push_back( range.getFirst() );
	}

	void testInParameters( float size, moduleA::TestEnum enumValue,
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

	void testOutParameters( float& size, moduleA::TestEnum& enumValue,
		std::string& text, moduleA::TestStruct& testStruct,
		moduleA::DummyInterface*& dummyInterface, std::vector<co::int32>& intList,
		co::RefVector<moduleA::DummyInterface>& interfaces )
	{
		size = -1;
		enumValue = moduleA::First;
		assert( text != "" );

		assert( testStruct.anInt16 > 0 );
		testStruct.anInt16 = -1;
		text = "";

		dummyInterface = NULL;

		assert( !intList.empty() );

		co::int32 myFirstInt = intList[0];
		assert( myFirstInt > 0 );
		myFirstInt = -1;

		assert( !interfaces.empty() );
		CORAL_UNUSED( interfaces );
	}
	
	const co::Any& testAnyReturn( const co::Any& param )
	{
		return param;
	}

	// co::TypeCreationTransaction methods:

	co::ArrayRange<co::TypeBuilder* const> getTypeBuilders()
	{
		return co::ArrayRange<co::TypeBuilder* const>();
	}

	void commit()
	{
		// empty
	}
	
	void rollback()
	{
		// empty
	}	
	
protected:
	co::InterfaceType* getReceptacleItfType()
	{
		return _itfType.get();
	}

	void setReceptacleItfType( co::InterfaceType* itfType )
	{
		_itfType = itfType;
	}

	co::Type* getReceptacleType()
	{
		return _type.get();
	}

	void setReceptacleType( co::Type* type )
	{
		_type = type;
	}

private:
	co::RefPtr<co::InterfaceType> _itfType;
	co::RefPtr<co::Type> _type;

	co::RefPtr<moduleA::DummyInterface> _dummyInterface;
	std::string _name;
	std::string _readOnlyString;

	std::vector<std::string> _names;
	co::RefVector<moduleA::DummyInterface> _dummyInterfaces;
	moduleA::TestEnum _testEnum;
	moduleA::TestStruct _testStruct;
	std::vector<moduleA::TestStruct> _testStructArray;
};

CORAL_EXPORT_COMPONENT( TestComponent, TestComponent );
