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

co::IInterface* ServiceManager_co_IServiceManager::getInterfaceType()
{
	return co::typeOf<co::IServiceManager>::get();
}

const std::string& ServiceManager_co_IServiceManager::getInterfaceName()
{
	static const std::string s_interfaceName( "serviceManager" );
	return s_interfaceName;
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

co::IObject* ServiceManager_Base::getInterfaceOwner()
{
	return this;
}

void ServiceManager_Base::componentRetain()
{
	incrementRefCount();
}

void ServiceManager_Base::componentRelease()
{
	decrementRefCount();
}

co::IComponent* ServiceManager_Base::getComponentType()
{
	co::IType* type = co::getType( "co.ServiceManager" );
	assert( dynamic_cast<co::IComponent*>( type ) );
	return static_cast<co::IComponent*>( type );
}

co::IService* ServiceManager_Base::getInterface( co::IPort* port )
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

void ServiceManager_Base::setReceptacle( co::IPort* receptacle, co::IService* service )
{
	checkValidReceptacle( receptacle );
	raiseUnexpectedPortIndex();
	CORAL_UNUSED( service );
}

} // namespace co
