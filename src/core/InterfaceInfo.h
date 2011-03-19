/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#ifndef _INTERFACEINFO_H_
#define _INTERFACEINFO_H_

#include "MemberInfo.h"
#include "InterfaceInfo_Base.h"

namespace co {

/*!
	Implements co.IInterfaceInfo.
 */
class InterfaceInfo : public InterfaceInfo_Base, public MemberInfoImpl
{
public:
	InterfaceInfo();
	virtual ~InterfaceInfo();

	// internal methods:
	void setType( IInterfaceType* interfaceType );
	void setIsFacet( bool isFacet );

	// IInterfaceInfo methods:
	IInterfaceType* getType();
	bool getIsFacet();

	DELEGATE_CO_MEMBERINFO_METHODS( MemberInfoImpl:: );

private:
	IInterfaceType* _interfaceType;
	bool _isFacet;
};

} // namespace co

#endif
