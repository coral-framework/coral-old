/*
 * Coral - A C++ Component Framework.
 * See Copyright Notice in Coral.h
 */

#include "CSLTester.h"
#include <co/Type.h>
#include <co/reserved/DependencyWalker.h>
#include <gtest/gtest.h>

class DependencyCollector : public co::IDependencyVisitor
{
public:
	void onTypeVisited( co::Type* type, co::uint32 distance )
	{
		CORAL_UNUSED( distance );
		const std::string& name = type->getFullName();
		typeNames.push_back( name );
	}

	std::vector<std::string> typeNames;
};

void printStringList( const std::vector<std::string>& strList )
{
	for( unsigned int i = 0; i < strList.size(); ++i )
		printf( "[%i] = '%s'\n", i, strList[i].c_str() );
}

TEST( DependencyWalkerTests, interfaceWalking )
{
	CSL_TEST( "DependencyWalkerTests.InterfaceWalkingTest.testInterface" );
	co::Type* type = TestHelper::type( "DependencyWalkerTests.InterfaceWalkingTest.testInterface" );

	DependencyCollector collector;
	co::DependencyWalker dw( type );
	dw.walk( &collector );

	//printStringList( collector.typeNames );

	ASSERT_EQ( 46, collector.typeNames.size() );

	// zero level, the type it self.
	ASSERT_EQ( "DependencyWalkerTests.InterfaceWalkingTest.testInterface", collector.typeNames[0] );

	//first level, its direct dependencies
	ASSERT_EQ( "DependencyWalkerTests.InterfaceWalkingTest.parentInterface", collector.typeNames[1] );
	ASSERT_EQ( "DependencyWalkerTests.InterfaceWalkingTest.Dependencies.anotherInterface", collector.typeNames[2] );
	ASSERT_EQ( "DependencyWalkerTests.InterfaceWalkingTest.Dependencies.yetAnotherInterface", collector.typeNames[3] );
	ASSERT_EQ( "DependencyWalkerTests.InterfaceWalkingTest.Dependencies.testEnum", collector.typeNames[4] );
	ASSERT_EQ( "DependencyWalkerTests.InterfaceWalkingTest.Dependencies.testStruct", collector.typeNames[5] );
	ASSERT_EQ( "DependencyWalkerTests.InterfaceWalkingTest.Dependencies.methodRetunType", collector.typeNames[6] );
	ASSERT_EQ( "DependencyWalkerTests.InterfaceWalkingTest.Dependencies.methodParameter", collector.typeNames[7] );
	ASSERT_EQ( "DependencyWalkerTests.InterfaceWalkingTest.testException", collector.typeNames[8] );
	ASSERT_EQ( "co.Interface", collector.typeNames[9] );

	// second level
	ASSERT_EQ( "DependencyWalkerTests.InterfaceWalkingTest.Dependencies.parentEnum", collector.typeNames[10] );
	ASSERT_EQ( "DependencyWalkerTests.InterfaceWalkingTest.Dependencies.DependencieOfYetAnotherInterface", collector.typeNames[11] );
	ASSERT_EQ( "DependencyWalkerTests.InterfaceWalkingTest.Dependencies.baseReturnType", collector.typeNames[12] );

	// third level
	ASSERT_EQ( "DependencyWalkerTests.InterfaceWalkingTest.Dependencies.valueType", collector.typeNames[13] );
	//  ... plus many types from namespace 'co', due to the 'co.Interface' interface
}

TEST( DependencyWalkerTests, walkingTypeSets )
{
	std::vector<co::Type*> typeSet;
	typeSet.push_back( co::getType( "DependencyWalkerTests.InterfaceWalkingTest.parentInterface" ) );
	typeSet.push_back( co::getType( "DependencyWalkerTests.InterfaceWalkingTest.Dependencies.anotherInterface" ) );
	typeSet.push_back( co::getType( "DependencyWalkerTests.InterfaceWalkingTest.Dependencies.yetAnotherInterface" ) );
	typeSet.push_back( co::getType( "DependencyWalkerTests.InterfaceWalkingTest.Dependencies.testEnum" ) );
	typeSet.push_back( co::getType( "DependencyWalkerTests.InterfaceWalkingTest.Dependencies.testStruct" ) );
	typeSet.push_back( co::getType( "DependencyWalkerTests.InterfaceWalkingTest.Dependencies.methodRetunType" ) );
	typeSet.push_back( co::getType( "DependencyWalkerTests.InterfaceWalkingTest.Dependencies.methodParameter" ) );
	typeSet.push_back( co::getType( "DependencyWalkerTests.InterfaceWalkingTest.testException" ) );
	typeSet.push_back( co::getType( "co.Interface" ) );

	DependencyCollector collector;
	co::DependencyWalker dw( typeSet.begin(), typeSet.end() );
	dw.walk( &collector );
	
	//printStringList( collector.typeNames );
	
	ASSERT_EQ( 45, collector.typeNames.size() );

	// zero level
	ASSERT_EQ( "DependencyWalkerTests.InterfaceWalkingTest.parentInterface", collector.typeNames[0] );
	ASSERT_EQ( "DependencyWalkerTests.InterfaceWalkingTest.Dependencies.anotherInterface", collector.typeNames[1] );
	ASSERT_EQ( "DependencyWalkerTests.InterfaceWalkingTest.Dependencies.yetAnotherInterface", collector.typeNames[2] );
	ASSERT_EQ( "DependencyWalkerTests.InterfaceWalkingTest.Dependencies.testEnum", collector.typeNames[3] );
	ASSERT_EQ( "DependencyWalkerTests.InterfaceWalkingTest.Dependencies.testStruct", collector.typeNames[4] );
	ASSERT_EQ( "DependencyWalkerTests.InterfaceWalkingTest.Dependencies.methodRetunType", collector.typeNames[5] );
	ASSERT_EQ( "DependencyWalkerTests.InterfaceWalkingTest.Dependencies.methodParameter", collector.typeNames[6] );
	ASSERT_EQ( "DependencyWalkerTests.InterfaceWalkingTest.testException", collector.typeNames[7] );
	ASSERT_EQ( "co.Interface", collector.typeNames[8] );
	
	// second level
	ASSERT_EQ( "DependencyWalkerTests.InterfaceWalkingTest.Dependencies.parentEnum", collector.typeNames[9] );
	ASSERT_EQ( "DependencyWalkerTests.InterfaceWalkingTest.Dependencies.DependencieOfYetAnotherInterface", collector.typeNames[10] );
	ASSERT_EQ( "DependencyWalkerTests.InterfaceWalkingTest.Dependencies.baseReturnType", collector.typeNames[11] );
	
	// third level
	ASSERT_EQ( "DependencyWalkerTests.InterfaceWalkingTest.Dependencies.valueType", collector.typeNames[12] );
	//  ... plus many types from namespace 'co', due to the 'co.Interface' interface
}

