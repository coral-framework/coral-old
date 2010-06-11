/*
 * Coral - A C++ Component Framework.
 * See Copyright Notice in Coral.h
 */

#ifndef _CO_RESERVED_LEXICALUTILITIES_H_
#define _CO_RESERVED_LEXICALUTILITIES_H_

#include <co/Platform.h>

#include <string>
#include <vector>

namespace co {

/*!
	Lexical utility functions.
 */
class CORAL_EXPORT LexicalUtilities
{
public:
	/*!
		Returns whether the passed parameter \c str is a valid identifier.
		Identifiers must start with a letter, which can then be followed by more letters,
		numbers and the underscore symbol.
	 */
	static bool isValidIdentifier( const std::string& str );

	/*!
		Returns whether the passed \c identifier is a valid attribute name (i.e. starts with lower case).
		\warning this method assumes the passed parameter is already a valid identifier.
		\sa isValidIdentifier
	 */
	static bool isValidAttributeName( const std::string& identifier );

	//! Enumerates the kind of attribute acessors.
	enum AttributeAccessor
	{
		Getter  = 1,
		Setter  = 2
	};

	/*!
		Generates an accessor name given an attribute name.
		\warning This method assumes \c methodName is a valid identifier.
	 */
	static void formatAccessor( const std::string& attributeName,
								AttributeAccessor accessorKind,
								std::string& methodName );

	/*!
		Parses a method name looking for a specific acessor name pattern.
		The passed method name will be searched for the patterns created by the formatAccessor() method.
		Only the kind of accessors specified in the parameter \c accessorKinds will be searched for.

		If a prefix 'get' or 'set' is found, the subsequent character must be a capital letter
		(or this method will return -1). If a valid pattern is found, the prefix is removed and
		the first letter is converted back to lowercase.

	 	\param[in] methodName an identifier representing a method name.
		\param[in] accessorKinds a bitfield with one or more of the following values:
			- co::LexicalUtilities::Getter meaning that the method should search for a \e getter.
			- co::LexicalUtilities::Setter meaning that the method should search for a \e setter.
		\param[out] attributeName the parsed attribute name, starting with a lower case letter.

		\return One of the following values:
			- co::LexicalUtilities::Getter if a \e getter name was parsed;
			- co::LexicalUtilities::Setter if a \e setter name was parsed;
			- -1 if \c methodName was not a valid accessor name.

		\warning This method assumes \c methodName is a valid identifier.
	 */
	static int parseAccessor( const std::string& methodName, int accessorKinds, std::string& attributeName );
};

} // namespace co

#endif // _CO_RESERVED_LEXICALUTILITIES_H_
