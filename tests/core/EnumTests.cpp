/*
 * Coral - Lightweight C++ Component Framework
 * See Copyright Notice in Coral.h
 */

#include "CSLTester.h"
#include <gtest/gtest.h>

TEST( EnumTests, simpleValidEnum )
{
	CSL_TEST( "EnumTests.enum_valid" )
}

TEST( EnumTests, emptyDeclaration )
{
	CSL_TEST_BEGIN( "EnumTests.enum_empty_declaration" )
	CSL_EXPECT_SYNTAX_ERROR( "enum_empty_declaration.csl", 3 )
	CSL_TEST_END()
}

TEST( EnumTests, invalidIdentifiers )
{
	CSL_TEST_BEGIN( "EnumTests.enum_invalid_identifier" )
	CSL_EXPECT_SYNTAX_ERROR( "enum_invalid_identifier.csl", 3 )
	CSL_TEST_END()

	CSL_TEST_BEGIN( "EnumTests.enum_invalid_identifier2" )
	CSL_EXPECT_SYNTAX_ERROR( "enum_invalid_identifier2.csl", 3 )
	CSL_TEST_END()
}

TEST( EnumTests, identifierWithTypeName )
{
	// type names are reserved words
	CSL_TEST_BEGIN( "EnumTests.enum_identifierWithTypeName" )
	CSL_EXPECT_SYNTAX_ERROR( "enum_identifierWithTypeName.csl", 4 )
	CSL_TEST_END()
}

TEST( EnumTests, methodDeclaration )
{
	// method declarations are not allowed inside an enum
	CSL_TEST_BEGIN( "EnumTests.enum_methodDeclaration" )
	CSL_EXPECT_SYNTAX_ERROR( "enum_methodDeclaration.csl", 4 )
	CSL_TEST_END()
}

TEST( EnumTests, inheritance )
{
	// inheritance not allowed
	CSL_TEST_BEGIN( "EnumTests.enum_inheritance" )
	CSL_EXPECT_SYNTAX_ERROR( "enum_inheritance.csl", 1 )
	CSL_TEST_END()
}

TEST( EnumTests, miscSyntaxErrors )
{
	CSL_TEST_BEGIN( "EnumTests.enum_syntaxerror1" )
	CSL_EXPECT_SYNTAX_ERROR( "enum_syntaxerror1.csl", 2 )
	CSL_TEST_END()

	CSL_TEST_BEGIN( "EnumTests.enum_syntaxerror1" )
	CSL_EXPECT_SYNTAX_ERROR( "enum_syntaxerror1.csl", 2 )
	CSL_TEST_END()
}
