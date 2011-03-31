/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#ifndef _TESTHELPER_H_
#define _TESTHELPER_H_

#include <co/Coral.h>
#include <co/IObject.h>
#include <co/IReflector.h>
#include <co/ITypeTransaction.h>
#include <vector>
#include <string>

namespace co {
	class IType;
	class ITypeBuilder;
	class ITypeTransaction;
}

//! Helper function to instantiate a co::ITypeTransaction.
inline co::ITypeTransaction* createTypeTransaction()
{
	co::IObject* instance = co::newInstance( "co.TypeTransaction" );
	assert( instance );
	co::ITypeTransaction* transaction = instance->getService<co::ITypeTransaction>();
	assert( transaction );
	return transaction;
}

class TestHelper
{
public:
	// Creates a typeBuilder for the given fullTypeName, using the given
	// transaction. It creates all namespaces needed to build the type.
	static co::ITypeBuilder* createBuilder( co::TypeKind kind, const std::string& fullTypeName, co::ITypeTransaction* tct );

	// Gets a system type using fullTypeName
	static co::IType* type( const std::string& fullTypeName );

	// Auxiliary string functions
	inline static bool stringEndsWith( const std::string& str, const std::string& tail )
	{
		return str.rfind( tail ) == ( str.length() - tail.length() );
	}
};

#endif
