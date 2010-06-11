/*
 * Coral - A C++ Component Framework.
 * See Copyright Notice in Coral.h
 */

#include "InterfaceInfo.h"

InterfaceInfo::InterfaceInfo() : _interfaceType( 0 ), _isProvided( true )
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

void InterfaceInfo::setIsProvided( bool isProvided )
{
	_isProvided = isProvided;
}

co::InterfaceType* InterfaceInfo::getType()
{
	return _interfaceType;
}

bool InterfaceInfo::getIsProvided()
{
	return _isProvided;
}

CORAL_EXPORT_COMPONENT( InterfaceInfo, InterfaceInfoComponent );
