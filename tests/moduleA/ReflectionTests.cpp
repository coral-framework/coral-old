/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#include <gtest/gtest.h>

#include <co/Coral.h>
#include <co/IComponent.h>
#include <co/IReflector.h>
#include <co/IMethodInfo.h>
#include <co/ICompoundType.h>
#include <co/IAttributeInfo.h>
#include <co/IInterfaceInfo.h>
#include <co/IComponentType.h>
#include <co/IllegalCastException.h>
#include <co/IllegalArgumentException.h>
#include <co/NoSuchInterfaceException.h>
#include <moduleA/TestStruct.h>
#include <moduleA/TestInterface.h>

template<typename T>
T* getMember( co::IType* type, const char* memberName )
{
	co::ICompoundType* ct = dynamic_cast<co::ICompoundType*>( type );
	assert( ct );

	T* mi = dynamic_cast<T*>( ct->getMember( memberName ) );
	assert( mi );

	return mi;
}

co::IAttributeInfo* getAttributeInfo( co::IType* type, const char* memberName )
{
	return getMember<co::IAttributeInfo>( type, memberName );
}

co::IMethodInfo* getMethodInfo( co::IType* type, const char* memberName )
{
	return getMember<co::IMethodInfo>( type, memberName );
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
	co::IAttributeInfo* aTypeAttrib = getAttributeInfo( type, "aType" );
	ASSERT_TRUE( aTypeAttrib != NULL );

	co::IAttributeInfo* floatArrayAttrib = getAttributeInfo( type, "floatArray" );
	ASSERT_TRUE( floatArrayAttrib != NULL );

	co::IAttributeInfo* typeArrayAttrib = getAttributeInfo( type, "typeArray" );
	ASSERT_TRUE( typeArrayAttrib != NULL );

	// --- attribute setting:
	reflector->setAttribute( ts, aTypeAttrib, co::getType( "co.IArrayType" ) );
	ASSERT_TRUE( ts->aType.isValid() );
	EXPECT_EQ( "co.IArrayType", ts->aType->getFullName() );

	std::vector<co::IType*> typeVec;
	typeVec.push_back( co::getType( "co.IMemberInfo" ) );
	typeVec.push_back( co::getType( "co.IAttributeInfo" ) );
	typeVec.push_back( co::getType( "co.IMethodInfo" ) );
	
	co::Any typeVecAny;
	typeVecAny.set<std::vector<co::IType*>&>( typeVec );

	EXPECT_THROW( reflector->setAttribute( ts, floatArrayAttrib, typeVecAny ), co::IllegalCastException );
	EXPECT_NO_THROW( reflector->setAttribute( ts, typeArrayAttrib, typeVecAny ) );

	ASSERT_EQ( 3, ts->typeArray.size() );
	EXPECT_EQ( "co.IMemberInfo", ts->typeArray[0]->getFullName() );
	EXPECT_EQ( "co.IAttributeInfo", ts->typeArray[1]->getFullName() );
	EXPECT_EQ( "co.IMethodInfo", ts->typeArray[2]->getFullName() );

	// --- attribute getting:
	co::Any res;

	reflector->getAttribute( ts, aTypeAttrib, res );
	EXPECT_EQ( "co.IInterfaceType", res.get<co::Interface*>()->getInterfaceType()->getFullName() );

	reflector->getAttribute( ts, floatArrayAttrib, res );
	EXPECT_EQ( 0, res.get< co::ArrayRange<const float> >().getSize() );

	ts->floatArray.push_back( 1.1f );
	ts->floatArray.push_back( 2.2f );
	ts->floatArray.push_back( 3.3f );

	reflector->getAttribute( ts, floatArrayAttrib, res );
	ASSERT_EQ( 3, res.get< co::ArrayRange<const float> >().getSize() );
	EXPECT_EQ( 1.1f, res.get< co::ArrayRange<const float> >()[0] );
	EXPECT_EQ( 2.2f, res.get< co::ArrayRange<const float> >()[1] );
	EXPECT_EQ( 3.3f, res.get< co::ArrayRange<const float> >()[2] );

	// --- in-place destruction:
	reflector->destroyValue( ts );
}

TEST( ReflectionTests, getAndBindComponentInterfaces )
{
	co::RefPtr<co::IComponent> testComponent = co::newInstance( "moduleA.TestComponent" );
	ASSERT_TRUE( testComponent.isValid() );

	EXPECT_THROW( testComponent->getInterface( NULL ), co::NoSuchInterfaceException );

	co::IComponentType* type = testComponent->getComponentType();

	// get a facet info
	co::IInterfaceInfo* testInterfaceInfo = dynamic_cast<co::IInterfaceInfo*>( type->getMember( "testInterface" ) );
	ASSERT_TRUE( testInterfaceInfo != NULL );

	// get the 'testInterface' instance
	co::Interface* itf = testComponent->getInterface( testInterfaceInfo );
	EXPECT_EQ( testComponent->getFacet<moduleA::TestInterface>(), itf );

	// cannot 'bind' to a facet
	EXPECT_THROW( testComponent->setReceptacle( testInterfaceInfo, itf ), co::NoSuchInterfaceException );

	// get IInterfaceInfo's for the receptacles
	co::IInterfaceInfo* typeItfInfo = dynamic_cast<co::IInterfaceInfo*>( type->getMember( "type" ) );
	co::IInterfaceInfo* itfTypeItfInfo = dynamic_cast<co::IInterfaceInfo*>( type->getMember( "itfType" ) );
	ASSERT_TRUE( typeItfInfo && itfTypeItfInfo );

	// get the interface currently bound to the 'type' receptacle (should be null)
	EXPECT_EQ( NULL, testComponent->getInterface( typeItfInfo ) );

	// attempting to bind a IStructType to 'itfType' should produce an exception (it expects an IInterfaceType)
	co::IType* structType = co::getType( "moduleA.TestStruct" );
	EXPECT_THROW( testComponent->setReceptacle( itfTypeItfInfo, structType ), co::IllegalArgumentException );

	// bind an IInterfaceType to both receptacles
	co::IType* itfType = co::getType( "moduleA.TestInterface" );

	testComponent->setReceptacle( typeItfInfo, itfType );
	itf = testComponent->getInterface( typeItfInfo );
	EXPECT_EQ( itfType->getInterfaceOwner(), itf->getInterfaceOwner() );
	EXPECT_EQ( itfType->getInterfaceName(), itf->getInterfaceName() );

	testComponent->setReceptacle( itfTypeItfInfo, itfType );
	itf = testComponent->getInterface( itfTypeItfInfo );
	EXPECT_EQ( itfType->getInterfaceOwner(), itf->getInterfaceOwner() );
	EXPECT_EQ( itfType->getInterfaceName(), itf->getInterfaceName() );

	// try setting a receptacle by name
	co::setReceptacleByName( testComponent.get(), "type", NULL );
	EXPECT_EQ( NULL, testComponent->getInterface( typeItfInfo ) );
	EXPECT_THROW( co::setReceptacleByName( testComponent.get(), "noReceptacle", NULL ), co::NoSuchInterfaceException );
}
