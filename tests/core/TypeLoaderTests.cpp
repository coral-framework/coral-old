/*
 * Coral - Lightweight C++ Component Framework
 * See Copyright Notice in Coral.h
 */

#include "CSLTester.h"
#include <co/ISystem.h>
#include <co/ITypeManager.h>
#include <co/IInterface.h>
#include <co/IField.h>
#include <gtest/gtest.h>

TEST( TypeLoaderTests, windowsLineBreaks )
{
	CSL_TEST( "TypeLoaderTests.WindowsLineBreaks" )
}

TEST( TypeLoaderTests, namespaceScope )
{
	CSL_TEST( "TypeLoaderTests.ImportClauseTests.Office" )
}

TEST( TypeLoaderTests, clashingImports )
{
	CSL_TEST_BEGIN( "TypeLoaderTests.ImportClauseTests.ClashingImports" )
	CSL_EXPECT_ERROR( "conflicts with a previous import", "ClashingImports.csl", 2 )
	CSL_TEST_END()
}

TEST( TypeLoaderTests, validImport )
{
	CSL_TEST( "TypeLoaderTests.ImportClauseTests.ValidImports" );
	co::IInterface* designer = co::cast<co::IInterface>( TestHelper::type( "TypeLoaderTests.ImportClauseTests.Roles.Designer") );

	ASSERT_TRUE( designer != NULL );

	co::IField* field = co::cast<co::IField>( designer->getMember( "mainOffice" ) );

	ASSERT_TRUE( field != NULL );

	EXPECT_EQ( "TypeLoaderTests.ImportClauseTests.Office", field->getType()->getFullName() );
}

TEST( TypeLoaderTests, sameNamespaceImport )
{
	CSL_TEST_BEGIN( "TypeLoaderTests.ImportClauseTests.SameNamespaceImport" )
	CSL_EXPECT_ERROR( "is in the same namespace and does not require importing", "SameNamespaceImport.csl", 1 )
	CSL_TEST_END()
}

TEST( TypeLoaderTests, cyclicalImport )
{
	CSL_TEST( "TypeLoaderTests.ImportClauseTests.Chicken" )
}

TEST( TypeLoaderTests, syntaxError )
{
	CSL_TEST_BEGIN( "TypeLoaderTests.SyntaxError.struct1" )
	CSL_EXPECT_ERROR( "syntax error near 'structt'", "struct1.csl", 1 )
	CSL_TEST_END()

	CSL_TEST_BEGIN( "TypeLoaderTests.SyntaxError.struct2" )
	CSL_EXPECT_ERROR( "syntax error near '1'", "struct2.csl", 1 )
	CSL_TEST_END()

	CSL_TEST_BEGIN( "TypeLoaderTests.SyntaxError.struct3" )
	CSL_EXPECT_ERROR( "syntax error near ';'", "struct3.csl", 4 )
	CSL_TEST_END()

	CSL_TEST_BEGIN( "TypeLoaderTests.SyntaxError.struct4" )
	CSL_EXPECT_ERROR( "syntax error near '// 7th line'", "struct4.csl", 7 )
	CSL_TEST_END()

	CSL_TEST_BEGIN( "TypeLoaderTests.SyntaxError.struct5" )
	CSL_EXPECT_ERROR( "syntax error near 'string'", "struct5.csl", 3)
	CSL_TEST_END()

	CSL_TEST_BEGIN( "TypeLoaderTests.SyntaxError.struct6" )
	CSL_EXPECT_ERROR( "unknown character '%' (0x25)", "struct6.csl", 3 )
	CSL_TEST_END()
}

inline const std::string& getDoc( const char* typeOrMemberName )
{
	return co::getSystem()->getTypes()->getDocumentation( typeOrMemberName );
}

TEST( TypeLoaderTests, interfaceDocs )
{
	CSL_TEST( "TypeLoaderTests.SingleFileDocMapLoading.DocumentedInterface" );

	const std::string& interfaceDoc = getDoc( "TypeLoaderTests.SingleFileDocMapLoading.DocumentedInterface" );
	const std::string& nameDoc = getDoc( "TypeLoaderTests.SingleFileDocMapLoading.DocumentedInterface:name" );
	const std::string& fooDoc = getDoc( "TypeLoaderTests.SingleFileDocMapLoading.DocumentedInterface:foo" );

	EXPECT_EQ( "This is the interface Declaration\nThis could be a multi-line comment", interfaceDoc );
	EXPECT_EQ( "and this is the field doc.", nameDoc );
	EXPECT_EQ( "foo method declaration.\nusing separated.\ndocumentation lines.", fooDoc );
}

