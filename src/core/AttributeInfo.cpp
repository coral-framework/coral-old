/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#include "AttributeInfo.h"

namespace co {

AttributeInfo::AttributeInfo()
	: _type( 0 ), _isReadOnly( true )
{
	// empty
}

AttributeInfo::~AttributeInfo()
{
	// empty
}

void AttributeInfo::setType( IType* type )
{
	_type = type;
}

void AttributeInfo::setIsReadOnly( bool isReadOnly )
{
	_isReadOnly = isReadOnly;
}

IType* AttributeInfo::getType()
{
	return _type;
}

bool AttributeInfo::getIsReadOnly()
{
	return _isReadOnly;
}

CORAL_EXPORT_COMPONENT( AttributeInfo, AttributeInfo );

} // namespace co
