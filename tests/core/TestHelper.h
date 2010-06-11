/*
 * Coral - A C++ Component Framework.
 * See Copyright Notice in Coral.h
 */

#ifndef _TESTHELPER_H_
#define _TESTHELPER_H_

#include <co/Coral.h>
#include <co/Component.h>
#include <co/Reflector.h>
#include <co/TypeCreationTransaction.h>
#include <vector>
#include <string>

namespace co {
	class Type;
	class TypeBuilder;
	class TypeCreationTransaction;
}

//! Helper function to instantiate a co::TypeCreationTransaction.
inline co::TypeCreationTransaction* createTypeCreationTransaction()
{
	co::Component* component = co::newInstance( "co.TypeCreationTransactionComponent" );
	assert( component );
	co::TypeCreationTransaction* tct = component->getProvided<co::TypeCreationTransaction>();
	assert( tct );
	return tct;
}

class TestHelper
{
public:
	// Creates a typeBuilder for the given fullTypeName, using the given
	// transaction. It creates all namespaces needed to build the type.
	static co::TypeBuilder* createBuilder( co::TypeKind kind, const std::string& fullTypeName, co::TypeCreationTransaction* tct );

	// Gets a system type using fullTypeName
	static co::Type* type( const std::string& fullTypeName );

	// Auxiliary string functions
	inline static bool stringEndsWith( const std::string& str, const std::string& tail )
	{
		return str.rfind( tail ) == ( str.length() - tail.length() );
	}
};

#endif
