/*
 * Coral - Lightweight C++ Component Framework.
 * See Copyright Notice in Coral.h
 */

#ifndef _ATTRIBUTEINFO_H_
#define _ATTRIBUTEINFO_H_

#include "MemberInfo.h"
#include "AttributeInfoComponent_Base.h"

/*!
	Component that implements co.AttributeInfo.
 */
class AttributeInfo : public co::AttributeInfoComponent_Base, public MemberInfoImpl
{
public:
	AttributeInfo();
	virtual ~AttributeInfo();

	// internal methods:
	void setType( co::Type* type );
	void setIsReadOnly( bool isReadOnly );

	// co::AttributeInfo methods:
	co::Type* getType();
	bool getIsReadOnly();

	DELEGATE_CO_MEMBERINFO_METHODS( MemberInfoImpl:: );

private:
	co::Type* _type;
	bool _isReadOnly;
};

#endif
