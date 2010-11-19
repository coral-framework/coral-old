/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#ifndef _INTERFACEINFO_H_
#define _INTERFACEINFO_H_

#include "MemberInfo.h"
#include "InterfaceInfoComponent_Base.h"

/*!
	Component that implements co.InterfaceInfo.
 */
class InterfaceInfo : public co::InterfaceInfoComponent_Base, public MemberInfoImpl
{
public:
	InterfaceInfo();
	virtual ~InterfaceInfo();

	// internal methods:
	void setType( co::InterfaceType* interfaceType );
	void setIsFacet( bool isFacet );

	// co::InterfaceInfo methods:
	co::InterfaceType* getType();
	bool getIsFacet();

	DELEGATE_CO_MEMBERINFO_METHODS( MemberInfoImpl:: );

private:
	co::InterfaceType* _interfaceType;
	bool _isFacet;
};

#endif
