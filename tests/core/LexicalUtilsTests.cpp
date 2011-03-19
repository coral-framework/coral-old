/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#include <co/Coral.h>
#include <co/RefPtr.h>
#include <co/ITypeManager.h>
#include <co/reserved/LexicalUtils.h>
#include <gtest/gtest.h>

TEST( LexicalUtilsTests, isValidIdentifier )
{
	ASSERT_TRUE( co::LexicalUtils::isValidIdentifier( "A" ) );
	ASSERT_TRUE( co::LexicalUtils::isValidIdentifier( "a" ) );
	ASSERT_TRUE( co::LexicalUtils::isValidIdentifier( "Ab_cd" ) );
	ASSERT_TRUE( co::LexicalUtils::isValidIdentifier( "d123" ) );
	ASSERT_TRUE( co::LexicalUtils::isValidIdentifier( "lalalalalala" ) );

	ASSERT_FALSE( co::LexicalUtils::isValidIdentifier( "" ) );
	ASSERT_FALSE( co::LexicalUtils::isValidIdentifier( "1lalla" ) );
	ASSERT_FALSE( co::LexicalUtils::isValidIdentifier( " lalala" ) );
	ASSERT_FALSE( co::LexicalUtils::isValidIdentifier( "lalala " ) );
	ASSERT_FALSE( co::LexicalUtils::isValidIdentifier( "lalala!" ) );
	ASSERT_FALSE( co::LexicalUtils::isValidIdentifier( "lalala lelele" ) );
	ASSERT_FALSE( co::LexicalUtils::isValidIdentifier( " " ) );
	ASSERT_FALSE( co::LexicalUtils::isValidIdentifier( "." ) );
}

TEST( LexicalUtilsTests, isValidAttribute )
{
	ASSERT_TRUE( co::LexicalUtils::isValidAttributeName( "a" ) );
	ASSERT_TRUE( co::LexicalUtils::isValidAttributeName( "ala" ) );
	ASSERT_TRUE( co::LexicalUtils::isValidAttributeName( "ab_cd" ) );
	ASSERT_TRUE( co::LexicalUtils::isValidAttributeName( "d123" ) );
	ASSERT_TRUE( co::LexicalUtils::isValidAttributeName( "lalalalalala" ) );

	ASSERT_FALSE( co::LexicalUtils::isValidAttributeName( "" ) );
	ASSERT_FALSE( co::LexicalUtils::isValidAttributeName( "Ae" ) );
	ASSERT_FALSE( co::LexicalUtils::isValidAttributeName( "Baaaa" ) );
}

TEST( LexicalUtilsTests, formatAccessor )
{
	std::string attributeName;

	co::LexicalUtils::formatAccessor( "name", co::LexicalUtils::Getter, attributeName );
	ASSERT_EQ( attributeName, "getName" );

	co::LexicalUtils::formatAccessor( "name", co::LexicalUtils::Setter, attributeName );
	ASSERT_EQ( attributeName, "setName" );

	co::LexicalUtils::formatAccessor( "Name", co::LexicalUtils::Getter, attributeName );
	ASSERT_EQ( attributeName, "getName" );

	co::LexicalUtils::formatAccessor( "Name", co::LexicalUtils::Setter, attributeName );
	ASSERT_EQ( attributeName, "setName" );

	co::LexicalUtils::formatAccessor( "a", co::LexicalUtils::Getter, attributeName );
	ASSERT_EQ( attributeName, "getA" );

	co::LexicalUtils::formatAccessor( "a", co::LexicalUtils::Setter, attributeName );
	ASSERT_EQ( attributeName, "setA" );
}

TEST( LexicalUtilsTests, parseAccessor )
{
	int res;
	std::string attributeName;

	res = co::LexicalUtils::parseAccessor( "getName", co::LexicalUtils::Getter, attributeName );
	ASSERT_EQ( "name", attributeName );
	ASSERT_EQ( co::LexicalUtils::Getter, res );

	res = co::LexicalUtils::parseAccessor( "getName", co::LexicalUtils::Getter | co::LexicalUtils::Setter, attributeName );
	ASSERT_EQ( "name", attributeName );
	ASSERT_EQ( co::LexicalUtils::Getter, res );

	res = co::LexicalUtils::parseAccessor( "setName", co::LexicalUtils::Setter, attributeName );
	ASSERT_EQ( "name" , attributeName );
	ASSERT_EQ( co::LexicalUtils::Setter, res );

	res = co::LexicalUtils::parseAccessor( "setName", co::LexicalUtils::Getter | co::LexicalUtils::Setter, attributeName );
	ASSERT_EQ( "name", attributeName );
	ASSERT_EQ( co::LexicalUtils::Setter, res );

	res = co::LexicalUtils::parseAccessor( "getname", co::LexicalUtils::Getter, attributeName );
	ASSERT_EQ( -1, res );

	res = co::LexicalUtils::parseAccessor( "GetName", co::LexicalUtils::Getter, attributeName );
	ASSERT_EQ( -1, res );

	res = co::LexicalUtils::parseAccessor( "getName", co::LexicalUtils::Setter, attributeName );
	ASSERT_EQ( -1, res );

	res = co::LexicalUtils::parseAccessor( "get", co::LexicalUtils::Getter, attributeName );
	ASSERT_EQ( -1, res );

	res = co::LexicalUtils::parseAccessor( "is", co::LexicalUtils::Getter, attributeName );
	ASSERT_EQ( -1, res );

	res = co::LexicalUtils::parseAccessor( "set", co::LexicalUtils::Getter, attributeName );
	ASSERT_EQ( -1, res );

	res = co::LexicalUtils::parseAccessor( "a", co::LexicalUtils::Getter, attributeName );
	ASSERT_EQ( -1, res );

	res = co::LexicalUtils::parseAccessor( "setName", co::LexicalUtils::Getter, attributeName );
	ASSERT_EQ( -1, res );

	res = co::LexicalUtils::parseAccessor( "isName", co::LexicalUtils::Getter, attributeName );
	ASSERT_EQ( -1, res );

	res = co::LexicalUtils::parseAccessor( "isName", co::LexicalUtils::Getter | co::LexicalUtils::Setter, attributeName );
	ASSERT_EQ( -1, res );

	res = co::LexicalUtils::parseAccessor( "setName", co::LexicalUtils::Setter, attributeName );
	ASSERT_EQ( "name" , attributeName );
	ASSERT_EQ( co::LexicalUtils::Setter, res );

	res = co::LexicalUtils::parseAccessor( "setName", co::LexicalUtils::Getter | co::LexicalUtils::Setter, attributeName );
	ASSERT_EQ( "name", attributeName );
	ASSERT_EQ( co::LexicalUtils::Setter, res );

	res = co::LexicalUtils::parseAccessor( "isName", co::LexicalUtils::Setter, attributeName );
	ASSERT_EQ( -1, res );
}
