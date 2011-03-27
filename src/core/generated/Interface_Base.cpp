/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#include "Interface_Base.h"
#include <co/Coral.h>
#include <co/IComponent.h>
#include <co/IPort.h>
#include <co/IInterface.h>

namespace co {

// ------ co.Interface provides an interface named 'type', of type co.IInterface ------ //

co::IInterface* Interface_co_IInterface::getInterfaceType()
{
	return co::typeOf<co::IInterface>::get();
}

const std::string& Interface_co_IInterface::getInterfaceName()
{
	static const std::string s_interfaceName( "type" );
	return s_interfaceName;
}

// ------ Interface_Base ------ //

Interface_Base::Interface_Base()
{
	// empty
}

Interface_Base::~Interface_Base()
{
	// empty
}

co::IObject* Interface_Base::getInterfaceOwner()
{
	return this;
}

void Interface_Base::componentRetain()
{
	incrementRefCount();
}

void Interface_Base::componentRelease()
{
	decrementRefCount();
}

co::IComponent* Interface_Base::getComponentType()
{
	co::IType* type = co::getType( "co.Interface" );
	assert( dynamic_cast<co::IComponent*>( type ) );
	return static_cast<co::IComponent*>( type );
}

co::IService* Interface_Base::getInterface( co::IPort* port )
{
	checkValidPort( port );
	co::IService* res = NULL;
	switch( port->getIndex() )
	{
	case 0:		res = co::disambiguate<co::IService, co::IInterface>( this ); break;
	default:	raiseUnexpectedInterfaceIndex();
	}
	return res;
}

void Interface_Base::setReceptacle( co::IPort* receptacle, co::IService* facet )
{
	checkValidReceptacle( receptacle );
	raiseUnexpectedInterfaceIndex();
	CORAL_UNUSED( facet );
}

} // namespace co
