/*
 * Coral - A C++ Component Framework.
 * See Copyright Notice in Coral.h
 */

#include "CSLTester.h"
#include <gtest/gtest.h>

TEST( StructTypeTests, simpleValidStruct )
{
	CSL_TEST( "StructTypeTests.struct_valid" )
}

TEST( StructTypeTests, missingDefinition )
{
	CSL_TEST_BEGIN( "StructTypeTests.struct_missingDefinition" )
	CSL_EXPECT_SEMANTIC_ERROR( "missing struct contents" )
	CSL_TEST_END()
}

TEST( StructTypeTests, conflictingType )
{
	// loads the struct_valid type, in order produce a type conflict
	// it checks if struct_valid is already loaded (to keep independence from
	// the first struct test)
	co::Type* structValid = TestHelper::type( "StructTypeTests.struct_valid" );
	if( structValid == NULL )
	{
		CSL_TEST( "StructTypeTests.struct_valid" )
	}

	// altough it seems that a conflicting type name is a semantic error,
	// it is handled as a syntax error because the type definition cannot be
	// completed by the parser: it tries to create the type but it already exists
	CSL_TEST_BEGIN( "StructTypeTests.struct_valid" )
	CSL_EXPECT_ERROR( "already contains a type called 'struct_valid'", "struct_valid.csl", 2 )
	CSL_TEST_END()
}

TEST( StructTypeTests, inheritance )
{
	// structs don't support inheritance syntax
	CSL_TEST_BEGIN( "StructTypeTests.struct_inheritance" )
	CSL_EXPECT_SYNTAX_ERROR( "struct_inheritance.csl", 1 )
	CSL_TEST_END()
}

TEST( StructTypeTests, methodDefinition )
{
	// structs don't support method definitions
	CSL_TEST_BEGIN( "StructTypeTests.struct_methods" )
	CSL_EXPECT_SYNTAX_ERROR( "struct_methods.csl", 7 )
	CSL_TEST_END()
}

TEST( StructTypeTests, badAttributeName )
{
	CSL_TEST_BEGIN( "StructTypeTests.struct_badAttributeName" )
	CSL_EXPECT_SYNTAX_ERROR( "struct_badAttributeName.csl", 3 )
	CSL_TEST_END()

	CSL_TEST_BEGIN( "StructTypeTests.struct_badAttributeName2" )
	CSL_EXPECT_SYNTAX_ERROR( "struct_badAttributeName2.csl", 3 )
	CSL_TEST_END()

	CSL_TEST_BEGIN( "StructTypeTests.struct_badAttributeName3" )
	CSL_EXPECT_SYNTAX_ERROR( "struct_badAttributeName3.csl", 3 )
	CSL_TEST_END()
}

TEST( StructTypeTests, cyclicDependencies )
{
	// cyclic dependencies are naturally handled by the compiler
	CSL_TEST( "StructTypeTests.struct_valid2" )
}

TEST( StructTypeTests, selfReference )
{
	CSL_TEST_BEGIN( "StructTypeTests.struct_self_reference" )
	CSL_EXPECT_ERROR( "a struct cannot contain itself recursively", "struct_self_reference.csl", 4 )
	CSL_TEST_END()
}

TEST( StructTypeTests, validUsingInvalid )
{
	CSL_TEST_BEGIN( "StructTypeTests.struct_validUsingAnInvalid" )
	CSL_EXPECT_ERROR( "error loading dependency", "struct_validUsingAnInvalid.csl", 4 )
	CSL_EXPECT_SYNTAX_ERROR( "struct_invalid.csl", 3 )
	CSL_TEST_END()
}

TEST( StructTypeTests, withInvalidMemberType )
{
	CSL_TEST_BEGIN( "StructTypeTests.struct_invalid_with_component" )
	CSL_EXPECT_ERROR( "components are illegal as attribute types", "struct_invalid_with_component.csl", 4 )
	CSL_TEST_END()

	CSL_TEST_BEGIN( "StructTypeTests.struct_invalid_with_exception" )
	CSL_EXPECT_ERROR( "exceptions are illegal as attribute types", "struct_invalid_with_exception.csl", 4 )
	CSL_TEST_END()
}
