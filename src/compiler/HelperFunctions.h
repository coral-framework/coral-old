/*
 * Coral - A C++ Component Framework.
 * See Copyright Notice in Coral.h
 */

#ifndef _HELPERFUNCTIONS_H_
#define _HELPERFUNCTIONS_H_

#include <co/Type.h>

inline void convertSeparators( std::string& str, char sep, char newSep )
{
	unsigned int len = str.length();
	for( unsigned int i = 0; i < len; ++i )
		if( str[i] == sep )
			str[i]  = newSep;
}

inline void typeToHeaderName( std::string& str )
{
	convertSeparators( str, '.', '/' );
	str += ".h";
}

inline void concatenate( const std::string& s1, const std::string& s2, const std::string& s3, std::string& out )
{
	out.reserve( s1.size() + s2.size() + s3.size() );
	out = s1;
	out += s2;
	out += s3;
}

inline std::string getCppName( const std::string& coralName )
{
	std::string cppName( coralName );
	for( unsigned int i = 0; i < cppName.length(); ++i )
		if( cppName[i] == '.' )
			cppName.replace( i, 1, "::" );
	return cppName;
}

inline std::string getCppName( co::Type* type )
{
	co::TypeKind kind = type->getKind();
	assert( kind != co::TK_NONE && kind != co::TK_ARRAY );

	if( kind < co::TK_ARRAY )
		return co::TK_STRINGS_CPP[kind];

	return getCppName( type->getFullName() );
}

inline const char* getTypeInterfaceName( co::Type* type )
{
	const char* res;
	switch( type->getKind() )
	{
	case co::TK_ARRAY:			res = "ArrayType";			break;
	case co::TK_ENUM:			res = "EnumType";			break;
	case co::TK_EXCEPTION:		res = "ExceptionType";		break;
	case co::TK_STRUCT:			res = "StructType";			break;
	case co::TK_NATIVECLASS:	res = "NativeClassType";	break;
	case co::TK_INTERFACE:		res = "InterfaceType";		break;
	case co::TK_COMPONENT:		res = "ComponentType";		break;
	default:					res = "Type";
	}
	return res;
}

#endif // _HELPERFUNCTIONS_H_
