/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#ifndef _ATTRIBUTEINFO_H_
#define _ATTRIBUTEINFO_H_

#include "MemberInfo.h"
#include "AttributeInfoComponent_Base.h"

namespace co {

/*!
	Component that implements co.AttributeInfo.
 */
class AttributeInfoComponent : public AttributeInfoComponent_Base, public MemberInfoImpl
{
public:
	AttributeInfoComponent();
	virtual ~AttributeInfoComponent();

	// internal methods:
	void setType( Type* type );
	void setIsReadOnly( bool isReadOnly );

	// co::AttributeInfo methods:
	Type* getType();
	bool getIsReadOnly();

	DELEGATE_CO_MEMBERINFO_METHODS( MemberInfoImpl:: );

private:
	Type* _type;
	bool _isReadOnly;
};

} // namespace co

#endif
