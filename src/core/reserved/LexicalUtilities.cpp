/*
 * Coral - A C++ Component Framework.
 * See Copyright Notice in Coral.h
 */

#include <co/reserved/LexicalUtilities.h>
#include <cassert>
#include <cctype>

static const std::string GETTER_PREFIX( "get" );
static const std::string SETTER_PREFIX( "set" );

namespace co {

inline bool isAlphaOrUnderscore( char c )
{
	return ( c >= 'a' && c <= 'z' ) || ( c >= 'A' && c <= 'Z' ) || c == '_';
}

inline bool isDigit( char c )
{
	return ( c >= '0' && c <= '9' );
}

bool LexicalUtilities::isValidIdentifier( const std::string& str )
{
	std::size_t length = str.length();
	if( length < 1 || !isAlphaOrUnderscore( str[0] ) )
		return false;

	for( std::size_t i = 1; i < length; ++i )
	{
		char c = str[i];
		if( !isAlphaOrUnderscore( c ) && !isDigit( c ) )
			return false;
	}

	return true;
}

bool LexicalUtilities::isValidAttributeName( const std::string& identifier )
{
	return !identifier.empty() && islower( identifier[0] ) != 0;
}

void LexicalUtilities::formatAccessor( const std::string& attributeName, AttributeAccessor kind, std::string& methodName )
{
	assert( !attributeName.empty() );

	const int PREFIX_LENGTH = 3;
	methodName.reserve( attributeName.length() + PREFIX_LENGTH );

	switch( kind )
	{
	case Getter:
		methodName = GETTER_PREFIX;
		break;

	case Setter:
		methodName = SETTER_PREFIX;
		break;
	}

	// append attributeName
	methodName.append( attributeName );
	methodName[PREFIX_LENGTH] = toupper( methodName[PREFIX_LENGTH] );
}

inline bool startsWith( const std::string& str, const std::string& start )
{
	return str.find( start ) == 0;
}

int LexicalUtilities::parseAccessor( const std::string& methodName, int accessorKinds, std::string& attributeName )
{
	assert( accessorKinds > 0 && accessorKinds < 4 );

	int foundKind;
	const char* attributeNamePos = 0;

	if( accessorKinds & Getter && startsWith( methodName, GETTER_PREFIX ) )
	{
		foundKind = Getter;
		attributeNamePos = methodName.c_str() + GETTER_PREFIX.length();
	}

	if( accessorKinds & Setter && startsWith( methodName, SETTER_PREFIX )  )
	{
		foundKind = Setter;
		attributeNamePos = methodName.c_str() + SETTER_PREFIX.length();
	}

	if( attributeNamePos == 0 )
		return -1;

	int firstChar = tolower( attributeNamePos[0] );
	if( firstChar == attributeNamePos[0] )
		return -1;

	attributeName.clear();
	attributeName.reserve( attributeNamePos - methodName.c_str() );
	attributeName += firstChar;
	attributeName += attributeNamePos + 1;

	return foundKind;
}

} // namespace co
