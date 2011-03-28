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

co::IField* getAttributeInfo( co::IType* type, const char* memberName )
{
	return getMember<co::IField>( type, memberName );
}

co::IMethod* getMethodInfo( co::IType* type, const char* memberName )
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

	// --- obtain the necessary attribute infos:
	co::IField* aTypeAttrib = getAttributeInfo( type, "aType" );
	ASSERT_TRUE( aTypeAttrib != NULL );

	co::IField* floatArrayAttrib = getAttributeInfo( type, "floatArray" );
	ASSERT_TRUE( floatArrayAttrib != NULL );

	co::IField* typeArrayAttrib = getAttributeInfo( type, "typeArray" );
	ASSERT_TRUE( typeArrayAttrib != NULL );

	// --- attribute setting:
	reflector->setAttribute( ts, aTypeAttrib, co::getType( "co.IArray" ) );
	ASSERT_TRUE( ts->aType.isValid() );
	EXPECT_EQ( "co.IArray", ts->aType->getFullName() );

	std::vector<co::IType*> typeVec;
	typeVec.push_back( co::getType( "co.IMember" ) );
	typeVec.push_back( co::getType( "co.IField" ) );
	typeVec.push_back( co::getType( "co.IMethod" ) );
	
	co::Any typeVecAny;
	typeVecAny.set<std::vector<co::IType*>&>( typeVec );

	EXPECT_THROW( reflector->setAttribute( ts, floatArrayAttrib, typeVecAny ), co::IllegalCastException );
	EXPECT_NO_THROW( reflector->setAttribute( ts, typeArrayAttrib, typeVecAny ) );

	ASSERT_EQ( 3, ts->typeArray.size() );
	EXPECT_EQ( "co.IMember", ts->typeArray[0]->getFullName() );
	EXPECT_EQ( "co.IField", ts->typeArray[1]->getFullName() );
	EXPECT_EQ( "co.IMethod", ts->typeArray[2]->getFullName() );

	// --- attribute getting:
	co::Any res;

	reflector->getAttribute( ts, aTypeAttrib, res );
	EXPECT_EQ( "co.IInterface", res.get<co::IService*>()->getInterfaceType()->getFullName() );

	reflector->getAttribute( ts, floatArrayAttrib, res );
	EXPECT_EQ( 0, res.get< co::Range<const float> >().getSize() );

	ts->floatArray.push_back( 1.1f );
	ts->floatArray.push_back( 2.2f );
	ts->floatArray.push_back( 3.3f );

	reflector->getAttribute( ts, floatArrayAttrib, res );
	ASSERT_EQ( 3, res.get< co::Range<const float> >().getSize() );
	EXPECT_EQ( 1.1f, res.get< co::Range<const float> >()[0] );
	EXPECT_EQ( 2.2f, res.get< co::Range<const float> >()[1] );
	EXPECT_EQ( 3.3f, res.get< co::Range<const float> >()[2] );

	// --- in-place destruction:
	reflector->destroyValue( ts );
}

TEST( ReflectionTests, getAndBindComponentInterfaces )
{
	co::RefPtr<co::IObject> testComponent = co::newInstance( "moduleA.TestComponent" );
	ASSERT_TRUE( testComponent.isValid() );

	EXPECT_THROW( testComponent->getInterface( NULL ), co::NoSuchPortException );

	co::IComponent* type = testComponent->getComponentType();

	// get a facet info
	co::IPort* testInterfaceInfo = dynamic_cast<co::IPort*>( type->getMember( "testInterface" ) );
	ASSERT_TRUE( testInterfaceInfo != NULL );

	// get the 'testInterface' instance
	co::IService* itf = testComponent->getInterface( testInterfaceInfo );
	EXPECT_EQ( testComponent->getFacet<moduleA::TestInterface>(), itf );

	// cannot 'bind' to a facet
	EXPECT_THROW( testComponent->setReceptacle( testInterfaceInfo, itf ), co::NoSuchPortException );

	// get IPort's for the receptacles
	co::IPort* typePort = dynamic_cast<co::IPort*>( type->getMember( "type" ) );
	co::IPort* itfTypePort = dynamic_cast<co::IPort*>( type->getMember( "itfType" ) );
	ASSERT_TRUE( typePort && itfTypePort );

	// get the interface currently bound to the 'type' receptacle (should be null)
	EXPECT_EQ( NULL, testComponent->getInterface( typePort ) );

	// attempting to bind a IStruct to 'itfType' should produce an exception (it expects an IInterface)
	co::IType* structType = co::getType( "moduleA.TestStruct" );
	EXPECT_THROW( testComponent->setReceptacle( itfTypePort, structType ), co::IllegalCastException );

	// bind an IInterface to both receptacles
	co::IType* itfType = co::getType( "moduleA.TestInterface" );

	testComponent->setReceptacle( typePort, itfType );
	itf = testComponent->getInterface( typePort );
	EXPECT_EQ( itfType->getInterfaceOwner(), itf->getInterfaceOwner() );
	EXPECT_EQ( itfType->getInterfaceName(), itf->getInterfaceName() );

	testComponent->setReceptacle( itfTypePort, itfType );
	itf = testComponent->getInterface( itfTypePort );
	EXPECT_EQ( itfType->getInterfaceOwner(), itf->getInterfaceOwner() );
	EXPECT_EQ( itfType->getInterfaceName(), itf->getInterfaceName() );

	// try setting a receptacle by name
	co::setReceptacleByName( testComponent.get(), "type", NULL );
	EXPECT_EQ( NULL, testComponent->getInterface( typePort ) );
	EXPECT_THROW( co::setReceptacleByName( testComponent.get(), "noReceptacle", NULL ), co::NoSuchPortException );
}
