/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#ifndef _ENUMTYPE_H_
#define _ENUMTYPE_H_

#include "Type.h"
#include "EnumType_Base.h"
#include <co/RefVector.h>

namespace co {

/*!
	Implements co.IEnumType.
 */
class EnumType : public EnumType_Base, public TypeImpl
{
public:
	virtual ~EnumType();

	// internal methods:
	void addIdentifier( const std::string& identifier );

	// IEnumType methods:
	ArrayRange<std::string const> getIdentifiers();
	int32 getValueOf( const std::string& identifier );

	DELEGATE_CO_TYPE_METHODS( TypeImpl:: );

private:
	typedef std::vector<std::string> IdentifierVector;
	IdentifierVector _identifiers;
};

} // namespace co

#endif
