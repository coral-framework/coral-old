/*
 * Coral - A C++ Component Framework.
 * See Copyright Notice in Coral.h
 */

#include "CSLTester.h"
#include <co/InterfaceType.h>
#include <co/AttributeInfo.h>
#include <gtest/gtest.h>

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
	co::InterfaceType* designer = dynamic_cast<co::InterfaceType*>( TestHelper::type( "TypeLoaderTests.ImportClauseTests.Roles.Designer") );

	ASSERT_TRUE( designer != NULL );

	co::AttributeInfo* attribute = dynamic_cast<co::AttributeInfo*>( designer->getMember( "mainOffice" ) );

	ASSERT_TRUE( attribute != NULL );

	EXPECT_EQ( "TypeLoaderTests.ImportClauseTests.Office", attribute->getType()->getFullName() );
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
	CSL_EXPECT_SYNTAX_ERROR( "struct1.csl", 1 )
	CSL_TEST_END()

	CSL_TEST_BEGIN( "TypeLoaderTests.SyntaxError.struct2" )
	CSL_EXPECT_SYNTAX_ERROR( "struct2.csl", 1 )
	CSL_TEST_END()

	CSL_TEST_BEGIN( "TypeLoaderTests.SyntaxError.struct3" )
	CSL_EXPECT_SYNTAX_ERROR( "struct3.csl", 3 )
	CSL_TEST_END()

	CSL_TEST_BEGIN( "TypeLoaderTests.SyntaxError.struct4" )
	CSL_EXPECT_SYNTAX_ERROR( "struct4.csl", 4 )
	CSL_TEST_END()

	CSL_TEST_BEGIN( "TypeLoaderTests.SyntaxError.struct5" )
	CSL_EXPECT_SYNTAX_ERROR( "struct5.csl", 3)
	CSL_TEST_END()

	CSL_TEST_BEGIN( "TypeLoaderTests.SyntaxError.struct6" )
	CSL_EXPECT_SYNTAX_ERROR( "struct6.csl", 3 )
	CSL_TEST_END()
}

#define DECL_TYPE_LOADER( varName, typeName ) \
	co::TypeLoader varName( typeName, co::getPaths(), co::getSystem()->getTypes() )

typedef co::TypeLoader::DocMap DocMap;
const DocMap& docMap = co::TypeLoader::getDocMap();

TEST( TypeLoaderTests, interfaceDocMapLoading )
{
	DECL_TYPE_LOADER( loader, "TypeLoaderTests.SingleFileDocMapLoading.DocumentedInterface" );
	loader.loadType();

	ASSERT_TRUE( loader.getError() == NULL ) << loader.getError()->getMessage();

	// check if the members are all in the map.
	DocMap::const_iterator interfaceDoc = docMap.find( "TypeLoaderTests.SingleFileDocMapLoading.DocumentedInterface" );
	DocMap::const_iterator nameDoc = docMap.find( "TypeLoaderTests.SingleFileDocMapLoading.DocumentedInterface:name" );
	DocMap::const_iterator fooDoc = docMap.find( "TypeLoaderTests.SingleFileDocMapLoading.DocumentedInterface:foo" );

	ASSERT_TRUE( interfaceDoc != docMap.end() );
	ASSERT_TRUE( nameDoc != docMap.end() );
	ASSERT_TRUE( fooDoc != docMap.end() );

	EXPECT_EQ( "// This is the interface Declaration\n/* \n\tThat could be a multi-line comment\n*/", interfaceDoc->second );
	EXPECT_EQ( "// and this is the attribute doc.\n", nameDoc->second );
	EXPECT_EQ( "// foo method declaration.\n// using separated.\n/* documentation lines.*/", fooDoc->second );
}

TEST( TypeLoaderTests, structDocMapLoading )
{
	DECL_TYPE_LOADER( loader, "TypeLoaderTests.SingleFileDocMapLoading.DocumentedStruct" );
	loader.loadType();

	ASSERT_TRUE( loader.getError() == NULL ) << loader.getError()->getMessage();

	// check if the members are all in the map.
	DocMap::const_iterator structDoc = docMap.find( "TypeLoaderTests.SingleFileDocMapLoading.DocumentedStruct" );
	DocMap::const_iterator nameDoc = docMap.find( "TypeLoaderTests.SingleFileDocMapLoading.DocumentedStruct:name" );
	DocMap::const_iterator fooDoc = docMap.find( "TypeLoaderTests.SingleFileDocMapLoading.DocumentedStruct:foo" );

	ASSERT_TRUE( structDoc != docMap.end() );
	ASSERT_TRUE( nameDoc != docMap.end() );
	ASSERT_TRUE( fooDoc != docMap.end() );

	EXPECT_EQ( "//Struct Declaration\n", structDoc->second );
	EXPECT_EQ( "// struct member.\n//< extra doc.\n", nameDoc->second );
	EXPECT_EQ( "//< postDoc.\n", fooDoc->second );
}

TEST( TypeLoaderTests, enumDocMapLoading )
{
	DECL_TYPE_LOADER( loader, "TypeLoaderTests.SingleFileDocMapLoading.DocumentedEnum" );
	loader.loadType();

	ASSERT_TRUE( loader.getError() == NULL ) << loader.getError()->getMessage();

	// check if the members are all in the map.
	DocMap::const_iterator enumDoc = docMap.find( "TypeLoaderTests.SingleFileDocMapLoading.DocumentedEnum" );
	DocMap::const_iterator firstDoc = docMap.find( "TypeLoaderTests.SingleFileDocMapLoading.DocumentedEnum:first" );
	DocMap::const_iterator secondDoc = docMap.find( "TypeLoaderTests.SingleFileDocMapLoading.DocumentedEnum:second" );
	DocMap::const_iterator thirdDoc = docMap.find( "TypeLoaderTests.SingleFileDocMapLoading.DocumentedEnum:third" );

	ASSERT_TRUE( enumDoc != docMap.end() );
	ASSERT_TRUE( firstDoc != docMap.end() );
	ASSERT_TRUE( secondDoc != docMap.end() );
	ASSERT_TRUE( thirdDoc == docMap.end() );

	EXPECT_EQ( "// This is the Enum Declaration\n", enumDoc->second );
	EXPECT_EQ( "// doc for first.\n//< second doc for first.\n", firstDoc->second );
	EXPECT_EQ( "//< doc for second.\n", secondDoc->second );
}

