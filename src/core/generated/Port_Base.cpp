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

co::IInterface* Port_co_IPort::getInterfaceType()
{
	return co::typeOf<co::IPort>::get();
}

const std::string& Port_co_IPort::getInterfaceName()
{
	static const std::string s_interfaceName( "port" );
	return s_interfaceName;
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

co::IObject* Port_Base::getInterfaceOwner()
{
	return this;
}

void Port_Base::componentRetain()
{
	incrementRefCount();
}

void Port_Base::componentRelease()
{
	decrementRefCount();
}

co::IComponent* Port_Base::getComponentType()
{
	co::IType* type = co::getType( "co.Port" );
	assert( dynamic_cast<co::IComponent*>( type ) );
	return static_cast<co::IComponent*>( type );
}

co::IService* Port_Base::getInterface( co::IPort* port )
{
	checkValidPort( port );
	co::IService* res = NULL;
	switch( port->getIndex() )
	{
	case 0:		res = co::disambiguate<co::IService, co::IPort>( this ); break;
	default:	raiseUnexpectedInterfaceIndex();
	}
	return res;
}

void Port_Base::setReceptacle( co::IPort* receptacle, co::IService* facet )
{
	checkValidReceptacle( receptacle );
	raiseUnexpectedInterfaceIndex();
	CORAL_UNUSED( facet );
}

} // namespace co
