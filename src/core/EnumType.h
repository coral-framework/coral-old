/*
 * Coral - Lightweight C++ Component Framework.
 * See Copyright Notice in Coral.h
 */

#ifndef _ENUMTYPE_H_
#define _ENUMTYPE_H_

#include "Type.h"
#include "EnumTypeComponent_Base.h"
#include <co/RefVector.h>

/*!
	Component that implements co.EnumType.
 */
class EnumType : public co::EnumTypeComponent_Base, public TypeImpl
{
public:
	virtual ~EnumType();

	// internal methods:
	void addIdentifier( const std::string& identifier );

	// co::EnumType methods:
	co::ArrayRange<std::string const> getIdentifiers();
	co::int32 getValueOf( const std::string& identifier );

	DELEGATE_CO_TYPE_METHODS( TypeImpl:: );

private:
	typedef std::vector<std::string> IdentifierVector;
	IdentifierVector _identifiers;
};

#endif
