/*
 * Coral - Lightweight C++ Component Framework.
 * See Copyright Notice in Coral.h
 */

#ifndef _TESTCOMPONENT_H_
#define _TESTCOMPONENT_H_

#include "TestComponent_Base.h"
#include <moduleA/TestStruct.h>

class TestComponent : public moduleA::TestComponent_Base
{
public:
    TestComponent();
	virtual ~TestComponent();

	// moduleA::TestInterface: methods:

	virtual moduleA::DummyInterface* getDummyInterfaceAttribute();
	virtual void setDummyInterfaceAttribute( moduleA::DummyInterface* dummyInterfaceAttribute );

	virtual co::ArrayRange<moduleA::DummyInterface* const> getDummyInterfaces();
	virtual void setDummyInterfaces( co::ArrayRange<moduleA::DummyInterface* const> dummyInterfaces );

	virtual const std::string& getName();
	virtual void setName( const std::string& name );

	virtual const std::string& getNameReadonly();

	virtual co::ArrayRange<std::string const> getNames();
	virtual void setNames( co::ArrayRange<std::string const> names );

	virtual moduleA::TestInterface* getSelfReferenceAttrib();

	virtual moduleA::TestEnum getTestEnum();
	virtual void setTestEnum( moduleA::TestEnum value );
	
	virtual const moduleA::TestStruct& getTestStruct();
	virtual void setTestStruct( const moduleA::TestStruct& value );
	
	virtual co::ArrayRange<moduleA::TestStruct const> getTestStructArray();
	virtual void setTestStructArray( co::ArrayRange<moduleA::TestStruct const> range );

	virtual void testInParameters( float size, moduleA::TestEnum enumValue,
		const std::string& text, const moduleA::TestStruct& testStruct,
		moduleA::DummyInterface* dummyInterface, co::ArrayRange<co::int32 const> intList,
		co::ArrayRange<moduleA::DummyInterface* const> interfaces );

	virtual void testOutParameters( float& size, moduleA::TestEnum& enumValue,
		std::string& text, moduleA::TestStruct& testStruct,
		moduleA::DummyInterface*& dummyInterface, std::vector<co::int32>& intList,
		co::RefVector<moduleA::DummyInterface>& interfaces );

	// co::TypeCreationTransaction methods:
	co::ArrayRange<co::TypeBuilder* const> getTypeBuilders();
	void commit();
	void rollback();
	
protected:
	co::InterfaceType* getReceptacleItfType();
	void setReceptacleItfType( co::InterfaceType* itfType );
	co::Type* getReceptacleType();
	void setReceptacleType( co::Type* type );

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

#endif
