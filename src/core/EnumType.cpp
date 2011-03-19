/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#include "EnumType.h"
#include <algorithm>

namespace co {

EnumType::~EnumType()
{
	// empty
}

void EnumType::addIdentifier( const std::string& identifier )
{
	_identifiers.push_back( identifier );
}

ArrayRange<std::string const> EnumType::getIdentifiers()
{
	return _identifiers;
}

int32 EnumType::getValueOf( const std::string& identifier )
{
	int32 count = static_cast<int32>( _identifiers.size() );
	for( int32 i = 0; i < count; ++i )
	{
		if( identifier == _identifiers[i] )
			return i;
	}
	return -1;
}

CORAL_EXPORT_COMPONENT( EnumType, EnumType );

} // namespace co
