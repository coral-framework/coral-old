/*
 * Coral - Lightweight C++ Component Framework.
 * See Copyright Notice in Coral.h
 */

#include "InterfaceInfo.h"

InterfaceInfo::InterfaceInfo() : _interfaceType( 0 ), _isFacet( true )
{
	// empty
}


InterfaceInfo::~InterfaceInfo()
{
	// empty
}

void InterfaceInfo::setType( co::InterfaceType* interfaceType )
{
	_interfaceType = interfaceType;
}

void InterfaceInfo::setIsFacet( bool isFacet )
{
	_isFacet = isFacet;
}

co::InterfaceType* InterfaceInfo::getType()
{
	return _interfaceType;
}

bool InterfaceInfo::getIsFacet()
{
	return _isFacet;
}

CORAL_EXPORT_COMPONENT( InterfaceInfo, InterfaceInfoComponent );
