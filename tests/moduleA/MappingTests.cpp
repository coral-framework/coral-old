/*
 * Coral - Lightweight C++ Component Framework.
 * See Copyright Notice in Coral.h
 */

#include <gtest/gtest.h>

#include <co/Coral.h>
#include <co/Component.h>
#include <moduleA/Vec2D.h>
#include <moduleA/TestEnum.h>
#include <moduleA/TestStruct.h>
#include <moduleA/TestException.h>
#include <moduleA/TestInterface.h>
#include <moduleA/DummyInterface.h>
#include <moduleA/TestNamespaces.h>
#include <moduleA/BadlySortedStruct.h>
#include <moduleA/IBatman.h>

TEST( MappingTests, enumValues )
{
	EXPECT_EQ( 0, moduleA::First );
	EXPECT_EQ( 1, moduleA::Second );
	EXPECT_EQ( 2, moduleA::Third );

	EXPECT_EQ( 2, moduleA::First + moduleA::Third );
	EXPECT_EQ( 1, moduleA::Third - moduleA::Second );
}

TEST( MappingTests, enumSize )
{
	EXPECT_EQ( sizeof(moduleA::TestEnum), 4 );
}

TEST( MappingTests, structFields )
{
	moduleA::TestStruct dummyStruct;

	dummyStruct.anInt8 = co::MAX_INT8;
	ASSERT_EQ( dummyStruct.anInt8, co::MAX_INT8 );

	dummyStruct.anInt16 = co::MAX_INT16;
	ASSERT_TRUE( dummyStruct.anInt16 == co::MAX_INT16 );

	dummyStruct.testEnum = moduleA::First;
	ASSERT_TRUE( dummyStruct.testEnum == moduleA::First );
	ASSERT_FALSE( dummyStruct.testEnum == moduleA::Third );

	dummyStruct.floatArray.push_back( 1.01234f );
	ASSERT_FALSE( dummyStruct.floatArray.empty() );
	ASSERT_EQ( 1, dummyStruct.floatArray.size() );
	ASSERT_EQ( 1.01234f, dummyStruct.floatArray[0] );

	dummyStruct.enumArray.push_back( moduleA::Second );
	ASSERT_FALSE( dummyStruct.enumArray.empty() );
	ASSERT_EQ( 1, dummyStruct.enumArray.size() );
	ASSERT_EQ( moduleA::Second, dummyStruct.enumArray[0] );

	dummyStruct.typeArray.push_back( NULL );
	co::ArrayRange<co::Type* const> typeRange( dummyStruct.typeArray );
	ASSERT_EQ( 1, typeRange.getSize() );
	ASSERT_EQ( dummyStruct.typeArray.front().get(), typeRange.getFirst() );
}

TEST( MappingTests, structAlignment )
{
	moduleA::BadlySortedStruct bss;

	EXPECT_EQ( 1, sizeof(bss.myInt8) );
	EXPECT_EQ( 8, sizeof(bss.myDouble) );
	EXPECT_EQ( 2, sizeof(bss.myInt16) );
	EXPECT_EQ( 4, sizeof(bss.myFloat) );
	EXPECT_EQ( 4, sizeof(bss.myEnum) );
	EXPECT_EQ( 1, sizeof(bss.myBool) );
	EXPECT_EQ( 1, sizeof(bss.myUInt8) );
	EXPECT_EQ( 4, sizeof(bss.myInt32) );
	EXPECT_EQ( 2, sizeof(bss.myUInt16) );

#if defined(CORAL_OS_UNIX) && CORAL_POINTER_SIZE == 4
	// double alignment on 32-bit Linuxses is 4-byte aligned
	EXPECT_EQ( 28, sizeof(moduleA::BadlySortedStruct) );
#else
	// double alignment is 8-byte aligned on 64-bit systems and on Win32
	EXPECT_EQ( 32, sizeof(moduleA::BadlySortedStruct) );
#endif

}

TEST( MappingTests, exception )
{
	moduleA::TestException ex;
	ASSERT_TRUE( static_cast<co::Exception*>( &ex ) != NULL );
	EXPECT_THROW( throw moduleA::TestException();, moduleA::TestException );

	try
	{
		throw moduleA::TestException( "TEST MESSAGE" );
	}
	catch( co::Exception& e )
	{
		ASSERT_EQ( "TEST MESSAGE", e.getMessage() );
	}
}