TEST( DependencyWalkerTests, depthLimit )
{
	co::Type* type = TestHelper::type( "DependencyWalkerTests.InterfaceWalkingTest.testInterface" );
	if( type == NULL )
	{
		CSL_TEST( "DependencyWalkerTests.InterfaceWalkingTest.testInterface" );
		type = TestHelper::type( "DependencyWalkerTests.InterfaceWalkingTest.testInterface" );
	}

	DependencyCollector firstLevelCollector;
	co::DependencyWalker dw( type );
	dw.walk( &firstLevelCollector, 1 );

	ASSERT_EQ( 9, firstLevelCollector.typeNames.size() );

	// zero level, the type it self.
	ASSERT_EQ( "DependencyWalkerTests.InterfaceWalkingTest.testInterface", firstLevelCollector.typeNames[0] );

	//first level, its direct dependencies
	ASSERT_EQ( "DependencyWalkerTests.InterfaceWalkingTest.parentInterface", firstLevelCollector.typeNames[1] );
	ASSERT_EQ( "DependencyWalkerTests.InterfaceWalkingTest.Dependencies.anotherInterface", firstLevelCollector.typeNames[2] );
	ASSERT_EQ( "DependencyWalkerTests.InterfaceWalkingTest.Dependencies.yetAnotherInterface", firstLevelCollector.typeNames[3] );
	ASSERT_EQ( "DependencyWalkerTests.InterfaceWalkingTest.Dependencies.testEnum", firstLevelCollector.typeNames[4] );
	ASSERT_EQ( "DependencyWalkerTests.InterfaceWalkingTest.Dependencies.testStruct", firstLevelCollector.typeNames[5] );
	ASSERT_EQ( "DependencyWalkerTests.InterfaceWalkingTest.Dependencies.methodRetunType", firstLevelCollector.typeNames[6] );
	ASSERT_EQ( "DependencyWalkerTests.InterfaceWalkingTest.Dependencies.methodParameter", firstLevelCollector.typeNames[7] );

}

TEST( DependencyWalkerTests, cyclicDependencies )
{
	CSL_TEST( "DependencyWalkerTests.CyclicDependencyWalkTest.child" );
	co::Type* type = TestHelper::type( "DependencyWalkerTests.CyclicDependencyWalkTest.child" );

	DependencyCollector collector;
	co::DependencyWalker dw( type );
	dw.walk( &collector );

	ASSERT_EQ( 35, collector.typeNames.size() );

	ASSERT_EQ( "DependencyWalkerTests.CyclicDependencyWalkTest.child", collector.typeNames[0] );
	ASSERT_EQ( "DependencyWalkerTests.CyclicDependencyWalkTest.parent", collector.typeNames[1] );
}

TEST( DependencyWalkerTests, componentWalking )
{
	CSL_TEST( "DependencyWalkerTests.ComponentWalkTest.testComponent" );
	co::Type* type = TestHelper::type( "DependencyWalkerTests.ComponentWalkTest.testComponent" );

	DependencyCollector collector;
	co::DependencyWalker dw( type );
	dw.walk( &collector );
	
	//printStringList( collector.typeNames );

	ASSERT_EQ( 37, collector.typeNames.size() );

	// zero level, the type it self.
	ASSERT_EQ( "DependencyWalkerTests.ComponentWalkTest.testComponent", collector.typeNames[0] );

	// first level, component interfaces;
	ASSERT_EQ( "DependencyWalkerTests.ComponentWalkTest.requiredInterface", collector.typeNames[1] );
	ASSERT_EQ( "DependencyWalkerTests.ComponentWalkTest.providedInterface", collector.typeNames[2] );
	ASSERT_EQ( "co.Interface", collector.typeNames[3] );

	// second level, interfaces dependent types;
	ASSERT_EQ( "DependencyWalkerTests.ComponentWalkTest.child", collector.typeNames[4] );
	//  ... plus many types from namespace 'co', due to the 'co.Interface' interface
}
