/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#include <co/Coral.h>
#include <co/RefPtr.h>
#include <co/TypeManager.h>
#include <co/reserved/LexicalUtilities.h>
#include <gtest/gtest.h>

TEST( LexicalUtilitiesTests, isValidIdentifier )
{
	ASSERT_TRUE( co::LexicalUtilities::isValidIdentifier( "A" ) );
	ASSERT_TRUE( co::LexicalUtilities::isValidIdentifier( "a" ) );
	ASSERT_TRUE( co::LexicalUtilities::isValidIdentifier( "Ab_cd" ) );
	ASSERT_TRUE( co::LexicalUtilities::isValidIdentifier( "d123" ) );
	ASSERT_TRUE( co::LexicalUtilities::isValidIdentifier( "lalalalalala" ) );

	ASSERT_FALSE( co::LexicalUtilities::isValidIdentifier( "" ) );
	ASSERT_FALSE( co::LexicalUtilities::isValidIdentifier( "1lalla" ) );
	ASSERT_FALSE( co::LexicalUtilities::isValidIdentifier( " lalala" ) );
	ASSERT_FALSE( co::LexicalUtilities::isValidIdentifier( "lalala " ) );
	ASSERT_FALSE( co::LexicalUtilities::isValidIdentifier( "lalala!" ) );
	ASSERT_FALSE( co::LexicalUtilities::isValidIdentifier( "lalala lelele" ) );
	ASSERT_FALSE( co::LexicalUtilities::isValidIdentifier( " " ) );
	ASSERT_FALSE( co::LexicalUtilities::isValidIdentifier( "." ) );
}

TEST( LexicalUtilitiesTests, isValidAttribute )
{
	ASSERT_TRUE( co::LexicalUtilities::isValidAttributeName( "a" ) );
	ASSERT_TRUE( co::LexicalUtilities::isValidAttributeName( "ala" ) );
	ASSERT_TRUE( co::LexicalUtilities::isValidAttributeName( "ab_cd" ) );
	ASSERT_TRUE( co::LexicalUtilities::isValidAttributeName( "d123" ) );
	ASSERT_TRUE( co::LexicalUtilities::isValidAttributeName( "lalalalalala" ) );

	ASSERT_FALSE( co::LexicalUtilities::isValidAttributeName( "" ) );
	ASSERT_FALSE( co::LexicalUtilities::isValidAttributeName( "Ae" ) );
	ASSERT_FALSE( co::LexicalUtilities::isValidAttributeName( "Baaaa" ) );
}

TEST( LexicalUtilitiesTests, formatAccessor )
{
	std::string attributeName;

	co::LexicalUtilities::formatAccessor( "name", co::LexicalUtilities::Getter, attributeName );
	ASSERT_EQ( attributeName, "getName" );

	co::LexicalUtilities::formatAccessor( "name", co::LexicalUtilities::Setter, attributeName );
	ASSERT_EQ( attributeName, "setName" );

	co::LexicalUtilities::formatAccessor( "Name", co::LexicalUtilities::Getter, attributeName );
	ASSERT_EQ( attributeName, "getName" );

	co::LexicalUtilities::formatAccessor( "Name", co::LexicalUtilities::Setter, attributeName );
	ASSERT_EQ( attributeName, "setName" );

	co::LexicalUtilities::formatAccessor( "a", co::LexicalUtilities::Getter, attributeName );
	ASSERT_EQ( attributeName, "getA" );

	co::LexicalUtilities::formatAccessor( "a", co::LexicalUtilities::Setter, attributeName );
	ASSERT_EQ( attributeName, "setA" );
}

TEST( LexicalUtilitiesTests, parseAccessor )
{
	int res;
	std::string attributeName;

	res = co::LexicalUtilities::parseAccessor( "getName", co::LexicalUtilities::Getter, attributeName );
	ASSERT_EQ( "name", attributeName );
	ASSERT_EQ( co::LexicalUtilities::Getter, res );

	res = co::LexicalUtilities::parseAccessor( "getName", co::LexicalUtilities::Getter | co::LexicalUtilities::Setter, attributeName );
	ASSERT_EQ( "name", attributeName );
	ASSERT_EQ( co::LexicalUtilities::Getter, res );

	res = co::LexicalUtilities::parseAccessor( "setName", co::LexicalUtilities::Setter, attributeName );
	ASSERT_EQ( "name" , attributeName );
	ASSERT_EQ( co::LexicalUtilities::Setter, res );

	res = co::LexicalUtilities::parseAccessor( "setName", co::LexicalUtilities::Getter | co::LexicalUtilities::Setter, attributeName );
	ASSERT_EQ( "name", attributeName );
	ASSERT_EQ( co::LexicalUtilities::Setter, res );

	res = co::LexicalUtilities::parseAccessor( "getname", co::LexicalUtilities::Getter, attributeName );
	ASSERT_EQ( -1, res );

	res = co::LexicalUtilities::parseAccessor( "GetName", co::LexicalUtilities::Getter, attributeName );
	ASSERT_EQ( -1, res );

	res = co::LexicalUtilities::parseAccessor( "getName", co::LexicalUtilities::Setter, attributeName );
	ASSERT_EQ( -1, res );

	res = co::LexicalUtilities::parseAccessor( "get", co::LexicalUtilities::Getter, attributeName );
	ASSERT_EQ( -1, res );

	res = co::LexicalUtilities::parseAccessor( "is", co::LexicalUtilities::Getter, attributeName );
	ASSERT_EQ( -1, res );

	res = co::LexicalUtilities::parseAccessor( "set", co::LexicalUtilities::Getter, attributeName );
	ASSERT_EQ( -1, res );

	res = co::LexicalUtilities::parseAccessor( "a", co::LexicalUtilities::Getter, attributeName );
	ASSERT_EQ( -1, res );

	res = co::LexicalUtilities::parseAccessor( "setName", co::LexicalUtilities::Getter, attributeName );
	ASSERT_EQ( -1, res );

	res = co::LexicalUtilities::parseAccessor( "isName", co::LexicalUtilities::Getter, attributeName );
	ASSERT_EQ( -1, res );

	res = co::LexicalUtilities::parseAccessor( "isName", co::LexicalUtilities::Getter | co::LexicalUtilities::Setter, attributeName );
	ASSERT_EQ( -1, res );

	res = co::LexicalUtilities::parseAccessor( "setName", co::LexicalUtilities::Setter, attributeName );
	ASSERT_EQ( "name" , attributeName );
	ASSERT_EQ( co::LexicalUtilities::Setter, res );

	res = co::LexicalUtilities::parseAccessor( "setName", co::LexicalUtilities::Getter | co::LexicalUtilities::Setter, attributeName );
	ASSERT_EQ( "name", attributeName );
	ASSERT_EQ( co::LexicalUtilities::Setter, res );

	res = co::LexicalUtilities::parseAccessor( "isName", co::LexicalUtilities::Setter, attributeName );
	ASSERT_EQ( -1, res );
}
