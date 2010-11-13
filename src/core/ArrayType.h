/*
 * Coral - Lightweight C++ Component Framework.
 * See Copyright Notice in Coral.h
 */

#ifndef _ARRAYTYPE_H_
#define _ARRAYTYPE_H_

#include "Type.h"
#include "ArrayTypeComponent_Base.h"

/*!
	Component that implements co.ArrayType.
 */
class ArrayType : public co::ArrayTypeComponent_Base, public TypeImpl
{
public:
	virtual ~ArrayType();

	// internal methods:
	void setElementType( co::Type* type );

	// co::ArrayType methods:
	co::Type* getElementType();

	DELEGATE_CO_TYPE_METHODS( TypeImpl:: );

private:
	co::RefPtr<co::Type> _elementType;
};

#endif
