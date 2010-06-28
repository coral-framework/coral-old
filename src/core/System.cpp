/*
 * Coral - A C++ Component Framework.
 * See Copyright Notice in Coral.h
 */

#include "System.h"
#include <co/LifeCycleException.h>

System::System()
{
	_state = co::SystemState_None;
	_types = new TypeManager;
	_modules = new ModuleManager;
	_services = new ServiceManager;
}

System::~System()
{
	// empty
}

void System::initialize()
{
	_types->initialize();
	_modules->initialize();
	_services->initialize();
}

co::SystemState System::getState()
{
	return _state;
}

co::TypeManager* System::getTypes()
{
	return _types.get();
}

co::ModuleManager* System::getModules()
{
	return _modules.get();
}

co::ServiceManager* System::getServices()
{
	return _services.get();
}

void System::setupBase( co::ArrayRange<std::string const> requiredModules )
{
	if( _state != co::SystemState_None )
		throw co::LifeCycleException( "the system's state is not SystemState_None" );

	_state = co::SystemState_Initializing;

	try
	{
		for( ; requiredModules; requiredModules.popFirst() )
			_modules->load( requiredModules.getFirst() );
	}
	catch( ... )
	{
		_state = co::SystemState_None;
		throw;
	}
	
	_state = co::SystemState_Integrating;

	_modules->updateModules( co::ModuleState_Integrated );

	_state = co::SystemState_Integrated;
}

void System::setupPresentation()
{
	if( _state != co::SystemState_Integrated )
		throw co::LifeCycleException( "the system's state is not SystemState_Integrated" );

	_state = co::SystemState_IntegratingPresentation;

	_modules->updateModules( co::ModuleState_PresentationIntegrated );

	_state = co::SystemState_Running;
}

void System::tearDown()
{
	if( _state != co::SystemState_Running )
		throw co::LifeCycleException( "the system's state is not SystemState_Running" );

	// release all service instances
	_services->tearDown();

	/*
		Module disposals should be the last tear down step, as it may unload shared libs
		(and calling a destructor from an unloaded lib will generally cause a segfault).
	 */
	_modules->updateModules( co::ModuleState_Disposed );

	_state = co::SystemState_None;
}

CORAL_EXPORT_COMPONENT( System, SystemComponent );
