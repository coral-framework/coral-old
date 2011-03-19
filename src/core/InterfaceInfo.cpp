/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#include "InterfaceInfo.h"

namespace co {

InterfaceInfo::InterfaceInfo() : _interfaceType( 0 ), _isFacet( true )
{
	// empty
}


InterfaceInfo::~InterfaceInfo()
{
	// empty
}

void InterfaceInfo::setType( IInterfaceType* interfaceType )
{
	_interfaceType = interfaceType;
}

void InterfaceInfo::setIsFacet( bool isFacet )
{
	_isFacet = isFacet;
}

IInterfaceType* InterfaceInfo::getType()
{
	return _interfaceType;
}

bool InterfaceInfo::getIsFacet()
{
	return _isFacet;
}

CORAL_EXPORT_COMPONENT( InterfaceInfo, InterfaceInfo );

} // namespace co
