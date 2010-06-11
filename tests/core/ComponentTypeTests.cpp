/*
 * Coral - A C++ Component Framework.
 * See Copyright Notice in Coral.h
 */

#include "CSLTester.h"

#include <gtest/gtest.h>

TEST( ComponentTypeTests, invalidDefinitions )
{
	CSL_TEST_BEGIN( "ComponentTypeTests.InvalidDefinitions.interfaceArrayMember" )
	CSL_EXPECT_SYNTAX_ERROR( "interfaceArrayMember.csl", 4 )
	CSL_TEST_END()

	CSL_TEST_BEGIN( "ComponentTypeTests.InvalidDefinitions.withoutInterfaceRole" )
	CSL_EXPECT_SYNTAX_ERROR( "withoutInterfaceRole.csl", 4 )
	CSL_TEST_END()

	CSL_TEST_BEGIN( "ComponentTypeTests.InvalidDefinitions.interfaceNotFound" )
	CSL_EXPECT_ERROR( "error loading dependency", "interfaceNotFound.csl", 4 )
	CSL_EXPECT_ERROR( "was not found", "", -1 )
	CSL_TEST_END()

	CSL_TEST_BEGIN( "ComponentTypeTests.InvalidDefinitions.memberClash" )
	CSL_EXPECT_ERROR( "clashes with a previous definition", "memberClash.csl", 5 )
	CSL_TEST_END()

	CSL_TEST_BEGIN( "ComponentTypeTests.InvalidDefinitions.notAnInterfaceMember" )
	CSL_EXPECT_ERROR( "interface type was expected", "notAnInterfaceMember.csl", 4 )
	CSL_TEST_END()
}

TEST( ComponentTypeTests, validDefinitions )
{
	CSL_TEST( "ComponentTypeTests.ValidDefinitions.valid" )
}
