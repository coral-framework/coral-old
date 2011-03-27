/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#include "Component_Base.h"
#include <co/Coral.h>
#include <co/IComponent.h>
#include <co/IPort.h>
#include <co/IInterface.h>

namespace co {

// ------ co.Component provides an interface named 'type', of type co.IComponent ------ //

co::IInterface* Component_co_IComponent::getInterfaceType()
{
	return co::typeOf<co::IComponent>::get();
}

const std::string& Component_co_IComponent::getInterfaceName()
{
	static const std::string s_interfaceName( "type" );
	return s_interfaceName;
}

// ------ Component_Base ------ //

Component_Base::Component_Base()
{
	// empty
}

Component_Base::~Component_Base()
{
	// empty
}

co::IObject* Component_Base::getInterfaceOwner()
{
	return this;
}

void Component_Base::componentRetain()
{
	incrementRefCount();
}

void Component_Base::componentRelease()
{
	decrementRefCount();
}

co::IComponent* Component_Base::getComponentType()
{
	co::IType* type = co::getType( "co.Component" );
	assert( dynamic_cast<co::IComponent*>( type ) );
	return static_cast<co::IComponent*>( type );
}

co::IService* Component_Base::getInterface( co::IPort* port )
{
	checkValidPort( port );
	co::IService* res = NULL;
	switch( port->getIndex() )
	{
	case 0:		res = co::disambiguate<co::IService, co::IComponent>( this ); break;
	default:	raiseUnexpectedInterfaceIndex();
	}
	return res;
}

void Component_Base::setReceptacle( co::IPort* receptacle, co::IService* facet )
{
	checkValidReceptacle( receptacle );
	raiseUnexpectedInterfaceIndex();
	CORAL_UNUSED( facet );
}

} // namespace co
