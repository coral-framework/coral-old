/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#include "InterfaceInfo.h"

namespace co {

InterfaceInfoComponent::InterfaceInfoComponent() : _interfaceType( 0 ), _isFacet( true )
{
	// empty
}


InterfaceInfoComponent::~InterfaceInfoComponent()
{
	// empty
}

void InterfaceInfoComponent::setType( InterfaceType* interfaceType )
{
	_interfaceType = interfaceType;
}

void InterfaceInfoComponent::setIsFacet( bool isFacet )
{
	_isFacet = isFacet;
}

InterfaceType* InterfaceInfoComponent::getType()
{
	return _interfaceType;
}

bool InterfaceInfoComponent::getIsFacet()
{
	return _isFacet;
}

CORAL_EXPORT_COMPONENT( InterfaceInfoComponent, InterfaceInfoComponent );

} // namespace co
