/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#include <gtest/gtest.h>

#include <co/Coral.h>
#include <co/Component.h>
#include <co/Reflector.h>
#include <co/MethodInfo.h>
#include <co/CompoundType.h>
#include <co/AttributeInfo.h>
#include <co/InterfaceInfo.h>
#include <co/ComponentType.h>
#include <co/IllegalCastException.h>
#include <co/IllegalArgumentException.h>
#include <co/NoSuchInterfaceException.h>
#include <moduleA/TestStruct.h>
#include <moduleA/TestInterface.h>

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

TEST( ReflectionTests, structGetSetInterfacesAndArrays )
{
	co::Type* type = co::getType( "moduleA.TestStruct" );
	co::Reflector* reflector = type->getReflector();
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
	co::AttributeInfo* aTypeAttrib = getAttributeInfo( type, "aType" );
	ASSERT_TRUE( aTypeAttrib != NULL );

	co::AttributeInfo* floatArrayAttrib = getAttributeInfo( type, "floatArray" );
	ASSERT_TRUE( floatArrayAttrib != NULL );

	co::AttributeInfo* typeArrayAttrib = getAttributeInfo( type, "typeArray" );
	ASSERT_TRUE( typeArrayAttrib != NULL );

	// --- attribute setting:
	reflector->setAttribute( ts, aTypeAttrib, co::getType( "co.ArrayType" ) );
	ASSERT_TRUE( ts->aType.isValid() );
	EXPECT_EQ( "co.ArrayType", ts->aType->getFullName() );

	std::vector<co::Type*> typeVec;
	typeVec.push_back( co::getType( "co.MemberInfo" ) );
	typeVec.push_back( co::getType( "co.AttributeInfo" ) );
	typeVec.push_back( co::getType( "co.MethodInfo" ) );
	
	co::Any typeVecAny;
	typeVecAny.set<std::vector<co::Type*>&>( typeVec );

	EXPECT_THROW( reflector->setAttribute( ts, floatArrayAttrib, typeVecAny ), co::IllegalCastException );
	EXPECT_NO_THROW( reflector->setAttribute( ts, typeArrayAttrib, typeVecAny ) );

	ASSERT_EQ( 3, ts->typeArray.size() );
	EXPECT_EQ( "co.MemberInfo", ts->typeArray[0]->getFullName() );
	EXPECT_EQ( "co.AttributeInfo", ts->typeArray[1]->getFullName() );
	EXPECT_EQ( "co.MethodInfo", ts->typeArray[2]->getFullName() );

	// --- attribute getting:
	co::Any res;

	reflector->getAttribute( ts, aTypeAttrib, res );
	EXPECT_EQ( "co.InterfaceType", res.get<co::Interface*>()->getInterfaceType()->getFullName() );

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
	co::Component* testComponent = co::newInstance( "moduleA.TestComponent" );
	ASSERT_TRUE( testComponent != NULL );

	EXPECT_THROW( testComponent->getInterface( NULL ), co::NoSuchInterfaceException );

	co::ComponentType* type = testComponent->getComponentType();

	// get a facet info
	co::InterfaceInfo* testInterfaceInfo = dynamic_cast<co::InterfaceInfo*>( type->getMember( "testInterface" ) );
	ASSERT_TRUE( testInterfaceInfo != NULL );

	// get the 'testInterface' instance
	co::Interface* itf = testComponent->getInterface( testInterfaceInfo );
	EXPECT_EQ( testComponent->getFacet<moduleA::TestInterface>(), itf );

	// cannot 'bind' to a facet
	EXPECT_THROW( testComponent->bindInterface( testInterfaceInfo, itf ), co::NoSuchInterfaceException );

	// get InterfaceInfo's for the receptacles
	co::InterfaceInfo* typeItfInfo = dynamic_cast<co::InterfaceInfo*>( type->getMember( "type" ) );
	co::InterfaceInfo* itfTypeItfInfo = dynamic_cast<co::InterfaceInfo*>( type->getMember( "itfType" ) );
	ASSERT_TRUE( typeItfInfo && itfTypeItfInfo );

	// get the interface currently bound to the 'type' receptacle (should be null)
	EXPECT_EQ( NULL, testComponent->getInterface( typeItfInfo ) );

	// attempting to bind a StructType to 'itfType' should produce an exception (it expects an InterfaceType)
	co::Type* structType = co::getType( "moduleA.TestStruct" );
	EXPECT_THROW( testComponent->bindInterface( itfTypeItfInfo, structType ), co::IllegalArgumentException );

	// bind an InterfaceType to both receptacles
	co::Type* itfType = co::getType( "moduleA.TestInterface" );

	testComponent->bindInterface( typeItfInfo, itfType );
	itf = testComponent->getInterface( typeItfInfo );
	EXPECT_EQ( itfType->getInterfaceOwner(), itf->getInterfaceOwner() );
	EXPECT_EQ( itfType->getInterfaceName(), itf->getInterfaceName() );

	testComponent->bindInterface( itfTypeItfInfo, itfType );
	itf = testComponent->getInterface( itfTypeItfInfo );
	EXPECT_EQ( itfType->getInterfaceOwner(), itf->getInterfaceOwner() );
	EXPECT_EQ( itfType->getInterfaceName(), itf->getInterfaceName() );
}
