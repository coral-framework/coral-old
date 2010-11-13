/*
 * Coral - Lightweight C++ Component Framework.
 * See Copyright Notice in Coral.h
 */

#include "ArrayType.h"

ArrayType::~ArrayType()
{
	// empty
}

void ArrayType::setElementType( co::Type* type )
{
	_elementType = type;
}

co::Type* ArrayType::getElementType()
{
	return _elementType.get();
}

CORAL_EXPORT_COMPONENT( ArrayType, ArrayTypeComponent );
