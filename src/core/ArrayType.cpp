/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#include "ArrayType.h"

namespace co {

ArrayTypeComponent::~ArrayTypeComponent()
{
	// empty
}

void ArrayTypeComponent::setElementType( Type* type )
{
	_elementType = type;
}

Type* ArrayTypeComponent::getElementType()
{
	return _elementType.get();
}

CORAL_EXPORT_COMPONENT( ArrayTypeComponent, ArrayTypeComponent );

} // namespace co
