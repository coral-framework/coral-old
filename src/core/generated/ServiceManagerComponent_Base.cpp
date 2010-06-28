/*******************************************************************************
** Base class generated for component 'co.ServiceManagerComponent'
**
** Created: Mon Jun 28 12:01:36 2010
**      by: Coral Compiler version 0.1.0
**
** WARNING! All changes made in this file will be lost when recompiling!
********************************************************************************/

#include "ServiceManagerComponent_Base.h"
#include <co/Coral.h>
#include <co/ComponentType.h>
#include <co/InterfaceInfo.h>
#include <co/InterfaceType.h>

// ------ co.ServiceManagerComponent provides an interface named 'serviceManager', of type co.ServiceManager ------ //

co::InterfaceType* co::ServiceManagerComponent_co_ServiceManager::getInterfaceType()
{
	return co::typeOf<co::ServiceManager>::get();
}

const std::string& co::ServiceManagerComponent_co_ServiceManager::getInterfaceName()
{
	static const std::string s_interfaceName( "serviceManager" );
	return s_interfaceName;
}

// ------ ServiceManagerComponent_Base ------ //

co::ServiceManagerComponent_Base::ServiceManagerComponent_Base()
{
	// empty
}

co::ServiceManagerComponent_Base::~ServiceManagerComponent_Base()
{
	// empty
}

co::Component* co::ServiceManagerComponent_Base::getInterfaceOwner()
{
	return this;
}

void co::ServiceManagerComponent_Base::componentRetain()
{
	incrementRefCount();
}

void co::ServiceManagerComponent_Base::componentRelease()
{
	decrementRefCount();
}

co::ComponentType* co::ServiceManagerComponent_Base::getComponentType()
{
	co::Type* type = co::getType( "co.ServiceManagerComponent" );
	assert( dynamic_cast<co::ComponentType*>( type ) );
	return static_cast<co::ComponentType*>( type );
}

co::Interface* co::ServiceManagerComponent_Base::getInterface( co::InterfaceInfo* interfaceInfo )
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

void co::ServiceManagerComponent_Base::bindInterface( co::InterfaceInfo* clientInterface, co::Interface* instance )
{
	checkValidClientInterface( clientInterface );
	raiseUnexpectedInterfaceIndex();
	CORAL_UNUSED( instance );
}
