/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#include "ServiceManager_Base.h"
#include <co/Coral.h>
#include <co/IComponent.h>
#include <co/IPort.h>
#include <co/IInterface.h>

namespace co {

// ------ co.ServiceManager provides an interface named 'serviceManager', of type co.IServiceManager ------ //

co::IInterface* ServiceManager_co_IServiceManager::getInterface()
{
	return co::typeOf<co::IServiceManager>::get();
}

co::IPort* ServiceManager_co_IServiceManager::getFacet()
{
	co::IComponent* component = static_cast<co::IComponent*>( co::getType( "co.ServiceManager" ) );
	assert( component );
	co::IPort* facet = static_cast<co::IPort*>( component->getMember( "serviceManager" ) );
	assert( facet );
	return facet;
}

// ------ ServiceManager_Base ------ //

ServiceManager_Base::ServiceManager_Base()
{
	// empty
}

ServiceManager_Base::~ServiceManager_Base()
{
	// empty
}

co::IObject* ServiceManager_Base::getProvider()
{
	return this;
}

void ServiceManager_Base::serviceRetain()
{
	incrementRefCount();
}

void ServiceManager_Base::serviceRelease()
{
	decrementRefCount();
}

co::IComponent* ServiceManager_Base::getComponent()
{
	co::IType* type = co::getType( "co.ServiceManager" );
	assert( dynamic_cast<co::IComponent*>( type ) );
	return static_cast<co::IComponent*>( type );
}

co::IService* ServiceManager_Base::getService( co::IPort* port )
{
	checkValidPort( port );
	co::IService* res = NULL;
	switch( port->getIndex() )
	{
	case 0:		res = co::disambiguate<co::IService, co::IServiceManager>( this ); break;
	default:	raiseUnexpectedPortIndex();
	}
	return res;
}

void ServiceManager_Base::setService( co::IPort* receptacle, co::IService* service )
{
	checkValidReceptacle( receptacle );
	raiseUnexpectedPortIndex();
	CORAL_UNUSED( service );
}

} // namespace co
