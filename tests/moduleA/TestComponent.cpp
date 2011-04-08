/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#include "TestComponent_Base.h"
#include <moduleA/TestStruct.h>
#include <moduleA/IDummy.h>
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

	IDummy* getDummyInterfaceField()
	{
		return _dummyInterface.get();
	}

	void setDummyInterfaceField( IDummy* dummyInterfaceField )
	{
		_dummyInterface = dummyInterfaceField;
	}

	co::Range<IDummy* const> getDummyInterfaces()
	{
		return _dummyInterfaces;
	}

	void setDummyInterfaces( co::Range<IDummy* const> dummyInterfaces )
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

	co::Range<std::string const> getNames()
	{
		return co::Range<std::string const>( _names );
	}

	void setNames( co::Range<std::string const> names )
	{
		_names.clear();
		for( ; names; names.popFirst() )
			_names.push_back( names.getFirst() );
	}

	TestInterface* getSelfReferenceField()
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

	co::Range<TestStruct const> getTestStructArray()
	{
		return _testStructArray;
	}

	void setTestStructArray( co::Range<TestStruct const> range )
	{
		_testStructArray.clear();
		_testStructArray.reserve( range.getSize() );
		for( ; range; range.popFirst() )
			_testStructArray.push_back( range.getFirst() );
	}

	void testInParameters( float size, TestEnum enumValue,
		const std::string& text, const TestStruct& testStruct,
		IDummy* dummyInterface, co::Range<co::int32 const> intList,
		co::Range<IDummy* const> interfaces )
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
		IDummy* firstInterface = interfaces.getFirst();
		firstInterface = NULL;

		// suppress warnings in release mode
		CORAL_UNUSED( text );
		CORAL_UNUSED( testStruct );
		CORAL_UNUSED( interfaces );
	}

	void testOutParameters( float& size, TestEnum& enumValue,
		std::string& text, TestStruct& testStruct,
		IDummy*& dummyInterface, std::vector<co::int32>& intList,
		co::RefVector<IDummy>& interfaces )
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

	// co::ITypeTransaction methods:

	co::Range<co::ITypeBuilder* const> getTypeBuilders()
	{
		return co::Range<co::ITypeBuilder* const>();
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
	co::IInterface* getItfService()
	{
		return _itf.get();
	}

	void setItfService( co::IInterface* itf )
	{
		_itf = itf;
	}

	co::IType* getTypeService()
	{
		return _type.get();
	}

	void setTypeService( co::IType* type )
	{
		_type = type;
	}

private:
	co::RefPtr<co::IType> _type;
	co::RefPtr<co::IInterface> _itf;

	co::RefPtr<IDummy> _dummyInterface;
	std::string _name;
	std::string _readOnlyString;

	std::vector<std::string> _names;
	co::RefVector<IDummy> _dummyInterfaces;
	TestEnum _testEnum;
	TestStruct _testStruct;
	std::vector<TestStruct> _testStructArray;
};

CORAL_EXPORT_COMPONENT( TestComponent, TestComponent );

} // namespace co
