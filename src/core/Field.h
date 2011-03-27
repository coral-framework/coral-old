/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#ifndef _ATTRIBUTEINFO_H_
#define _ATTRIBUTEINFO_H_

#include "Member.h"
#include "Field_Base.h"

namespace co {

/*!
	Implements co.IField.
 */
class Field : public Field_Base, public MemberImpl
{
public:
	Field();
	virtual ~Field();

	// internal methods:
	void setType( IType* type );
	void setIsReadOnly( bool isReadOnly );

	// co::IField methods:
	IType* getType();
	bool getIsReadOnly();

	DELEGATE_co_IMember( MemberImpl:: );

private:
	IType* _type;
	bool _isReadOnly;
};

} // namespace co

#endif
