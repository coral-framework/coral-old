/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#include "CSLTester.h"
#include <gtest/gtest.h>

TEST( InterfaceTests, fieldAndMethodClashes )
{
	CSL_TEST_BEGIN( "InterfaceTests.FieldAndMethodClashes.methodAndFieldMemberName" )
	CSL_EXPECT_SEMANTIC_ERROR( "name clash" )
	CSL_TEST_END()

	CSL_TEST_BEGIN( "InterfaceTests.FieldAndMethodClashes.fieldGetterAndMethod" )
	CSL_EXPECT_SEMANTIC_ERROR( "name clash" )
	CSL_TEST_END()

	CSL_TEST_BEGIN( "InterfaceTests.FieldAndMethodClashes.fieldSetterAndMethod" )
	CSL_EXPECT_SEMANTIC_ERROR( "name clash" )
	CSL_TEST_END()

	CSL_TEST( "InterfaceTests.FieldAndMethodClashes.fieldSetterAndMethod_valid" )

	CSL_TEST_BEGIN( "InterfaceTests.FieldAndMethodClashes.methodAndFieldGetter" )
	CSL_EXPECT_SEMANTIC_ERROR( "name clash" )
	CSL_TEST_END()

	CSL_TEST_BEGIN( "InterfaceTests.FieldAndMethodClashes.methodAndFieldSetter" )
	CSL_EXPECT_SEMANTIC_ERROR( "name clash" )
	CSL_TEST_END()

	CSL_TEST( "InterfaceTests.FieldAndMethodClashes.methodAndFieldSetter_valid" )
}

TEST( InterfaceTests, clashesWithSuperType )
{
	CSL_TEST_BEGIN( "InterfaceTests.ClashesWithSuperType.Interface1" )
	CSL_EXPECT_SEMANTIC_ERROR( "name clash" )
	CSL_TEST_END()

	CSL_TEST_BEGIN( "InterfaceTests.ClashesWithSuperType.Interface2" )
	CSL_EXPECT_SEMANTIC_ERROR( "name clash" )
	CSL_TEST_END()

	CSL_TEST_BEGIN( "InterfaceTests.ClashesWithSuperType.Interface3" )
	CSL_EXPECT_SEMANTIC_ERROR( "name clash" )
	CSL_TEST_END()

	CSL_TEST_BEGIN( "InterfaceTests.ClashesWithSuperType.Interface4" )
	CSL_EXPECT_SEMANTIC_ERROR( "name clash" )
	CSL_TEST_END()

	// this test catches the error in a second iteration of the semantic checker
	CSL_TEST_BEGIN( "InterfaceTests.StartFromParent.ClashingParent" )
	CSL_EXPECT_SEMANTIC_ERROR( "name clash" )
	CSL_TEST_END()

	CSL_TEST_BEGIN( "InterfaceTests.DeepInheritanceTree.UserClash" )
	CSL_EXPECT_SEMANTIC_ERROR( "name clash" )
	CSL_TEST_END()
}

TEST( InterfaceTests, firstOderInheritance )
{
	CSL_TEST( "InterfaceTests.FirstOrder.Child" )

	CSL_TEST( "InterfaceTests.FirstOrder.InheritanceOnly" )

	CSL_TEST( "InterfaceTests.StartFromParent.Parent" )
}

TEST( InterfaceTests, deepInheritanceTree )
{
	CSL_TEST( "InterfaceTests.DeepInheritanceTree.User" )
}

