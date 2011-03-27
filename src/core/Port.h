/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#ifndef _INTERFACEINFO_H_
#define _INTERFACEINFO_H_

#include "Member.h"
#include "Port_Base.h"

namespace co {

/*!
	Implements co.IPort.
 */
class Port : public Port_Base, public MemberImpl
{
public:
	Port();
	virtual ~Port();

	// internal methods:
	void setType( IInterface* interfaceType );
	void setIsFacet( bool isFacet );

	// IPort methods:
	IInterface* getType();
	bool getIsFacet();

	DELEGATE_co_IMember( MemberImpl:: );

private:
	IInterface* _interfaceType;
	bool _isFacet;
};

} // namespace co

#endif
