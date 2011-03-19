/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#include "TestComponent_Base.h"
#include <moduleA/TestStruct.h>
#include <moduleA/DummyInterface.h>
#include <cassert>

namespace moduleA {

class TestComponent : public TestComponent_Base
{
public:
	TestComponent() : _name( "NOT SET" ), _readOnlyString( "READONLY" )
	{
		_testEnum = First;
	}

	virtual ~TestComponent()
	{
		// empty
	}

	// TestInterface: methods:

	DummyInterface* getDummyInterfaceAttribute()
	{
		return _dummyInterface.get();
	}

	void setDummyInterfaceAttribute( DummyInterface* dummyInterfaceAttribute )
	{
		_dummyInterface = dummyInterfaceAttribute;
	}

	co::ArrayRange<DummyInterface* const> getDummyInterfaces()
	{
		return _dummyInterfaces;
	}

	void setDummyInterfaces( co::ArrayRange<DummyInterface* const> dummyInterfaces )
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

	TestInterface* getSelfReferenceAttrib()
	{
		return this;
	}

	TestEnum getTestEnum()
	{
		return _testEnum;
	}

	void setTestEnum( TestEnum value )
	{
		_testEnum = value;
	}

	const TestStruct& getTestStruct()
	{
		return _testStruct;
	}

	void setTestStruct( const TestStruct& value )
	{
		_testStruct = value;
	}

	co::ArrayRange<TestStruct const> getTestStructArray()
	{
		return _testStructArray;
	}

	void setTestStructArray( co::ArrayRange<TestStruct const> range )
	{
		_testStructArray.clear();
		_testStructArray.reserve( range.getSize() );
		for( ; range; range.popFirst() )
			_testStructArray.push_back( range.getFirst() );
	}

	void testInParameters( float size, TestEnum enumValue,
		const std::string& text, const TestStruct& testStruct,
		DummyInterface* dummyInterface, co::ArrayRange<co::int32 const> intList,
		co::ArrayRange<DummyInterface* const> interfaces )
	{
		size = -1;
		enumValue = First;
		assert( text != "" );

		assert( testStruct.anInt16 > 0 );

		dummyInterface = NULL;

		assert( !intList.isEmpty() );

		co::int32 myFirstInt = intList.getFirst();
		assert( myFirstInt > 0 );
		myFirstInt = -1;

		assert( !interfaces.isEmpty() );
		DummyInterface* firstInterface = interfaces.getFirst();
		firstInterface = NULL;

		// suppress warnings in release mode
		CORAL_UNUSED( text );
		CORAL_UNUSED( testStruct );
		CORAL_UNUSED( interfaces );
	}

	void testOutParameters( float& size, TestEnum& enumValue,
		std::string& text, TestStruct& testStruct,
		DummyInterface*& dummyInterface, std::vector<co::int32>& intList,
		co::RefVector<DummyInterface>& interfaces )
	{
		size = -1;
		enumValue = First;
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

	// co::ITypeCreationTransaction methods:

	co::ArrayRange<co::ITypeBuilder* const> getTypeBuilders()
	{
		return co::ArrayRange<co::ITypeBuilder* const>();
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
	co::IInterfaceType* getReceptacleItfType()
	{
		return _itfType.get();
	}

	void setReceptacleItfType( co::IInterfaceType* itfType )
	{
		_itfType = itfType;
	}

	co::IType* getReceptacleType()
	{
		return _type.get();
	}

	void setReceptacleType( co::IType* type )
	{
		_type = type;
	}

private:
	co::RefPtr<co::IInterfaceType> _itfType;
	co::RefPtr<co::IType> _type;

	co::RefPtr<DummyInterface> _dummyInterface;
	std::string _name;
	std::string _readOnlyString;

	std::vector<std::string> _names;
	co::RefVector<DummyInterface> _dummyInterfaces;
	TestEnum _testEnum;
	TestStruct _testStruct;
	std::vector<TestStruct> _testStructArray;
};

CORAL_EXPORT_COMPONENT( TestComponent, TestComponent );

} // namespace co
