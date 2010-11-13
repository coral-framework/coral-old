/*
 * Coral - Lightweight C++ Component Framework.
 * See Copyright Notice in Coral.h
 */

#include "CSLTester.h"
#include <gtest/gtest.h>

TEST( InterfaceTypeTests, attributeAndMethodClash )
{
	CSL_TEST_BEGIN( "InterfaceTypeTests.AttributeAndMethodClashes.methodAndAttributeMemberName" )
	CSL_EXPECT_SEMANTIC_ERROR( "name clash" )
	CSL_TEST_END()

	CSL_TEST_BEGIN( "InterfaceTypeTests.AttributeAndMethodClashes.attributeGetterAndMethod" )
	CSL_EXPECT_SEMANTIC_ERROR( "name clash" )
	CSL_TEST_END()

	CSL_TEST_BEGIN( "InterfaceTypeTests.AttributeAndMethodClashes.attributeSetterAndMethod" )
	CSL_EXPECT_SEMANTIC_ERROR( "name clash" )
	CSL_TEST_END()

	CSL_TEST( "InterfaceTypeTests.AttributeAndMethodClashes.attributeSetterAndMethod_valid" )

	CSL_TEST_BEGIN( "InterfaceTypeTests.AttributeAndMethodClashes.methodAndAttributeGetter" )
	CSL_EXPECT_SEMANTIC_ERROR( "name clash" )
	CSL_TEST_END()

	CSL_TEST_BEGIN( "InterfaceTypeTests.AttributeAndMethodClashes.methodAndAttributeSetter" )
	CSL_EXPECT_SEMANTIC_ERROR( "name clash" )
	CSL_TEST_END()

	CSL_TEST( "InterfaceTypeTests.AttributeAndMethodClashes.methodAndAttributeSetter_valid" )
}

TEST( InterfaceTypeTests, clashBetweenSuperClasses )
{
	CSL_TEST_BEGIN( "InterfaceTypeTests.InheritanceTests.ClashBetweenSuperClasses.clashingParentsMembers" )
	CSL_EXPECT_SEMANTIC_ERROR( "name clash" )
	CSL_TEST_END()
}

TEST( InterfaceTypeTests, redundantInheritance )
{
	CSL_TEST_BEGIN( "InterfaceTypeTests.InheritanceTests.RedundantInheritance.InheritFromTheSameTypeTwice" )
	CSL_EXPECT_ERROR( "cannot inherit twice", "InheritFromTheSameTypeTwice.csl", 2 )
	CSL_TEST_END()

	CSL_TEST_BEGIN( "InterfaceTypeTests.InheritanceTests.RedundantInheritance.InheritFromSelf" )
	CSL_EXPECT_SEMANTIC_ERROR( "cyclic inheritance detected" )
	CSL_TEST_END()

	CSL_TEST_BEGIN( "InterfaceTypeTests.InheritanceTests.RedundantInheritance.SecondInheritanceIsSubtypeOftheFirst" )
	CSL_EXPECT_SEMANTIC_ERROR( "inherits more than once" )
	CSL_TEST_END()

	CSL_TEST_BEGIN( "InterfaceTypeTests.InheritanceTests.RedundantInheritance.FirstInheritanceIsSubtypeOftheSecond" )
	CSL_EXPECT_SEMANTIC_ERROR( "inherits more than once" )
	CSL_TEST_END()

	CSL_TEST_BEGIN( "InterfaceTypeTests.InheritanceTests.RedundantInheritance.HigherInheritanceCoorelation" )
	CSL_EXPECT_SEMANTIC_ERROR( "inherits more than once" )
	CSL_TEST_END()
}

TEST( InterfaceTypeTests, memberClashWithSuperType )
{
	CSL_TEST_BEGIN( "InterfaceTypeTests.InheritanceTests.MemberClashWithSuperType.InterfaceClashWithSuperType" )
	CSL_EXPECT_SEMANTIC_ERROR( "name clash" )
	CSL_TEST_END()

	// Test that made the semantic checks to run in a second pass.
	CSL_TEST_BEGIN( "InterfaceTypeTests.InheritanceTests.StartFromParent.ClashingParent" )
	CSL_EXPECT_SEMANTIC_ERROR( "name clash" )
	CSL_TEST_END()

	CSL_TEST_BEGIN( "InterfaceTypeTests.InheritanceTests.LargerInheritanceTree.UserClashWithPerson" )
	CSL_EXPECT_SEMANTIC_ERROR( "name clash" )
	CSL_TEST_END()
}

TEST( InterfaceTypeTests, firstOderInheritance )
{
	CSL_TEST( "InterfaceTypeTests.InheritanceTests.FirstOrder.Child" )

	CSL_TEST( "InterfaceTypeTests.InheritanceTests.FirstOrder.InheritanceOnly" )

	CSL_TEST( "InterfaceTypeTests.InheritanceTests.StartFromParent.Parent" )
}

