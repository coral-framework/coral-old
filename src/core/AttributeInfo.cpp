/*
 * Coral - Lightweight C++ Component Framework.
 * See Copyright Notice in Coral.h
 */

#include "AttributeInfo.h"

AttributeInfo::AttributeInfo()
	: _type( 0 ), _isReadOnly( true )
{
	// empty
}

AttributeInfo::~AttributeInfo()
{
	// empty
}

void AttributeInfo::setType( co::Type* type )
{
	_type = type;
}

void AttributeInfo::setIsReadOnly( bool isReadOnly )
{
	_isReadOnly = isReadOnly;
}

co::Type* AttributeInfo::getType()
{
	return _type;
}

bool AttributeInfo::getIsReadOnly()
{
	return _isReadOnly;
}

CORAL_EXPORT_COMPONENT( AttributeInfo, AttributeInfoComponent );
