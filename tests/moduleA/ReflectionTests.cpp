/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#include <gtest/gtest.h>

#include <co/Coral.h>
#include <co/IObject.h>
#include <co/IReflector.h>
#include <co/IMethod.h>
#include <co/ICompositeType.h>
#include <co/IField.h>
#include <co/IPort.h>
#include <co/IComponent.h>
#include <co/IllegalCastException.h>
#include <co/IllegalArgumentException.h>
#include <co/NoSuchPortException.h>
#include <moduleA/TestStruct.h>
#include <moduleA/TestInterface.h>

template<typename T>
T* getMember( co::IType* type, const char* memberName )
{
	co::ICompositeType* ct = dynamic_cast<co::ICompositeType*>( type );
	assert( ct );

	T* mi = dynamic_cast<T*>( ct->getMember( memberName ) );
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

TEST( ReflectionTests, structGetSetInterfacesAndArrays )
{
	co::IType* type = co::getType( "moduleA.TestStruct" );
	co::IReflector* reflector = type->getReflector();
	ASSERT_TRUE( reflector != NULL );

	// --- sanity checks:
	EXPECT_EQ( type, reflector->getType() );
	EXPECT_EQ( sizeof(moduleA::TestStruct), reflector->getSize() );

	// --- in-place allocation:
	co::uint8 memoryArea[sizeof(moduleA::TestStruct)];
	moduleA::TestStruct* ts = reinterpret_cast<moduleA::TestStruct*>( memoryArea );
	reflector->createValue( ts, sizeof(moduleA::TestStruct) );

	EXPECT_FALSE( ts->aType.isValid() );
	EXPECT_EQ( 0, ts->floatArray.size() );
	EXPECT_EQ( 0, ts->typeArray.size() );

	// --- obtain the necessary fields:
	co::IField* aTypeField = getField( type, "aType" );
	ASSERT_TRUE( aTypeField != NULL );

	co::IField* floatArrayField = getField( type, "floatArray" );
	ASSERT_TRUE( floatArrayField != NULL );

	co::IField* typeArrayField = getField( type, "typeArray" );
	ASSERT_TRUE( typeArrayField != NULL );

	// --- field setting:
	reflector->setField( ts, aTypeField, co::getType( "co.IArray" ) );
	ASSERT_TRUE( ts->aType.isValid() );
	EXPECT_EQ( "co.IArray", ts->aType->getFullName() );

	std::vector<co::IType*> typeVec;
	typeVec.push_back( co::getType( "co.IMember" ) );
	typeVec.push_back( co::getType( "co.IField" ) );
	typeVec.push_back( co::getType( "co.IMethod" ) );

	co::Any typeVecAny;
	typeVecAny.set<std::vector<co::IType*>&>( typeVec );

	EXPECT_THROW( reflector->setField( ts, floatArrayField, typeVecAny ), co::IllegalCastException );
	EXPECT_NO_THROW( reflector->setField( ts, typeArrayField, typeVecAny ) );

	ASSERT_EQ( 3, ts->typeArray.size() );
	EXPECT_EQ( "co.IMember", ts->typeArray[0]->getFullName() );
	EXPECT_EQ( "co.IField", ts->typeArray[1]->getFullName() );
	EXPECT_EQ( "co.IMethod", ts->typeArray[2]->getFullName() );

	// --- field getting:
	co::Any res;

	reflector->getField( ts, aTypeField, res );
	EXPECT_EQ( "co.IInterface", res.get<co::IService*>()->getInterface()->getFullName() );

	reflector->getField( ts, floatArrayField, res );
	EXPECT_EQ( 0, res.get< co::Range<const float> >().getSize() );

	ts->floatArray.push_back( 1.1f );
	ts->floatArray.push_back( 2.2f );
	ts->floatArray.push_back( 3.3f );

	reflector->getField( ts, floatArrayField, res );
	ASSERT_EQ( 3, res.get< co::Range<const float> >().getSize() );
	EXPECT_EQ( 1.1f, res.get< co::Range<const float> >()[0] );
	EXPECT_EQ( 2.2f, res.get< co::Range<const float> >()[1] );
	EXPECT_EQ( 3.3f, res.get< co::Range<const float> >()[2] );

	// --- in-place destruction:
	reflector->destroyValue( ts );
}

TEST( ReflectionTests, getAndBindComponentInterfaces )
{
	co::RefPtr<co::IObject> instance = co::newInstance( "moduleA.TestComponent" );
	ASSERT_TRUE( instance.isValid() );

	EXPECT_THROW( instance->getService( NULL ), co::NoSuchPortException );

	co::IComponent* type = instance->getComponent();

	// get a facet info
	co::IPort* testInterfaceInfo = dynamic_cast<co::IPort*>( type->getMember( "testInterface" ) );
	ASSERT_TRUE( testInterfaceInfo != NULL );

	// get the 'testInterface' instance
	co::IService* service = instance->getService( testInterfaceInfo );
	EXPECT_EQ( instance->getService<moduleA::TestInterface>(), service );

	// cannot 'bind' to a facet
	EXPECT_THROW( instance->setService( testInterfaceInfo, service ), co::NoSuchPortException );

	// get IPort's for the receptacles
	co::IPort* typePort = dynamic_cast<co::IPort*>( type->getMember( "type" ) );
	co::IPort* itfTypePort = dynamic_cast<co::IPort*>( type->getMember( "itf" ) );
	ASSERT_TRUE( typePort && itfTypePort );

	// get the service currently bound to the 'type' receptacle (should be null)
	EXPECT_EQ( NULL, instance->getService( typePort ) );

	// attempting to bind a IStruct to 'itf' should produce an exception (it expects an IInterface)
	co::IType* structType = co::getType( "moduleA.TestStruct" );
	EXPECT_THROW( instance->setService( itfTypePort, structType ), co::IllegalCastException );

	// bind an IInterface to both receptacles
	co::IType* itf = co::getType( "moduleA.TestInterface" );

	instance->setService( typePort, itf );
	service = instance->getService( typePort );
	EXPECT_EQ( itf->getProvider(), service->getProvider() );
	EXPECT_EQ( itf->getFacet(), service->getFacet() );

	instance->setService( itfTypePort, itf );
	service = instance->getService( itfTypePort );
	EXPECT_EQ( itf->getProvider(), service->getProvider() );
	EXPECT_EQ( itf->getFacet(), service->getFacet() );

	// try setting a receptacle by name
	co::bindService( instance.get(), "type", NULL );
	EXPECT_EQ( NULL, instance->getService( typePort ) );
	EXPECT_THROW( co::bindService( instance.get(), "noReceptacle", NULL ), co::NoSuchPortException );
}