TEST( MappingTests, interface )
{
	co::RefPtr<co::Component> testComponent = co::newInstance( "moduleA.TestComponent" );

	moduleA::TestInterface* ti = testComponent->getFacet<moduleA::TestInterface>();

	ASSERT_EQ( 1, ti->testCppBlock1() );
	ASSERT_EQ( 2, ti->testCppBlock2() );

	ASSERT_EQ( "NOT SET", ti->getName() );
	ti->setName( "VALUE" );
	ASSERT_EQ( "VALUE", ti->getName() );

	ASSERT_EQ( NULL, ti->getDummyInterfaceAttribute() );

	float size = 1;
	std::string text = "VALUE";
	moduleA::TestEnum enumValue = moduleA::Second;
	moduleA::TestStruct testStruct;
	testStruct.anInt16 = 42;
	std::vector<co::int32> intVector( 1, 1 );
	co::ArrayRange<co::int32 const> intList(  intVector );

	co::RefVector<moduleA::DummyInterface> interfaceRefVector;
	interfaceRefVector.push_back( NULL );

	std::vector<moduleA::DummyInterface*> interfaceVector;
	interfaceVector.push_back( NULL );

	co::ArrayRange<moduleA::DummyInterface* const> interfaceList( interfaceVector );

	// Callling this method should NOT alter the parameters
	ti->testInParameters( size, enumValue, text, testStruct, NULL, intList, interfaceList );

	ASSERT_EQ( 1, size );
	ASSERT_EQ( "VALUE", text );
	ASSERT_EQ( moduleA::Second, enumValue );
	ASSERT_EQ( 42, testStruct.anInt16 );

	moduleA::DummyInterface* dummyPtr = NULL;

	// Callling this method should modify the parameters
	ti->testOutParameters( size, enumValue, text, testStruct, dummyPtr, intVector, interfaceRefVector );

	ASSERT_NE( 1, size );
	ASSERT_NE( "VALUE", text );
	ASSERT_NE( moduleA::Second, enumValue );
	ASSERT_NE( 42, testStruct.anInt16 );
	ASSERT_TRUE( dummyPtr == NULL );
}

TEST( MappingTests, interfaceDisambiguation )
{
	EXPECT_FALSE( ( co::traits::hasAmbiguousBase<moduleA::IBat, co::Interface>::value ) );

	EXPECT_TRUE( ( co::traits::hasAmbiguousBase<moduleA::IBatman, co::Interface>::value ) );
	EXPECT_TRUE( ( co::traits::hasAmbiguousBase<moduleA::IBatman, moduleA::IMammal>::value ) );

	moduleA::IBatman* batman = NULL;
	co::Interface* itf = co::disambiguate<co::Interface>( batman );
	moduleA::IMammal* mammalItf = co::disambiguate<moduleA::IMammal>( batman );

	CORAL_UNUSED( itf );
	CORAL_UNUSED( mammalItf );
}

TEST( MappingTests, nativeClass )
{
 	EXPECT_STREQ( "moduleA.Vec2D", ( co::nameOf< std::pair<double, double> >::get() ) );
}

TEST( MappingTests, namespaces )
{
	ASSERT_EQ( 0, moduleA::Continent::France );
	ASSERT_EQ( 1, moduleA::Continent::Portugal );
	ASSERT_EQ( 2, moduleA::Continent::England );

	moduleA::Continent::Country::China secondTest;
	secondTest.demographicDensity = 137;
	secondTest.population = 1321851888;

	ASSERT_FLOAT_EQ( 137, secondTest.demographicDensity );
	ASSERT_EQ( 1321851888, secondTest.population );

	ASSERT_EQ( 0, moduleA::Continent::Country::State::LasVegas );
	ASSERT_EQ( 1, moduleA::Continent::Country::State::Reno );
	ASSERT_EQ( 2, moduleA::Continent::Country::State::Henderson );
	ASSERT_EQ( 3, moduleA::Continent::Country::State::Sparks );
}

TEST( MappingTests, nameOf )
{
 	EXPECT_STREQ( "moduleA.TestNamespaces", co::nameOf<moduleA::TestNamespaces>::get() );
 	EXPECT_STREQ( "moduleA.Continent.Country.China", co::nameOf<moduleA::Continent::Country::China>::get() );
 	EXPECT_STREQ( "moduleA.DummyInterface", co::nameOf<moduleA::DummyInterface>::get() );
 	EXPECT_STREQ( "moduleA.TestStruct", co::nameOf<moduleA::TestStruct>::get() );
 	EXPECT_STREQ( "moduleA.TestEnum", co::nameOf<moduleA::TestEnum>::get() );
 	EXPECT_STREQ( "moduleA.TestException", co::nameOf<moduleA::TestException>::get() );
}