TEST( TypeLoaderTests, structDocs )
{
	CSL_TEST( "TypeLoaderTests.SingleFileDocMapLoading.DocumentedStruct" );

	const std::string& structDoc = getDoc( "TypeLoaderTests.SingleFileDocMapLoading.DocumentedStruct" );
	const std::string& nameDoc = getDoc( "TypeLoaderTests.SingleFileDocMapLoading.DocumentedStruct:name" );
	const std::string& fooDoc = getDoc( "TypeLoaderTests.SingleFileDocMapLoading.DocumentedStruct:foo" );

	EXPECT_EQ( "Struct Declaration", structDoc );
	EXPECT_EQ( "struct member.\nextra doc.", nameDoc );
	EXPECT_EQ( "postDoc.", fooDoc );
}

TEST( TypeLoaderTests, enumDocs )
{
	CSL_TEST( "TypeLoaderTests.SingleFileDocMapLoading.DocumentedEnum" );

	const std::string& enumDoc = getDoc( "TypeLoaderTests.SingleFileDocMapLoading.DocumentedEnum" );
	const std::string& firstDoc = getDoc( "TypeLoaderTests.SingleFileDocMapLoading.DocumentedEnum:first" );
	const std::string& secondDoc = getDoc( "TypeLoaderTests.SingleFileDocMapLoading.DocumentedEnum:second" );
	const std::string& thirdDoc = getDoc( "TypeLoaderTests.SingleFileDocMapLoading.DocumentedEnum:third" );

	EXPECT_EQ( "This is the Enum Declaration", enumDoc );
	EXPECT_EQ( "doc for first.\nsecond doc for first.", firstDoc );
	EXPECT_EQ( "doc for second.", secondDoc );
	EXPECT_TRUE( thirdDoc.empty() );
}

TEST( TypeLoaderTests, componentDocs )
{
	CSL_TEST( "TypeLoaderTests.SingleFileDocMapLoading.DocumentedComponent" );

	const std::string& componentDoc = getDoc( "TypeLoaderTests.SingleFileDocMapLoading.DocumentedComponent" );
	const std::string& providedDoc = getDoc( "TypeLoaderTests.SingleFileDocMapLoading.DocumentedComponent:providedInterface" );
	const std::string& requiredDoc = getDoc( "TypeLoaderTests.SingleFileDocMapLoading.DocumentedComponent:requiredInterface" );
	const std::string& notDoc = getDoc( "TypeLoaderTests.SingleFileDocMapLoading.DocumentedComponent:providedInterfaceNotDocumented" );

	EXPECT_EQ( "Component Declaration", componentDoc );
	EXPECT_EQ( "provided", providedDoc );
	EXPECT_EQ( "Required interface", requiredDoc );
	EXPECT_TRUE( notDoc.empty() );
}

TEST( TypeLoaderTests, dependencyDocs )
{
	CSL_TEST( "TypeLoaderTests.DependencyDocMapLoading.StartingType" );

	EXPECT_FALSE( getDoc( "TypeLoaderTests.DependencyDocMapLoading.StartingType" ).empty() );
	EXPECT_FALSE( getDoc( "TypeLoaderTests.DependencyDocMapLoading.StartingType:dep" ).empty() );
	EXPECT_FALSE( getDoc( "TypeLoaderTests.DependencyDocMapLoading.DependencyType" ).empty() );
	EXPECT_FALSE( getDoc( "TypeLoaderTests.DependencyDocMapLoading.DependencyType:name" ).empty() );
}

