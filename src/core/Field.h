/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#ifndef _CO_FIELD_H_
#define _CO_FIELD_H_

#include "Member.h"
#include "Field_Base.h"

namespace co {

/*!
	Implements co.IField.
 */
class Field : public Member<Field_Base>
{
public:
	Field();
	virtual ~Field();

	// internal methods:
	void setType( IType* type );
	void setIsReadOnly( bool isReadOnly );

	// IMember methods:
	MemberKind getKind();

	// IField methods:
	IType* getType();
	bool getIsReadOnly();

private:
	IType* _type;
	bool _isReadOnly;
};

} // namespace co

#endif
