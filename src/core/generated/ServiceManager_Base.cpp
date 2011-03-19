/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#include "ServiceManager_Base.h"
#include <co/Coral.h>
#include <co/IComponentType.h>
#include <co/IInterfaceInfo.h>
#include <co/IInterfaceType.h>

namespace co {

// ------ co.ServiceManager provides an interface named 'serviceManager', of type co.IServiceManager ------ //

co::IInterfaceType* ServiceManager_co_IServiceManager::getInterfaceType()
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

co::IComponent* ServiceManager_Base::getInterfaceOwner()
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

co::IComponentType* ServiceManager_Base::getComponentType()
{
	co::IType* type = co::getType( "co.ServiceManager" );
	assert( dynamic_cast<co::IComponentType*>( type ) );
	return static_cast<co::IComponentType*>( type );
}

co::Interface* ServiceManager_Base::getInterface( co::IInterfaceInfo* interfaceInfo )
{
	checkValidInterface( interfaceInfo );
	co::Interface* res = NULL;
	switch( interfaceInfo->getIndex() )
	{
	case 0:		res = co::disambiguate<co::Interface, co::IServiceManager>( this ); break;
	default:	raiseUnexpectedInterfaceIndex();
	}
	return res;
}

void ServiceManager_Base::setReceptacle( co::IInterfaceInfo* receptacle, co::Interface* facet )
{
	checkValidReceptacle( receptacle );
	raiseUnexpectedInterfaceIndex();
	CORAL_UNUSED( facet );
}

} // namespace co