TEST( TypeLoaderTests, nestedErrors )
{
	CSL_TEST_BEGIN( "TypeLoaderTests.NestedErrors.Struct1" )
	CSL_EXPECT_ERROR( "error loading dependency 'Struct2'", "Struct1.csl", 3 )
	CSL_EXPECT_ERROR( "error loading dependency 'Struct3'", "Struct2.csl", 3 )
	CSL_EXPECT_ERROR( "error loading dependency 'Struct4'", "Struct3.csl", 3 )
	CSL_EXPECT_ERROR( "error loading dependency 'Struct5'", "Struct4.csl", 3 )
	CSL_EXPECT_ERROR( "was not found", "", 3 )
	CSL_TEST_END()
}

TEST( TypeLoaderTests, fileNameDivergence )
{
	CSL_TEST_BEGIN( "TypeLoaderTests.FileNameDivergence.diferentName" )
	CSL_EXPECT_ERROR( "the name of the defined type must match its filename", "diferentName.csl", 2 )
	CSL_TEST_END()

	CSL_TEST_BEGIN( "TypeLoaderTests.FileNameDivergence.diferentCaseName" )
	CSL_EXPECT_ERROR( "the name of the defined type must match its filename", "diferentCaseName.csl", 2 )
	CSL_TEST_END()
}

TEST( TypeLoaderTests, multipleSpecifications )
{
	CSL_TEST_BEGIN( "TypeLoaderTests.InvalidDeclarationTests.multipleTypes" )
	CSL_EXPECT_ERROR( "only one type specification is allowed per file", "multipleTypes.csl", 8 )
	CSL_TEST_END()
}

TEST( TypeLoaderTests, inexistantArrayElementType )
{
	CSL_TEST_BEGIN( "TypeLoaderTests.InvalidDeclarationTests.inexistantArrayElementType" )
	CSL_EXPECT_ERROR( "error loading array element type", "inexistantArrayElementType.csl", 4 )
	CSL_EXPECT_ERROR( "'IDontExist' was not found", "", 4 )
	CSL_TEST_END()
}

TEST( TypeLoaderTests, arrayTypeAutoCreation )
{
	CSL_TEST( "TypeLoaderTests.ValidDeclarationTests.arrayTypeAutoCreation" );
	EXPECT_TRUE( TestHelper::type( "TypeLoaderTests.ValidDeclarationTests.BaseClasses.ArrayElement[]" ) != NULL );
}

TEST( TypeLoaderTests, inexistantType )
{
	CSL_TEST_BEGIN( "TypeLoaderTests.InexistantType" )
	CSL_EXPECT_ERROR( "'TypeLoaderTests.InexistantType' was not found", "", -1 )
	CSL_TEST_END()
}

TEST( TypeLoaderTests, importAfterTypeSpecification )
{
	CSL_TEST_BEGIN( "TypeLoaderTests.InvalidDeclarationTests.includeAfterTypeSpecifications" )
	CSL_EXPECT_ERROR( "import clauses must come before the type specification", "includeAfterTypeSpecifications.csl", 7 )
	CSL_TEST_END()
}

TEST( TypeLoaderTests, singleCppBlock )
{
	CSL_TEST( "TypeLoaderTests.CppCodeBlocksTests.singleBlockInterface" );

	co::IInterface* it = co::cast<co::IInterface>( co::getType( "TypeLoaderTests.CppCodeBlocksTests.singleBlockInterface" ) );
	ASSERT_TRUE( it != NULL );

	const std::string& str = it->getCppBlock();
	EXPECT_TRUE( str.find( "\t//This code block was injected by the compiler using the '<c++' tag" ) != std::string::npos );
	EXPECT_TRUE( str.find( "\tvoid myInjectedFoo() {;}" ) != std::string::npos );
}

TEST( TypeLoaderTests, multipleCppBlocks )
{
	CSL_TEST( "TypeLoaderTests.CppCodeBlocksTests.multipleBlocksInterface" );

	co::IInterface* it = co::cast<co::IInterface>( co::getType( "TypeLoaderTests.CppCodeBlocksTests.multipleBlocksInterface" ) );
	ASSERT_TRUE( it != NULL );

	const std::string& str = it->getCppBlock();
	EXPECT_TRUE( str.find( "\tvoid myInjectedFoo() {;}" ) != std::string::npos );
	EXPECT_TRUE( str.find( "\tvoid myLastInjectedFoo() {;}" ) != std::string::npos );
}