TEST( InterfaceTypeTests, largeInheritanceTree )
{
	CSL_TEST( "InterfaceTypeTests.InheritanceTests.LargerInheritanceTree.User" )
}

TEST( InterfaceTypeTests, invalidDeclarations )
{
	CSL_TEST_BEGIN( "InterfaceTypeTests.InvalidDeclarations.emptyDeclaration" )
	CSL_EXPECT_SEMANTIC_ERROR( "missing interface contents" )
	CSL_TEST_END()

	CSL_TEST_BEGIN( "InterfaceTypeTests.InvalidDeclarations.emptyDeclarationWithComment" )
	CSL_EXPECT_SEMANTIC_ERROR( "missing interface contents" )
	CSL_TEST_END()

	CSL_TEST_BEGIN( "InterfaceTypeTests.InvalidDeclarations.invalidAttributeName" )
	CSL_EXPECT_ERROR( "must start with a lowercase letter", "invalidAttributeName.csl", 6 )
	CSL_TEST_END()

	CSL_TEST_BEGIN( "InterfaceTypeTests.InvalidDeclarations.missingParameterIO" )
	CSL_EXPECT_SYNTAX_ERROR( "missingParameterIO.csl", 5 )
	CSL_TEST_END()

	CSL_TEST_BEGIN( "InterfaceTypeTests.InvalidDeclarations.parameterTypeNotFound" )
	CSL_EXPECT_ERROR( "error loading dependency", "parameterTypeNotFound.csl", 4  )
	CSL_EXPECT_ERROR( "'IDontExixst' was not found", "", -1  )
	CSL_TEST_END()

	CSL_TEST_BEGIN( "InterfaceTypeTests.InvalidDeclarations.parameterNameClash" )
	CSL_EXPECT_ERROR( "parameter 'fooMe' defined twice", "parameterNameClash.csl", 4  )
	CSL_TEST_END()

	CSL_TEST_BEGIN( "InterfaceTypeTests.InvalidDeclarations.missingReturnType" )
	CSL_EXPECT_SYNTAX_ERROR( "missingReturnType.csl", 4 )
	CSL_TEST_END()

	CSL_TEST_BEGIN( "InterfaceTypeTests.InvalidDeclarations.inexistentReturnType" )
	CSL_EXPECT_ERROR( "error loading dependency", "inexistentReturnType.csl", 4 )
	CSL_EXPECT_ERROR( "was not found", "", -1 )
	CSL_TEST_END()

	CSL_TEST_BEGIN( "InterfaceTypeTests.InvalidDeclarations.inexistentRaisedException" )
	CSL_EXPECT_ERROR( "error loading exception type", "inexistentRaisedException.csl", 5 )
	CSL_EXPECT_ERROR( "was not found", "", -1 )
	CSL_TEST_END()

	CSL_TEST_BEGIN( "InterfaceTypeTests.InvalidDeclarations.missingReturnTypeAndMethodNameClash" )
	CSL_EXPECT_SYNTAX_ERROR( "missingReturnTypeAndMethodNameClash.csl", 4 )
	CSL_TEST_END()

	CSL_TEST_BEGIN( "InterfaceTypeTests.InvalidDeclarations.RaisesInvalidType" )
	CSL_EXPECT_ERROR( "attempt to raise non-exception type", "RaisesInvalidType.csl", 5  )
	CSL_TEST_END()
}

TEST( InterfaceTypeTests, validDeclarations )
{
	CSL_TEST( "InterfaceTypeTests.ValidDeclarations.validInterface" )
}

TEST( InterfaceTypeTests, cyclicInheritance )
{
	CSL_TEST_BEGIN( "InterfaceTypeTests.InheritanceTests.CyclicInheritance.Child" )
	CSL_EXPECT_SEMANTIC_ERROR( "cyclic inheritance detected" )
	CSL_TEST_END()
}

TEST( InterfaceTypeTests, multiplePathsToTheSameSuperType )
{
	CSL_TEST( "InterfaceTypeTests.InheritanceTests.MultiplePathsToSuperType.Child" )
	CSL_TEST( "InterfaceTypeTests.InheritanceTests.MultiplePathsToSuperType.BastardChild" )
}

TEST( InterfaceTypeTests, invalidParameter )
{
	CSL_TEST_BEGIN( "InterfaceTypeTests.InvalidDeclarations.invalidInterfaceWithComponentAsParameter" )
	CSL_EXPECT_ERROR( "components cannot be passed as parameters", "invalidInterfaceWithComponentAsParameter.csl", 6 )
	CSL_TEST_END()

	CSL_TEST_BEGIN( "InterfaceTypeTests.InvalidDeclarations.invalidInterfaceWithExceptionAsParameter" )
	CSL_EXPECT_ERROR( "exceptions cannot be passed as parameters", "invalidInterfaceWithExceptionAsParameter.csl", 6 )
	CSL_TEST_END()
}
