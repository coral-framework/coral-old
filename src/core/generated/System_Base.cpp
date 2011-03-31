/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#include "System_Base.h"
#include <co/Coral.h>
#include <co/IComponent.h>
#include <co/IPort.h>
#include <co/IInterface.h>

namespace co {

// ------ co.System provides an interface named 'system', of type co.ISystem ------ //

co::IInterface* System_co_ISystem::getInterface()
{
	return co::typeOf<co::ISystem>::get();
}

co::IPort* System_co_ISystem::getFacet()
{
	co::IComponent* component = static_cast<co::IComponent*>( co::getType( "co.System" ) );
	assert( component );
	co::IPort* facet = static_cast<co::IPort*>( component->getMember( "system" ) );
	assert( facet );
	return facet;
}

// ------ System_Base ------ //

System_Base::System_Base()
{
	// empty
}

System_Base::~System_Base()
{
	// empty
}

co::IObject* System_Base::getProvider()
{
	return this;
}

void System_Base::serviceRetain()
{
	incrementRefCount();
}

void System_Base::serviceRelease()
{
	decrementRefCount();
}

co::IComponent* System_Base::getComponent()
{
	co::IType* type = co::getType( "co.System" );
	assert( dynamic_cast<co::IComponent*>( type ) );
	return static_cast<co::IComponent*>( type );
}

co::IService* System_Base::getService( co::IPort* port )
{
	checkValidPort( port );
	co::IService* res = NULL;
	switch( port->getIndex() )
	{
	case 0:		res = co::disambiguate<co::IService, co::ISystem>( this ); break;
	default:	raiseUnexpectedPortIndex();
	}
	return res;
}

void System_Base::setService( co::IPort* receptacle, co::IService* service )
{
	checkValidReceptacle( receptacle );
	raiseUnexpectedPortIndex();
	CORAL_UNUSED( service );
}

} // namespace co
