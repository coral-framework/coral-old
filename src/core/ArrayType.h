/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#ifndef _ARRAYTYPE_H_
#define _ARRAYTYPE_H_

#include "Type.h"
#include "ArrayTypeComponent_Base.h"

namespace co {

/*!
	Component that implements co.ArrayType.
 */
class ArrayTypeComponent : public ArrayTypeComponent_Base, public TypeImpl
{
public:
	virtual ~ArrayTypeComponent();

	// internal methods:
	void setElementType( Type* type );

	// co::ArrayType methods:
	Type* getElementType();

	DELEGATE_CO_TYPE_METHODS( TypeImpl:: );

private:
	RefPtr<Type> _elementType;
};

} // namespace co

#endif
