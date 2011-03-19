/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#include "ServiceManager.h"
#include <co/Coral.h>
#include <co/IComponent.h>
#include <co/IReflector.h>
#include <co/IComponentType.h>
#include <co/IInterfaceInfo.h>
#include <co/IInterfaceType.h>
#include <co/MissingServiceException.h>
#include <co/NoSuchInterfaceException.h>
#include <co/IllegalArgumentException.h>
#include <sstream>

namespace co {

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
	if( !paramName ) throw IllegalArgumentException( "illegal null " #paramName );

void ServiceManager::addService( IInterfaceType* serviceType, Interface* serviceInstance )
{
	validateServiceInstance( serviceType, serviceInstance );
	_services[serviceType].global.setInstance( serviceInstance );
}

void ServiceManager::addServiceForType( IInterfaceType* serviceType, IInterfaceType* clientType, Interface* serviceInstance )
{
	validateClientType( clientType );
	validateServiceInstance( serviceType, serviceInstance );
	_services[serviceType].custom[clientType].setInstance( serviceInstance );
}

void ServiceManager::addServiceImplementation( IInterfaceType* serviceType, const std::string& componentName )
{
	IComponentType* ct = validateComponentType( serviceType, componentName );

	LazyInstance& instance = _services[serviceType].global;
	instance.setComponentType( ct );

	if( !_isLazy )
		createServiceInstance( serviceType, instance );
}

void ServiceManager::addServiceImplementationForType( IInterfaceType* serviceType, IInterfaceType* clientType, const std::string& componentName )
{
	validateClientType( clientType );
	IComponentType* ct = validateComponentType( serviceType, componentName );

	LazyInstance& instance = _services[serviceType].custom[clientType];
	instance.setComponentType( ct );

	if( !_isLazy )
		createServiceInstance( serviceType, instance, clientType );
}

ServiceManager::ServiceRecord& ServiceManager::fetchServiceRecord( IInterfaceType* serviceType )
{
	ServiceMap::iterator it = _services.find( serviceType );
	if( it == _services.end() )
		throw MissingServiceException( "unknown service" );
	return it->second;
}

Interface* ServiceManager::getService( IInterfaceType* serviceType )
{
	CHECK_NOT_NULL( serviceType );

	ServiceRecord& rec = fetchServiceRecord( serviceType );
	if( !rec.global.isValid() )
		throw MissingServiceException( "service has no global instance" );

	createServiceInstance( serviceType, rec.global );

	return rec.global.getInstance();
}

Interface* ServiceManager::getServiceForType( IInterfaceType* serviceType, IInterfaceType* clientType )
{
	CHECK_NOT_NULL( serviceType );
	CHECK_NOT_NULL( clientType );
	return getServiceForType( fetchServiceRecord( serviceType ), serviceType, clientType );
}

IInterfaceInfo* ServiceManager::getFacetInfo( IComponentType* ct, IInterfaceType* itfType )
{
	ArrayRange<IInterfaceInfo* const> facets = ct->getFacets();
	for( ; facets; facets.popFirst() )
	{
		if( facets.getFirst()->getType()->isSubTypeOf( itfType ) )
			return facets.getFirst();
	}
	return NULL;
}

Interface* ServiceManager::getServiceForInstance( IInterfaceType* serviceType, Interface* clientInstance )
{
	CHECK_NOT_NULL( serviceType );
	CHECK_NOT_NULL( clientInstance );

	// give preference the component's instance of the service interface, if available
	IComponent* component = clientInstance->getInterfaceOwner();
	IComponentType* ct = component->getComponentType();
	IInterfaceInfo* itfInfo = getFacetInfo( ct, serviceType );
	if( itfInfo )
		return component->getInterface( itfInfo );

	// otherwise, fall back to getServiceForType()
	return getServiceForType( fetchServiceRecord( serviceType ), serviceType, clientInstance->getInterfaceType() );
}

void ServiceManager::removeService( IInterfaceType* serviceType )
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

void ServiceManager::removeServiceForType( IInterfaceType* serviceType, IInterfaceType* clientType )
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

void ServiceManager::validateClientType( IInterfaceType* clientType )
{
	CHECK_NOT_NULL( clientType );

	// is clientType the co.Interface interface?
	if( clientType->getSuperInterfaces().isEmpty() )
		throw IllegalArgumentException( "illegal clientType (co.Interface)" );
}

void ServiceManager::validateServiceInstance( IInterfaceType* serviceType, Interface* serviceInstance )
{
	CHECK_NOT_NULL( serviceType );
	CHECK_NOT_NULL( serviceInstance );
	if( !serviceInstance->getInterfaceType()->isSubTypeOf( serviceType ) )
		CORAL_THROW( IllegalArgumentException, "invalid serviceInstance (not a " << serviceType->getFullName() << ")" );
}

IComponentType* ServiceManager::validateComponentType( IInterfaceType* serviceType, const std::string& componentName )
{
	CHECK_NOT_NULL( serviceType );

	IComponentType* ct = NULL;
	try
	{
		ct = dynamic_cast<IComponentType*>( getType( componentName ) );
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

void ServiceManager::createServiceInstance( IInterfaceType* serviceType, LazyInstance& instance, IInterfaceType* clientType )
{
	if( instance.hasInstance() )
		return;

	IComponentType* ct = instance.getComponentType();

	try
	{
		IReflector* reflector = ct->getReflector();
		IComponent* component = reflector->newInstance();
		IInterfaceInfo* serviceItfInfo = getFacetInfo( ct, serviceType );
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

Interface* ServiceManager::getServiceForType( ServiceRecord& rec, IInterfaceType* serviceType, IInterfaceType* clientType )
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

ServiceManager::LazyInstance* ServiceManager::findSuitableInstance( ServiceRecord& rec, IInterfaceType* serviceType, IInterfaceType* clientType )
{
	ArrayRange<IInterfaceType* const> superInterfaces = clientType->getSuperInterfaces();
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

CORAL_EXPORT_COMPONENT( ServiceManager, ServiceManager );

} // namespace co
