/*
 * Coral - A C++ Component Framework.
 * See Copyright Notice in Coral.h
 */

#include "EnumType.h"
#include <algorithm>

EnumType::~EnumType()
{
	// empty
}

void EnumType::addIdentifier( const std::string& identifier )
{
	_identifiers.push_back( identifier );
}

co::ArrayRange<std::string const> EnumType::getIdentifiers()
{
	return _identifiers;
}

co::int32 EnumType::getValueOf( const std::string& identifier )
{
	co::int32 count = _identifiers.size();
	for( co::int32 i = 0; i < count; ++i )
	{
		if( identifier == _identifiers[i] )
			return i;
	}
	return -1;
}

CORAL_EXPORT_COMPONENT( EnumType, EnumTypeComponent );
