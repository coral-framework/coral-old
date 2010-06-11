/*
 * Coral - A C++ Component Framework.
 * See Copyright Notice in Coral.h
 */

#ifndef _TESTCOMPONENT_H_
#define _TESTCOMPONENT_H_

#include "TestComponent_Base.h"

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

	virtual void testInParameters( float size, moduleA::TestEnum enumValue,
		const std::string& text, const moduleA::TestStruct& testStruct,
		moduleA::DummyInterface* dummyInterface, co::ArrayRange<co::int32 const> intList,
		co::ArrayRange<moduleA::DummyInterface* const> interfaces );

	virtual void testOutParameters( float& size, moduleA::TestEnum& enumValue,
		std::string& text, moduleA::TestStruct& testStruct,
		moduleA::DummyInterface*& dummyInterface, std::vector<co::int32>* intList,
		co::RefVector<moduleA::DummyInterface>* interfaces );

	// co::TypeCreationTransaction methods:
	co::ArrayRange<co::TypeBuilder* const> getTypeBuilders();
	void commit();
	void rollback();

private:
	co::RefPtr<moduleA::DummyInterface> _dummyInterface;
	std::string _name;
	std::string _readOnlyString;

	std::vector<std::string> _names;
	co::RefVector<moduleA::DummyInterface> _dummyInterfaces;
};

#endif
