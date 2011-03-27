/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#include "ServiceManager.h"
#include <co/Coral.h>
#include <co/IObject.h>
#include <co/IReflector.h>
#include <co/IComponent.h>
#include <co/IPort.h>
#include <co/IInterface.h>
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

void ServiceManager::addService( IInterface* serviceType, IService* serviceInstance )
{
	validateServiceInstance( serviceType, serviceInstance );
	_services[serviceType].global.setInstance( serviceInstance );
}

void ServiceManager::addServiceForType( IInterface* serviceType, IInterface* clientType, IService* serviceInstance )
{
	validateClientType( clientType );
	validateServiceInstance( serviceType, serviceInstance );
	_services[serviceType].custom[clientType].setInstance( serviceInstance );
}

void ServiceManager::addServiceImplementation( IInterface* serviceType, const std::string& componentName )
{
	IComponent* ct = validateComponentType( serviceType, componentName );

	LazyInstance& instance = _services[serviceType].global;
	instance.setComponentType( ct );

	if( !_isLazy )
		createServiceInstance( serviceType, instance );
}

void ServiceManager::addServiceImplementationForType( IInterface* serviceType, IInterface* clientType, const std::string& componentName )
{
	validateClientType( clientType );
	IComponent* ct = validateComponentType( serviceType, componentName );

	LazyInstance& instance = _services[serviceType].custom[clientType];
	instance.setComponentType( ct );

	if( !_isLazy )
		createServiceInstance( serviceType, instance, clientType );
}

ServiceManager::ServiceRecord& ServiceManager::fetchServiceRecord( IInterface* serviceType )
{
	ServiceMap::iterator it = _services.find( serviceType );
	if( it == _services.end() )
		throw MissingServiceException( "unknown service" );
	return it->second;
}

IService* ServiceManager::getService( IInterface* serviceType )
{
	CHECK_NOT_NULL( serviceType );

	ServiceRecord& rec = fetchServiceRecord( serviceType );
	if( !rec.global.isValid() )
		throw MissingServiceException( "service has no global instance" );

	createServiceInstance( serviceType, rec.global );

	return rec.global.getInstance();
}

IService* ServiceManager::getServiceForType( IInterface* serviceType, IInterface* clientType )
{
	CHECK_NOT_NULL( serviceType );
	CHECK_NOT_NULL( clientType );
	return getServiceForType( fetchServiceRecord( serviceType ), serviceType, clientType );
}

IPort* ServiceManager::getFacetInfo( IComponent* ct, IInterface* itfType )
{
	Range<IPort* const> facets = ct->getFacets();
	for( ; facets; facets.popFirst() )
	{
		if( facets.getFirst()->getType()->isSubTypeOf( itfType ) )
			return facets.getFirst();
	}
	return NULL;
}

IService* ServiceManager::getServiceForInstance( IInterface* serviceType, IService* clientInstance )
{
	CHECK_NOT_NULL( serviceType );
	CHECK_NOT_NULL( clientInstance );

	// give preference the component's instance of the service interface, if available
	IObject* component = clientInstance->getInterfaceOwner();
	IComponent* ct = component->getComponentType();
	IPort* itfInfo = getFacetInfo( ct, serviceType );
	if( itfInfo )
		return component->getInterface( itfInfo );

	// otherwise, fall back to getServiceForType()
	return getServiceForType( fetchServiceRecord( serviceType ), serviceType, clientInstance->getInterfaceType() );
}

void ServiceManager::removeService( IInterface* serviceType )
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

void ServiceManager::removeServiceForType( IInterface* serviceType, IInterface* clientType )
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

void ServiceManager::validateClientType( IInterface* clientType )
{
	CHECK_NOT_NULL( clientType );

	// is clientType the co.IService interface?
	if( clientType->getSuperInterfaces().isEmpty() )
		throw IllegalArgumentException( "illegal clientType (co.IService)" );
}

void ServiceManager::validateServiceInstance( IInterface* serviceType, IService* serviceInstance )
{
	CHECK_NOT_NULL( serviceType );
	CHECK_NOT_NULL( serviceInstance );
	if( !serviceInstance->getInterfaceType()->isSubTypeOf( serviceType ) )
		CORAL_THROW( IllegalArgumentException, "invalid serviceInstance (not a " << serviceType->getFullName() << ")" );
}

IComponent* ServiceManager::validateComponentType( IInterface* serviceType, const std::string& componentName )
{
	CHECK_NOT_NULL( serviceType );

	IComponent* ct = NULL;
	try
	{
		ct = dynamic_cast<IComponent*>( getType( componentName ) );
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

void ServiceManager::createServiceInstance( IInterface* serviceType, LazyInstance& instance, IInterface* clientType )
{
	if( instance.hasInstance() )
		return;

	IComponent* ct = instance.getComponentType();

	try
	{
		IReflector* reflector = ct->getReflector();
		IObject* component = reflector->newInstance();
		IPort* serviceItfInfo = getFacetInfo( ct, serviceType );
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

IService* ServiceManager::getServiceForType( ServiceRecord& rec, IInterface* serviceType, IInterface* clientType )
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

ServiceManager::LazyInstance* ServiceManager::findSuitableInstance( ServiceRecord& rec, IInterface* serviceType, IInterface* clientType )
{
	Range<IInterface* const> superInterfaces = clientType->getSuperInterfaces();
	if( superInterfaces.isEmpty() )
		return NULL; // we reached a co.IService

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
