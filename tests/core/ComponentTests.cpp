/*
 * Coral - Lightweight C++ Component Framework
 * See Copyright Notice in Coral.h
 */

#include "CSLTester.h"

#include <gtest/gtest.h>

TEST( ComponentTests, invalidDefinitions )
{
	CSL_TEST_BEGIN( "ComponentTests.InvalidDefinitions.interfaceArrayMember" )
	CSL_EXPECT_SYNTAX_ERROR( "interfaceArrayMember.csl", 4 )
	CSL_TEST_END()

	CSL_TEST_BEGIN( "ComponentTests.InvalidDefinitions.withoutInterfaceRole" )
	CSL_EXPECT_SYNTAX_ERROR( "withoutInterfaceRole.csl", 4 )
	CSL_TEST_END()

	CSL_TEST_BEGIN( "ComponentTests.InvalidDefinitions.interfaceNotFound" )
	CSL_EXPECT_ERROR( "error loading dependency", "interfaceNotFound.csl", 4 )
	CSL_EXPECT_ERROR( "was not found", "", -1 )
	CSL_TEST_END()

	CSL_TEST_BEGIN( "ComponentTests.InvalidDefinitions.memberClash" )
	CSL_EXPECT_SEMANTIC_ERROR( "duplicate" )
	CSL_TEST_END()

	CSL_TEST_BEGIN( "ComponentTests.InvalidDefinitions.notAnInterfaceMember" )
	CSL_EXPECT_ERROR( "interface type was expected", "notAnInterfaceMember.csl", 4 )
	CSL_TEST_END()
}

TEST( ComponentTests, validDefinitions )
{
	CSL_TEST( "ComponentTests.ValidDefinitions.valid" )
}
