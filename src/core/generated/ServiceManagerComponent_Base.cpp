/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#include "ServiceManagerComponent_Base.h"
#include <co/Coral.h>
#include <co/ComponentType.h>
#include <co/InterfaceInfo.h>
#include <co/InterfaceType.h>

namespace co {

// ------ co.ServiceManagerComponent provides an interface named 'serviceManager', of type co.ServiceManager ------ //

co::InterfaceType* ServiceManagerComponent_co_ServiceManager::getInterfaceType()
{
	return co::typeOf<co::ServiceManager>::get();
}

const std::string& ServiceManagerComponent_co_ServiceManager::getInterfaceName()
{
	static const std::string s_interfaceName( "serviceManager" );
	return s_interfaceName;
}

// ------ ServiceManagerComponent_Base ------ //

ServiceManagerComponent_Base::ServiceManagerComponent_Base()
{
	// empty
}

ServiceManagerComponent_Base::~ServiceManagerComponent_Base()
{
	// empty
}

co::Component* ServiceManagerComponent_Base::getInterfaceOwner()
{
	return this;
}

void ServiceManagerComponent_Base::componentRetain()
{
	incrementRefCount();
}

void ServiceManagerComponent_Base::componentRelease()
{
	decrementRefCount();
}

co::ComponentType* ServiceManagerComponent_Base::getComponentType()
{
	co::Type* type = co::getType( "co.ServiceManagerComponent" );
	assert( dynamic_cast<co::ComponentType*>( type ) );
	return static_cast<co::ComponentType*>( type );
}

co::Interface* ServiceManagerComponent_Base::getInterface( co::InterfaceInfo* interfaceInfo )
{
	checkValidInterface( interfaceInfo );
	co::Interface* res = NULL;
	switch( interfaceInfo->getIndex() )
	{
	case 0:		res = co::disambiguate<co::Interface, co::ServiceManager>( this ); break;
	default:	raiseUnexpectedInterfaceIndex();
	}
	return res;
}

void ServiceManagerComponent_Base::bindInterface( co::InterfaceInfo* receptacle, co::Interface* instance )
{
	checkValidReceptacle( receptacle );
	raiseUnexpectedInterfaceIndex();
	CORAL_UNUSED( instance );
}

} // namespace co
