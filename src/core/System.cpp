/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#include "System.h"
#include <co/LifeCycleException.h>
#include <co/reserved/LibraryManager.h>

namespace co {

SystemComponent::SystemComponent()
{
	_state = SystemState_None;
	_types = new TypeManagerComponent;
	_modules = new ModuleManagerComponent;
	_services = new ServiceManagerComponent;
}

SystemComponent::~SystemComponent()
{
	// empty
}

void SystemComponent::initialize()
{
	_types->initialize();
	_modules->initialize();
	_services->initialize();
}

SystemState SystemComponent::getState()
{
	return _state;
}

TypeManager* SystemComponent::getTypes()
{
	return _types.get();
}

ModuleManager* SystemComponent::getModules()
{
	return _modules.get();
}

ServiceManager* SystemComponent::getServices()
{
	return _services.get();
}

void SystemComponent::setupBase( ArrayRange<std::string const> requiredModules )
{
	if( _state != SystemState_None )
		throw LifeCycleException( "the system's state is not SystemState_None" );

	_state = SystemState_Initializing;

	try
	{
		for( ; requiredModules; requiredModules.popFirst() )
			_modules->load( requiredModules.getFirst() );
	}
	catch( ... )
	{
		_state = SystemState_None;
		throw;
	}
	
	_state = SystemState_Integrating;

	_modules->updateModules( ModuleState_Integrated );

	_state = SystemState_Integrated;
}

void SystemComponent::setupPresentation()
{
	if( _state != SystemState_Integrated )
		throw LifeCycleException( "the system's state is not SystemState_Integrated" );

	_state = SystemState_IntegratingPresentation;

	_modules->updateModules( ModuleState_PresentationIntegrated );

	_state = SystemState_Running;
}

void SystemComponent::tearDown()
{
	if( _state != SystemState_Running )
		throw LifeCycleException( "the system's state is not SystemState_Running" );

	// dispose all modules
	_modules->updateModules( ModuleState_Disintegrated );
	_modules->updateModules( ModuleState_Disposed );

	// release the remaining service instances
	_services->tearDown();

	// make sure all released library instances are unloaded
	LibraryManager::flush();

	_state = SystemState_None;
}

CORAL_EXPORT_COMPONENT( SystemComponent, SystemComponent );

} // namespace co
