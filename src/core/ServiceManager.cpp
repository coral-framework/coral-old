/*
 * Coral - Lightweight C++ Component Framework.
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

ServiceManager::ServiceManager()
{
	_isLazy = true;
}

ServiceManager::~ServiceManager()
{
	// empty
}

void ServiceManager::initialize()
{
	// empty
}

void ServiceManager::tearDown()
{
	_services.clear();
}

bool ServiceManager::getIsLazy()
{
	return _isLazy;
}

void ServiceManager::setIsLazy( bool isLazy )
{
	_isLazy = isLazy;
}

#define CHECK_NOT_NULL( paramName ) \
	if( !paramName ) throw co::IllegalArgumentException( "illegal null " #paramName );

void ServiceManager::addService( co::InterfaceType* serviceType, co::Interface* serviceInstance )
{
	validateServiceInstance( serviceType, serviceInstance );
	_services[serviceType].global.setInstance( serviceInstance );
}

void ServiceManager::addServiceForType( co::InterfaceType* serviceType, co::InterfaceType* clientType, co::Interface* serviceInstance )
{
	validateClientType( clientType );
	validateServiceInstance( serviceType, serviceInstance );
	_services[serviceType].custom[clientType].setInstance( serviceInstance );
}

void ServiceManager::addServiceImplementation( co::InterfaceType* serviceType, const std::string& componentName )
{
	co::ComponentType* ct = validateComponentType( serviceType, componentName );

	LazyInstance& instance = _services[serviceType].global;
	instance.setComponentType( ct );

	if( !_isLazy )
		createServiceInstance( serviceType, instance );
}

void ServiceManager::addServiceImplementationForType( co::InterfaceType* serviceType, co::InterfaceType* clientType, const std::string& componentName )
{
	validateClientType( clientType );
	co::ComponentType* ct = validateComponentType( serviceType, componentName );

	LazyInstance& instance = _services[serviceType].custom[clientType];
	instance.setComponentType( ct );

	if( !_isLazy )
		createServiceInstance( serviceType, instance, clientType );
}

ServiceManager::ServiceRecord& ServiceManager::fetchServiceRecord( co::InterfaceType* serviceType )
{
	ServiceMap::iterator it = _services.find( serviceType );
	if( it == _services.end() )
		throw co::MissingServiceException( "unknown service" );
	return it->second;
}

co::Interface* ServiceManager::getService( co::InterfaceType* serviceType )
{
	CHECK_NOT_NULL( serviceType );

	ServiceRecord& rec = fetchServiceRecord( serviceType );
	if( !rec.global.isValid() )
		throw co::MissingServiceException( "service has no global instance" );

	createServiceInstance( serviceType, rec.global );

	return rec.global.getInstance();
}

co::Interface* ServiceManager::getServiceForType( co::InterfaceType* serviceType, co::InterfaceType* clientType )
{
	CHECK_NOT_NULL( serviceType );
	CHECK_NOT_NULL( clientType );
	return getServiceForType( fetchServiceRecord( serviceType ), serviceType, clientType );
}

co::InterfaceInfo* ServiceManager::getFacetInfo( co::ComponentType* ct, co::InterfaceType* itfType )
{
	co::ArrayRange<co::InterfaceInfo* const> facets = ct->getFacets();
	for( ; facets; facets.popFirst() )
	{
		if( facets.getFirst()->getType()->isSubTypeOf( itfType ) )
			return facets.getFirst();
	}
	return NULL;
}

co::Interface* ServiceManager::getServiceForInstance( co::InterfaceType* serviceType, co::Interface* clientInstance )
{
	CHECK_NOT_NULL( serviceType );
	CHECK_NOT_NULL( clientInstance );

	// give preference the component's instance of the service interface, if available
	co::Component* component = clientInstance->getInterfaceOwner();
	co::ComponentType* ct = component->getComponentType();
	co::InterfaceInfo* itfInfo = getFacetInfo( ct, serviceType );
	if( itfInfo )
		return component->getInterface( itfInfo );

	// otherwise, fall back to getServiceForType()
	return getServiceForType( fetchServiceRecord( serviceType ), serviceType, clientInstance->getInterfaceType() );
}

void ServiceManager::removeService( co::InterfaceType* serviceType )
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

	throw co::MissingServiceException( "nothing to remove" );
}

void ServiceManager::removeServiceForType( co::InterfaceType* serviceType, co::InterfaceType* clientType )
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

	throw co::MissingServiceException( "nothing to remove" );
}

void ServiceManager::validateClientType( co::InterfaceType* clientType )
{
	CHECK_NOT_NULL( clientType );

	// is clientType the co.Interface interface?
	if( clientType->getSuperInterfaces().isEmpty() )
		throw co::IllegalArgumentException( "illegal clientType (co.Interface)" );
}

void ServiceManager::validateServiceInstance( co::InterfaceType* serviceType, co::Interface* serviceInstance )
{
	CHECK_NOT_NULL( serviceType );
	CHECK_NOT_NULL( serviceInstance );
	if( !serviceInstance->getInterfaceType()->isSubTypeOf( serviceType ) )
		CORAL_THROW( co::IllegalArgumentException, "invalid serviceInstance (not a " << serviceType->getFullName() << ")" );
}

co::ComponentType* ServiceManager::validateComponentType( co::InterfaceType* serviceType, const std::string& componentName )
{
	CHECK_NOT_NULL( serviceType );

	co::ComponentType* ct = NULL;
	try
	{
		ct = dynamic_cast<co::ComponentType*>( co::getType( componentName ) );
	}
	catch( ... )
	{;}

	if( !ct )
		CORAL_THROW( co::IllegalArgumentException, "invalid component type name '" << componentName << "'" );

	if( getFacetInfo( ct, serviceType ) )
		return ct; // ok, component implements the service interface

	CORAL_THROW( co::NoSuchInterfaceException, "component '" << componentName
			<< "' does not provide the service interface ('" << serviceType->getFullName() << "')" );
}

void ServiceManager::createServiceInstance( co::InterfaceType* serviceType, LazyInstance& instance, co::InterfaceType* clientType )
{
	if( instance.hasInstance() )
		return;

	try
	{
		co::Reflector* reflector = instance.getComponentType()->getReflector();
		co::Component* component = reflector->newInstance();
		co::InterfaceInfo* serviceItfInfo = getFacetInfo( instance.getComponentType(), serviceType );
		assert( serviceItfInfo );
		instance.setInstance( component->getInterface( serviceItfInfo ) );
	}
	catch( std::exception& e )
	{
		if( clientType )
			removeServiceForType( serviceType, clientType );
		else
			removeService( serviceType );

		CORAL_THROW( co::MissingServiceException, "error instantiating service '" << serviceType->getFullName()
			<< "' from component type '" << instance.getComponentType()->getFullName() << "': " << e.what() );
	}
}

co::Interface* ServiceManager::getServiceForType( ServiceRecord& rec, co::InterfaceType* serviceType, co::InterfaceType* clientType )
{
	// pick the first service instance available for (a super-type of) clientType
	LazyInstance* instance = findSuitableInstance( rec, serviceType, clientType );

	// if none is available, fall back to the global provider
	if( !instance )
	{
		if( rec.global.isValid() )
			instance = &rec.global;
		else
			throw co::MissingServiceException( "no suitable service instance found" );
	}

	assert( instance );

	createServiceInstance( serviceType, *instance, clientType );

	return instance->getInstance();
}

ServiceManager::LazyInstance* ServiceManager::findSuitableInstance( ServiceRecord& rec, co::InterfaceType* serviceType, co::InterfaceType* clientType )
{
	co::ArrayRange<co::InterfaceType* const> superInterfaces = clientType->getSuperInterfaces();
	if( superInterfaces.isEmpty() )
		return NULL; // we reached a co.Interface

	// check if a custom service instance was provided for this clientType
	CustomServicesMap::iterator it = rec.custom.find( clientType );
	if( it != rec.custom.end() )
		return &it->second;

	// proceed with the depth-first search
	for( ; superInterfaces; superInterfaces.popFirst() )
	{
		ServiceManager::LazyInstance* res = findSuitableInstance( rec, serviceType, superInterfaces.getFirst() );
		if( res )
			return res;
	}

	return NULL;
}

CORAL_EXPORT_COMPONENT( ServiceManager, ServiceManagerComponent );
