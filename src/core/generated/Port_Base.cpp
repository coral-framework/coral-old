/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#include "Port_Base.h"
#include <co/Coral.h>
#include <co/IComponent.h>
#include <co/IPort.h>
#include <co/IInterface.h>

namespace co {

// ------ co.Port provides an interface named 'port', of type co.IPort ------ //

co::IInterface* Port_co_IPort::getInterface()
{
	return co::typeOf<co::IPort>::get();
}

co::IPort* Port_co_IPort::getFacet()
{
	co::IComponent* component = static_cast<co::IComponent*>( co::getType( "co.Port" ) );
	assert( component );
	co::IPort* facet = static_cast<co::IPort*>( component->getMember( "port" ) );
	assert( facet );
	return facet;
}

// ------ Port_Base ------ //

Port_Base::Port_Base()
{
	// empty
}

Port_Base::~Port_Base()
{
	// empty
}

co::IObject* Port_Base::getProvider()
{
	return this;
}

void Port_Base::serviceRetain()
{
	incrementRefCount();
}

void Port_Base::serviceRelease()
{
	decrementRefCount();
}

co::IComponent* Port_Base::getComponent()
{
	co::IType* type = co::getType( "co.Port" );
	assert( dynamic_cast<co::IComponent*>( type ) );
	return static_cast<co::IComponent*>( type );
}

co::IService* Port_Base::getService( co::IPort* port )
{
	checkValidPort( port );
	co::IService* res = NULL;
	switch( port->getIndex() )
	{
	case 0:		res = co::disambiguate<co::IService, co::IPort>( this ); break;
	default:	raiseUnexpectedPortIndex();
	}
	return res;
}

void Port_Base::setService( co::IPort* receptacle, co::IService* service )
{
	checkValidReceptacle( receptacle );
	raiseUnexpectedPortIndex();
	CORAL_UNUSED( service );
}

} // namespace co