TEST( InterfaceTests, invalidDeclarations )
{
	CSL_TEST_BEGIN( "InterfaceTests.InvalidDeclarations.emptyDeclaration" )
	CSL_EXPECT_SEMANTIC_ERROR( "missing interface contents" )
	CSL_TEST_END()

	CSL_TEST_BEGIN( "InterfaceTests.InvalidDeclarations.emptyDeclarationWithComment" )
	CSL_EXPECT_SEMANTIC_ERROR( "missing interface contents" )
	CSL_TEST_END()

	CSL_TEST_BEGIN( "InterfaceTests.InvalidDeclarations.invalidFieldName" )
	CSL_EXPECT_ERROR( "must start with a lowercase letter", "invalidFieldName.csl", 6 )
	CSL_TEST_END()

	CSL_TEST_BEGIN( "InterfaceTests.InvalidDeclarations.missingParameterIO" )
	CSL_EXPECT_SYNTAX_ERROR( "missingParameterIO.csl", 5 )
	CSL_TEST_END()

	CSL_TEST_BEGIN( "InterfaceTests.InvalidDeclarations.parameterTypeNotFound" )
	CSL_EXPECT_ERROR( "error loading dependency", "parameterTypeNotFound.csl", 4  )
	CSL_EXPECT_ERROR( "'IDontExixst' was not found", "", -1  )
	CSL_TEST_END()

	CSL_TEST_BEGIN( "InterfaceTests.InvalidDeclarations.parameterNameClash" )
	CSL_EXPECT_ERROR( "parameter 'fooMe' defined twice", "parameterNameClash.csl", 4  )
	CSL_TEST_END()

	CSL_TEST_BEGIN( "InterfaceTests.InvalidDeclarations.missingReturnType" )
	CSL_EXPECT_SYNTAX_ERROR( "missingReturnType.csl", 4 )
	CSL_TEST_END()

	CSL_TEST_BEGIN( "InterfaceTests.InvalidDeclarations.inexistentReturnType" )
	CSL_EXPECT_ERROR( "error loading dependency", "inexistentReturnType.csl", 4 )
	CSL_EXPECT_ERROR( "was not found", "", -1 )
	CSL_TEST_END()

	CSL_TEST_BEGIN( "InterfaceTests.InvalidDeclarations.inexistentRaisedException" )
	CSL_EXPECT_ERROR( "error loading exception type", "inexistentRaisedException.csl", 5 )
	CSL_EXPECT_ERROR( "was not found", "", -1 )
	CSL_TEST_END()

	CSL_TEST_BEGIN( "InterfaceTests.InvalidDeclarations.missingReturnTypeAndMethodNameClash" )
	CSL_EXPECT_SYNTAX_ERROR( "missingReturnTypeAndMethodNameClash.csl", 4 )
	CSL_TEST_END()

	CSL_TEST_BEGIN( "InterfaceTests.InvalidDeclarations.RaisesInvalidType" )
	CSL_EXPECT_ERROR( "attempt to raise non-exception type", "RaisesInvalidType.csl", 5  )
	CSL_TEST_END()
}

TEST( InterfaceTests, validDeclarations )
{
	CSL_TEST( "InterfaceTests.ValidDeclarations.validInterface" )
}

TEST( InterfaceTests, cyclicInheritance )
{
	CSL_TEST_BEGIN( "InterfaceTests.CyclicInheritance.A" )
	CSL_EXPECT_SEMANTIC_ERROR( "cyclic inheritance detected" )
	CSL_TEST_END()

	CSL_TEST_BEGIN( "InterfaceTests.CyclicInheritance.C" )
	CSL_EXPECT_SEMANTIC_ERROR( "cyclic inheritance detected" )
	CSL_TEST_END()
}

TEST( InterfaceTests, invalidParameter )
{
	CSL_TEST_BEGIN( "InterfaceTests.InvalidDeclarations.invalidInterfaceWithComponentAsParameter" )
	CSL_EXPECT_ERROR( "components cannot be passed as parameters", "invalidInterfaceWithComponentAsParameter.csl", 6 )
	CSL_TEST_END()

	CSL_TEST_BEGIN( "InterfaceTests.InvalidDeclarations.invalidInterfaceWithExceptionAsParameter" )
	CSL_EXPECT_ERROR( "exceptions cannot be passed as parameters", "invalidInterfaceWithExceptionAsParameter.csl", 6 )
	CSL_TEST_END()
}
