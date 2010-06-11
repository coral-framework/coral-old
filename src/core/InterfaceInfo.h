/*
 * Coral - A C++ Component Framework.
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
	void setIsProvided( bool isProvided );

	// co::InterfaceInfo methods:
	co::InterfaceType* getType();
	bool getIsProvided();

	DELEGATE_CO_MEMBERINFO_METHODS( MemberInfoImpl:: );

private:
	co::InterfaceType* _interfaceType;
	bool _isProvided;
};

#endif
