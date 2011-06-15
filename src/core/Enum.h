/*
 * Coral - Lightweight C++ Component Framework
 * See Copyright Notice in Coral.h
 */

#ifndef _ENUMTYPE_H_
#define _ENUMTYPE_H_

#include "Type.h"
#include "Enum_Base.h"
#include <co/RefVector.h>

namespace co {

/*!
	Implements co.IEnum.
 */
class Enum : public Enum_Base, public TypeImpl
{
public:
	virtual ~Enum();

	// internal methods:
	void addIdentifier( const std::string& identifier );

	// IEnum methods:
	Range<std::string const> getIdentifiers();
	int32 getValueOf( const std::string& identifier );

	DELEGATE_co_IType( TypeImpl:: );

private:
	typedef std::vector<std::string> IdentifierVector;
	IdentifierVector _identifiers;
};

} // namespace co

#endif
