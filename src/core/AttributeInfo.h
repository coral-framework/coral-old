/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#ifndef _ATTRIBUTEINFO_H_
#define _ATTRIBUTEINFO_H_

#include "MemberInfo.h"
#include "AttributeInfo_Base.h"

namespace co {

/*!
	Implements co.IAttributeInfo.
 */
class AttributeInfo : public AttributeInfo_Base, public MemberInfoImpl
{
public:
	AttributeInfo();
	virtual ~AttributeInfo();

	// internal methods:
	void setType( IType* type );
	void setIsReadOnly( bool isReadOnly );

	// co::IAttributeInfo methods:
	IType* getType();
	bool getIsReadOnly();

	DELEGATE_CO_MEMBERINFO_METHODS( MemberInfoImpl:: );

private:
	IType* _type;
	bool _isReadOnly;
};

} // namespace co

#endif
