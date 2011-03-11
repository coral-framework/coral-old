/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#include "ServiceManager.h"
#include <co/Coral.h>
#include <co/Component.h>
#include <co/Reflector.h>
#include <co/ComponentType.h>
#include <co/InterfaceInfo.h>
#include <co/InterfaceType.h>
#include <co/MissingServiceException.h>
#include <co/NoSuchInterfaceException.h>
#include <co/IllegalArgumentException.h>
#include <sstream>

namespace co {

ServiceManagerComponent::ServiceManagerComponent()
{
	_isLazy = true;
}

ServiceManagerComponent::~ServiceManagerComponent()
{
	// empty
}

void ServiceManagerComponent::initialize()
{
	// empty
}

void ServiceManagerComponent::tearDown()
{
	_services.clear();
}

bool ServiceManagerComponent::getIsLazy()
{
	return _isLazy;
}

void ServiceManagerComponent::setIsLazy( bool isLazy )
{
	_isLazy = isLazy;
}

#define CHECK_NOT_NULL( paramName ) \
	if( !paramName ) throw IllegalArgumentException( "illegal null " #paramName );

void ServiceManagerComponent::addService( InterfaceType* serviceType, Interface* serviceInstance )
{
	validateServiceInstance( serviceType, serviceInstance );
	_services[serviceType].global.setInstance( serviceInstance );
}

void ServiceManagerComponent::addServiceForType( InterfaceType* serviceType, InterfaceType* clientType, Interface* serviceInstance )
{
	validateClientType( clientType );
	validateServiceInstance( serviceType, serviceInstance );
	_services[serviceType].custom[clientType].setInstance( serviceInstance );
}

void ServiceManagerComponent::addServiceImplementation( InterfaceType* serviceType, const std::string& componentName )
{
	ComponentType* ct = validateComponentType( serviceType, componentName );

	LazyInstance& instance = _services[serviceType].global;
	instance.setComponentType( ct );

	if( !_isLazy )
		createServiceInstance( serviceType, instance );
}

void ServiceManagerComponent::addServiceImplementationForType( InterfaceType* serviceType, InterfaceType* clientType, const std::string& componentName )
{
	validateClientType( clientType );
	ComponentType* ct = validateComponentType( serviceType, componentName );

	LazyInstance& instance = _services[serviceType].custom[clientType];
	instance.setComponentType( ct );

	if( !_isLazy )
		createServiceInstance( serviceType, instance, clientType );
}

ServiceManagerComponent::ServiceRecord& ServiceManagerComponent::fetchServiceRecord( InterfaceType* serviceType )
{
	ServiceMap::iterator it = _services.find( serviceType );
	if( it == _services.end() )
		throw MissingServiceException( "unknown service" );
	return it->second;
}

Interface* ServiceManagerComponent::getService( InterfaceType* serviceType )
{
	CHECK_NOT_NULL( serviceType );

	ServiceRecord& rec = fetchServiceRecord( serviceType );
	if( !rec.global.isValid() )
		throw MissingServiceException( "service has no global instance" );

	createServiceInstance( serviceType, rec.global );

	return rec.global.getInstance();
}

Interface* ServiceManagerComponent::getServiceForType( InterfaceType* serviceType, InterfaceType* clientType )
{
	CHECK_NOT_NULL( serviceType );
	CHECK_NOT_NULL( clientType );
	return getServiceForType( fetchServiceRecord( serviceType ), serviceType, clientType );
}

InterfaceInfo* ServiceManagerComponent::getFacetInfo( ComponentType* ct, InterfaceType* itfType )
{
	ArrayRange<InterfaceInfo* const> facets = ct->getFacets();
	for( ; facets; facets.popFirst() )
	{
		if( facets.getFirst()->getType()->isSubTypeOf( itfType ) )
			return facets.getFirst();
	}
	return NULL;
}

Interface* ServiceManagerComponent::getServiceForInstance( InterfaceType* serviceType, Interface* clientInstance )
{
	CHECK_NOT_NULL( serviceType );
	CHECK_NOT_NULL( clientInstance );

	// give preference the component's instance of the service interface, if available
	Component* component = clientInstance->getInterfaceOwner();
	ComponentType* ct = component->getComponentType();
	InterfaceInfo* itfInfo = getFacetInfo( ct, serviceType );
	if( itfInfo )
		return component->getInterface( itfInfo );

	// otherwise, fall back to getServiceForType()
	return getServiceForType( fetchServiceRecord( serviceType ), serviceType, clientInstance->getInterfaceType() );
}

void ServiceManagerComponent::removeService( InterfaceType* serviceType )
{
	CHECK_NOT_NULL( serviceType );

	ServiceMap::iterator it = _services.find( serviceType );
	if( it != _services.end() )
	{
		ServiceRecord& rec = it->second;
		if( rec.global.isValid() )
		{
			rec.global.clear();
			if( rec.custom.empty() )
				_services.erase( it );
			return;
		}
	}

	throw MissingServiceException( "nothing to remove" );
}

void ServiceManagerComponent::removeServiceForType( InterfaceType* serviceType, InterfaceType* clientType )
{
	CHECK_NOT_NULL( serviceType );
	CHECK_NOT_NULL( clientType );

	ServiceMap::iterator it = _services.find( serviceType );
	if( it != _services.end() )
	{
		ServiceRecord& rec = it->second;
		CustomServicesMap::iterator customIt = rec.custom.find( clientType );
		if( customIt != rec.custom.end() )
		{
			rec.custom.erase( customIt );
			if( !rec.global.isValid() && rec.custom.empty() )
				_services.erase( it );
			return;
		}
	}

	throw MissingServiceException( "nothing to remove" );
}

void ServiceManagerComponent::validateClientType( InterfaceType* clientType )
{
	CHECK_NOT_NULL( clientType );

	// is clientType the co.Interface interface?
	if( clientType->getSuperInterfaces().isEmpty() )
		throw IllegalArgumentException( "illegal clientType (co.Interface)" );
}

void ServiceManagerComponent::validateServiceInstance( InterfaceType* serviceType, Interface* serviceInstance )
{
	CHECK_NOT_NULL( serviceType );
	CHECK_NOT_NULL( serviceInstance );
	if( !serviceInstance->getInterfaceType()->isSubTypeOf( serviceType ) )
		CORAL_THROW( IllegalArgumentException, "invalid serviceInstance (not a " << serviceType->getFullName() << ")" );
}

ComponentType* ServiceManagerComponent::validateComponentType( InterfaceType* serviceType, const std::string& componentName )
{
	CHECK_NOT_NULL( serviceType );

	ComponentType* ct = NULL;
	try
	{
		ct = dynamic_cast<ComponentType*>( getType( componentName ) );
	}
	catch( ... )
	{;}

	if( !ct )
		CORAL_THROW( IllegalArgumentException, "invalid component type name '" << componentName << "'" );

	if( getFacetInfo( ct, serviceType ) )
		return ct; // ok, component implements the service interface

	CORAL_THROW( NoSuchInterfaceException, "component '" << componentName
			<< "' does not provide the service interface ('" << serviceType->getFullName() << "')" );
}

void ServiceManagerComponent::createServiceInstance( InterfaceType* serviceType, LazyInstance& instance, InterfaceType* clientType )
{
	if( instance.hasInstance() )
		return;

	ComponentType* ct = instance.getComponentType();

	try
	{
		Reflector* reflector = ct->getReflector();
		Component* component = reflector->newInstance();
		InterfaceInfo* serviceItfInfo = getFacetInfo( ct, serviceType );
		assert( serviceItfInfo );
		instance.setInstance( component->getInterface( serviceItfInfo ) );
	}
	catch( std::exception& e )
	{
		if( clientType )
			removeServiceForType( serviceType, clientType );
		else
			removeService( serviceType );

		CORAL_THROW( MissingServiceException, "error instantiating service '" << serviceType->getFullName()
			<< "' from component type '" << ct->getFullName() << "': " << e.what() );
	}
}

Interface* ServiceManagerComponent::getServiceForType( ServiceRecord& rec, InterfaceType* serviceType, InterfaceType* clientType )
{
	// pick the first service instance available for (a super-type of) clientType
	LazyInstance* instance = findSuitableInstance( rec, serviceType, clientType );

	// if none is available, fall back to the global provider
	if( !instance )
	{
		if( rec.global.isValid() )
			instance = &rec.global;
		else
			throw MissingServiceException( "no suitable service instance found" );
	}

	assert( instance );

	createServiceInstance( serviceType, *instance, clientType );

	return instance->getInstance();
}

ServiceManagerComponent::LazyInstance* ServiceManagerComponent::findSuitableInstance( ServiceRecord& rec, InterfaceType* serviceType, InterfaceType* clientType )
{
	ArrayRange<InterfaceType* const> superInterfaces = clientType->getSuperInterfaces();
	if( superInterfaces.isEmpty() )
		return NULL; // we reached a co.Interface

	// check if a custom service instance was provided for this clientType
	CustomServicesMap::iterator it = rec.custom.find( clientType );
	if( it != rec.custom.end() )
		return &it->second;

	// proceed with the depth-first search
	for( ; superInterfaces; superInterfaces.popFirst() )
	{
		LazyInstance* res = findSuitableInstance( rec, serviceType, superInterfaces.getFirst() );
		if( res )
			return res;
	}

	return NULL;
}

CORAL_EXPORT_COMPONENT( ServiceManagerComponent, ServiceManagerComponent );

} // namespace co
