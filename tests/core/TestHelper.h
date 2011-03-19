/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#ifndef _TESTHELPER_H_
#define _TESTHELPER_H_

#include <co/Coral.h>
#include <co/IComponent.h>
#include <co/IReflector.h>
#include <co/ITypeCreationTransaction.h>
#include <vector>
#include <string>

namespace co {
	class IType;
	class ITypeBuilder;
	class ITypeCreationTransaction;
}

//! Helper function to instantiate a co::ITypeCreationTransaction.
inline co::ITypeCreationTransaction* createTypeCreationTransaction()
{
	co::IComponent* component = co::newInstance( "co.TypeCreationTransaction" );
	assert( component );
	co::ITypeCreationTransaction* tct = component->getFacet<co::ITypeCreationTransaction>();
	assert( tct );
	return tct;
}

class TestHelper
{
public:
	// Creates a typeBuilder for the given fullTypeName, using the given
	// transaction. It creates all namespaces needed to build the type.
	static co::ITypeBuilder* createBuilder( co::TypeKind kind, const std::string& fullTypeName, co::ITypeCreationTransaction* tct );

	// Gets a system type using fullTypeName
	static co::IType* type( const std::string& fullTypeName );

	// Auxiliary string functions
	inline static bool stringEndsWith( const std::string& str, const std::string& tail )
	{
		return str.rfind( tail ) == ( str.length() - tail.length() );
	}
};

#endif
