/*
 * Coral - A C++ Component Framework.
 * See Copyright Notice in Coral.h
 */

#include <co/Type.h>
#include <co/Coral.h>
#include <co/System.h>
#include <co/CSLError.h>
#include <co/ArrayType.h>
#include <co/Namespace.h>
#include <co/TypeManager.h>
#include <co/TypeLoadException.h>
#include <co/IllegalArgumentException.h>
#include <gtest/gtest.h>

TEST( TypeManagerTests, preDefinedTypes )
{
	co::TypeManager* tm = co::getSystem()->getTypes();

	// these are global predefined types:
	EXPECT_TRUE( tm->findType( "any" ) != NULL );
	EXPECT_TRUE( tm->findType( "bool" ) != NULL );
	EXPECT_TRUE( tm->findType( "int8" ) != NULL );
	EXPECT_TRUE( tm->findType( "uint8" ) != NULL );
	EXPECT_TRUE( tm->findType( "int16" ) != NULL );
	EXPECT_TRUE( tm->findType( "uint16" ) != NULL );
	EXPECT_TRUE( tm->findType( "int32" ) != NULL );
	EXPECT_TRUE( tm->findType( "uint32" ) != NULL );
	EXPECT_TRUE( tm->findType( "int64" ) != NULL );
	EXPECT_TRUE( tm->findType( "uint64" ) != NULL );
	EXPECT_TRUE( tm->findType( "float" ) != NULL );
	EXPECT_TRUE( tm->findType( "double" ) != NULL );
	EXPECT_TRUE( tm->findType( "string" ) != NULL );

	// these are NOT predefined types:
	EXPECT_TRUE( tm->findType( "void" ) == NULL );
}

TEST( TypeManagerTests, findType )
{
	co::TypeManager* tm = co::getSystem()->getTypes();

	EXPECT_TRUE( tm->findType( "TypeManagerTests.BarStruct" ) == NULL );
	EXPECT_TRUE( tm->findType( "TypeManagerTests.IFoo" ) == NULL );

	EXPECT_TRUE( tm->getType( "TypeManagerTests.BarStruct" ) != NULL );

	EXPECT_TRUE( tm->findType( "TypeManagerTests.BarStruct" ) != NULL );
	EXPECT_TRUE( tm->findType( "TypeManagerTests.IFoo" ) != NULL );
}

TEST( TypeManagerTests, getType )
{
	co::TypeManager* tm = co::getSystem()->getTypes();

	EXPECT_TRUE( tm->getType( "co.Uuid" ) != NULL );

	EXPECT_THROW( tm->getType( "" ), co::IllegalArgumentException );
	EXPECT_THROW( tm->getType( "TypeManagerTests.Unexistent" ), co::TypeLoadException );
}

TEST( TypeManagerTests, arrays )
{
	co::TypeManager* tm = co::getSystem()->getTypes();

	EXPECT_TRUE( tm->findType( "TypeManagerTests.IFoo[]" ) == NULL );
	EXPECT_TRUE( tm->getType( "TypeManagerTests.IFoo[]" ) != NULL );
	EXPECT_TRUE( tm->findType( "TypeManagerTests.IFoo[]" ) != NULL );

	EXPECT_TRUE( tm->findType( "TypeManagerTests.BarStruct[]" ) == NULL );
	EXPECT_EQ( tm->getArrayOf( tm->getType( "TypeManagerTests.BarStruct" ) ),
				static_cast<co::ArrayType*>( tm->getType( "TypeManagerTests.BarStruct[]" ) ) );
	EXPECT_TRUE( tm->findType( "TypeManagerTests.BarStruct[]" ) != NULL );

	EXPECT_THROW( tm->getArrayOf( NULL ), co::IllegalArgumentException );
	EXPECT_THROW( tm->getType( "TypeManagerTests.BarStruct[][]" ), co::IllegalArgumentException );
	EXPECT_THROW( tm->getType( "co.IllegalArgumentException[]" ), co::IllegalArgumentException );
	EXPECT_THROW( tm->getType( "TypeManagerTests.FooComponent[]" ), co::IllegalArgumentException );
}

TEST( TypeManagerTests, loadType )
{
	co::TypeManager* tm = co::getSystem()->getTypes();

	EXPECT_TRUE( tm->loadType( "", NULL ) == NULL );

	EXPECT_TRUE( tm->loadType( "int32", NULL )->getKind() == co::TK_INT32 );

	EXPECT_TRUE( tm->loadType( "TypeManagerTests.ErroneousStruct", NULL ) == NULL );

	std::vector<co::CSLError> errorStack;
	EXPECT_TRUE( tm->loadType( "TypeManagerTests.ErroneousStruct", &errorStack ) == NULL );

	EXPECT_EQ( 2, errorStack.size() );

	EXPECT_EQ( 3, errorStack[0].line );
	EXPECT_EQ( "error loading dependency 'ErroneousStruct2'", errorStack[0].message );

	EXPECT_EQ( 3, errorStack[1].line );
	EXPECT_EQ( "syntax error: cannot match to any predicted input", errorStack[1].message );
}

struct TypeStatistics
{
	co::uint32 numNamespaces;
	co::uint32 numTypes;
	co::uint32 numComplexTypes; // # of structs, native classes, interfaces and components
	co::uint32 numTypesWithReflector;

	TypeStatistics()
		: numNamespaces( 0 ), numTypes( 0 ), numComplexTypes( 0 ), numTypesWithReflector( 0 )
	{;}

	void traverse( co::Namespace* ns )
	{
		++numNamespaces;

		for( co::ArrayRange<co::Type* const> r( ns->getTypes() ); r; r.popFirst() )
		{
			++numTypes;

			co::Type* type = r.getFirst();
			if( type->getKind() > co::TK_EXCEPTION )
				++numComplexTypes;

			if( type->getReflector() )
			   ++numTypesWithReflector;
		}

		for( co::ArrayRange<co::Namespace* const> r( ns->getChildNamespaces() ); r; r.popFirst() )
			traverse( r.getFirst() );
	}
};

TEST( TypeManagerTests, defaultTypeChecks )
{
	TypeStatistics stats;
	stats.traverse( co::getSystem()->getTypes()->findNamespace( "co" ) );
	EXPECT_EQ( stats.numTypes, stats.numTypesWithReflector );
}
