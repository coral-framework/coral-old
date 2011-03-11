/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#ifndef _INTERFACEINFO_H_
#define _INTERFACEINFO_H_

#include "MemberInfo.h"
#include "InterfaceInfoComponent_Base.h"

namespace co {

/*!
	Component that implements co.InterfaceInfo.
 */
class InterfaceInfoComponent : public InterfaceInfoComponent_Base, public MemberInfoImpl
{
public:
	InterfaceInfoComponent();
	virtual ~InterfaceInfoComponent();

	// internal methods:
	void setType( InterfaceType* interfaceType );
	void setIsFacet( bool isFacet );

	// InterfaceInfo methods:
	InterfaceType* getType();
	bool getIsFacet();

	DELEGATE_CO_MEMBERINFO_METHODS( MemberInfoImpl:: );

private:
	InterfaceType* _interfaceType;
	bool _isFacet;
};

} // namespace co

#endif