TEST( TypeLoaderTests, componentDocMapLoading )
{
	DECL_TYPE_LOADER( loader, "TypeLoaderTests.SingleFileDocMapLoading.DocumentedComponent" );
	loader.loadType();

	ASSERT_TRUE( loader.getError() == NULL ) << loader.getError()->getMessage();

	// check if the members are all in the map.
	DocMap::const_iterator componentDoc = docMap.find( "TypeLoaderTests.SingleFileDocMapLoading.DocumentedComponent" );
	DocMap::const_iterator providedDoc = docMap.find( "TypeLoaderTests.SingleFileDocMapLoading.DocumentedComponent:providedInterface" );
	DocMap::const_iterator requiredDoc = docMap.find( "TypeLoaderTests.SingleFileDocMapLoading.DocumentedComponent:requiredInterface" );
	DocMap::const_iterator notDoc = docMap.find( "TypeLoaderTests.SingleFileDocMapLoading.DocumentedComponent:providedInterfaceNotDocumented" );

	ASSERT_TRUE( componentDoc != docMap.end() );
	ASSERT_TRUE( providedDoc != docMap.end() );
	ASSERT_TRUE( requiredDoc != docMap.end() );
	ASSERT_TRUE( notDoc == docMap.end() );

	EXPECT_EQ( "//Component Declaration\n", componentDoc->second );
	EXPECT_EQ( "// provided\n", providedDoc->second );
	EXPECT_EQ( "//< Required interface\n", requiredDoc->second );
}

TEST( TypeLoaderTests, dependencyDocMapLoading )
{
	DECL_TYPE_LOADER( loader, "TypeLoaderTests.DependencyDocMapLoading.StartingType" );
	loader.loadType();

	ASSERT_TRUE( loader.getError() == NULL ) << loader.getError()->getMessage();

	// check if the members are all in the map.
	EXPECT_TRUE( docMap.find( "TypeLoaderTests.DependencyDocMapLoading.StartingType" ) != docMap.end() );
	EXPECT_TRUE( docMap.find( "TypeLoaderTests.DependencyDocMapLoading.StartingType:dep" ) != docMap.end() );
	EXPECT_TRUE( docMap.find( "TypeLoaderTests.DependencyDocMapLoading.DependencyType" ) != docMap.end() );
	EXPECT_TRUE( docMap.find( "TypeLoaderTests.DependencyDocMapLoading.DependencyType:name" ) != docMap.end() );
}

TEST( TypeLoaderTests, nestedErrors )
{
	CSL_TEST_BEGIN( "TypeLoaderTests.NestedErrors.Struct1" )
	CSL_EXPECT_ERROR( "error loading dependency 'Struct2'", "Struct1.csl", 3 )
	CSL_EXPECT_ERROR( "error loading dependency 'Struct3'", "Struct2.csl", 3 )
	CSL_EXPECT_ERROR( "error loading dependency 'Struct4'", "Struct3.csl", 3 )
	CSL_EXPECT_ERROR( "error loading dependency 'Struct5'", "Struct4.csl", 3 )
	CSL_EXPECT_ERROR( "was not found", "", -1 )
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
	CSL_EXPECT_ERROR( "'IDontExist' was not found", "", -1 )
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

typedef co::TypeLoader::CppBlockMap CppBlockMap;
const CppBlockMap& cppBlockMap = co::TypeLoader::getCppBlockMap();

TEST( TypeLoaderTests, singleCppBlock )
{
	DECL_TYPE_LOADER( loader, "TypeLoaderTests.CppCodeBlocksTests.singleBlockInterface" );
	loader.loadType();

	ASSERT_TRUE( loader.getError() == NULL ) << loader.getError()->getMessage();

	// check if the members are all in the map.
	CppBlockMap::const_iterator interfaceCppBlock = cppBlockMap.find( "TypeLoaderTests.CppCodeBlocksTests.singleBlockInterface" );

	ASSERT_TRUE( interfaceCppBlock != cppBlockMap.end() );

	EXPECT_EQ( "\n\t//This code block was injected by the compiler using the '<c++' tag\n\tvoid myInjectedFoo() {;}\n\t", interfaceCppBlock->second );
}

TEST( TypeLoaderTests, multipleCppBlocks )
{
	DECL_TYPE_LOADER( loader, "TypeLoaderTests.CppCodeBlocksTests.multipleBlocksInterface" );
	loader.loadType();

	ASSERT_TRUE( loader.getError() == NULL ) << loader.getError()->getMessage();

	// check if the members are all in the map.
	CppBlockMap::const_iterator interfaceCppBlock = cppBlockMap.find( "TypeLoaderTests.CppCodeBlocksTests.multipleBlocksInterface" );

	ASSERT_TRUE( interfaceCppBlock != cppBlockMap.end() );

	EXPECT_EQ( "\n\tvoid myInjectedFoo() {;}\n\t\n\tvoid myLastInjectedFoo() {;}\n\t", interfaceCppBlock->second );
}
