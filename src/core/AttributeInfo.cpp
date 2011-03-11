/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#include "AttributeInfo.h"

namespace co {

AttributeInfoComponent::AttributeInfoComponent()
	: _type( 0 ), _isReadOnly( true )
{
	// empty
}

AttributeInfoComponent::~AttributeInfoComponent()
{
	// empty
}

void AttributeInfoComponent::setType( Type* type )
{
	_type = type;
}

void AttributeInfoComponent::setIsReadOnly( bool isReadOnly )
{
	_isReadOnly = isReadOnly;
}

Type* AttributeInfoComponent::getType()
{
	return _type;
}

bool AttributeInfoComponent::getIsReadOnly()
{
	return _isReadOnly;
}

CORAL_EXPORT_COMPONENT( AttributeInfoComponent, AttributeInfoComponent );

} // namespace co
