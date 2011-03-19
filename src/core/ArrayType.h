/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#ifndef _ARRAYTYPE_H_
#define _ARRAYTYPE_H_

#include "Type.h"
#include "ArrayType_Base.h"

namespace co {

/*!
	Implements co.IArrayType.
 */
class ArrayType : public ArrayType_Base, public TypeImpl
{
public:
	virtual ~ArrayType();

	// internal methods:
	void setElementType( IType* type );

	// co::IArrayType methods:
	IType* getElementType();

	DELEGATE_CO_TYPE_METHODS( TypeImpl:: );

private:
	RefPtr<IType> _elementType;
};

} // namespace co

#endif
