/*
 * Coral - A C++ Component Framework.
 * See Copyright Notice in Coral.h
 */

#include "ModuleManager.h"
#include "Module.h"
#include "ModulePartLoader.h"
#include <co/Coral.h>
#include <co/System.h>
#include <co/Namespace.h>
#include <co/SystemState.h>
#include <co/TypeManager.h>
#include <co/LifeCycleException.h>
#include <co/ModuleLoadException.h>
#include <co/IllegalArgumentException.h>
#include <algorithm>
#include <sstream>
#include <iostream>

ModuleManager::ModuleManager()
{
	// empty
}

ModuleManager::~ModuleManager()
{
	// empty
}

void ModuleManager::initialize()
{
	// install the default ModulePartLoader
	_loaders.push_back( new ModulePartLoader );
}

void ModuleManager::updateModules( co::ModuleState state )
{
	for( ModuleList::iterator it = _modules.begin(); it != _modules.end(); ++it )
	{
		co::Module* module = it->get();
		try
		{
			updateModule( module, state );
		}
		catch( std::exception& e )
		{
			module->abort();

			// TODO call a handler function instead
			std::cerr << "Module '" << module->getNamespace()->getFullName()
						 << "' aborted due to exception: " << e.what();
		}
	}
}

co::ArrayRange<co::Module* const> ModuleManager::getModules()
{
	return _modules;
}

co::ArrayRange<co::ModulePartLoader* const> ModuleManager::getLoaders()
{
	return _loaders;
}

co::Module* ModuleManager::findModule( const std::string& moduleName )
{
	co::Namespace* ns = co::getSystem()->getTypes()->findNamespace( moduleName );
	return ns ? ns->getModule() : NULL;
}

void ModuleManager::installLoader( co::ModulePartLoader* loader )
{
	if( !loader )
		throw co::IllegalArgumentException( "illegal null loader" );

	// assert there are no duplicates
	assert( std::find( _loaders.begin(), _loaders.end(), loader ) == _loaders.end() );

	_loaders.push_back( loader );
}

void ModuleManager::uninstallLoader( co::ModulePartLoader* loader )
{
	ModulePartLoaderList::iterator newEnd = std::remove( _loaders.begin(), _loaders.end(), loader );

	if( newEnd == _loaders.end() )
		throw co::IllegalArgumentException( "the specified loader was not found" );

	_loaders.erase( newEnd, _loaders.end() );
}

bool ModuleManager::isLoadable( const std::string& moduleName )
{
	for( ModulePartLoaderList::iterator it = _loaders.begin(); it != _loaders.end(); ++it )
		if( it->get()->canLoadModulePart( moduleName ) )
			return true;

	return false;
}

co::Module* ModuleManager::load( const std::string& moduleName )
{
	co::SystemState systemState = co::getSystem()->getState();

	if( systemState < co::SystemState_Initializing )
		throw co::LifeCycleException( "cannot load modules before the system is set up" );

	if( systemState > co::SystemState_Running )
		throw co::LifeCycleException( "cannot load modules while the system is being torn down" );

	if( _loaders.empty() )
		throw co::ModuleLoadException( "there are no installed module loaders" );

	// check if the module was already loaded
	co::Module* alreadyLoaded = findModule( moduleName );
	if( alreadyLoaded )
		return alreadyLoaded;

	// load ModuleParts
	Module* module = NULL;
	for( ModulePartLoaderList::iterator it = _loaders.begin(); it != _loaders.end(); ++it )
	{
		co::ModulePartLoader* loader = it->get();
		if( loader->canLoadModulePart( moduleName ) )
		{
			try
			{
				co::RefPtr<co::ModulePart> part = loader->loadModulePart( moduleName );
				if( !part.isValid() )
					throw co::Exception( "loader returned a null ModulePart" );

				// create the Module on demand
				if( !module )
					module = createModule( moduleName );

				module->addPart( part.get() );
			}
			catch( std::exception& e )
			{
				// if a part cannot be loaded, the whole module is aborted
				if( module )
					module->abort();

				CORAL_THROW( co::ModuleLoadException, "error loading module '" << moduleName << "': " << e.what() );
			}
		}
	}

	if( !module )
		CORAL_THROW( co::ModuleLoadException, "none of the module loaders recognized '"
						<< moduleName << "' as a module" );

	// initialize the module
	try
	{
		module->initialize();
	}
	catch( std::exception& e )
	{
		module->abort();
		CORAL_THROW( co::ModuleLoadException, "exception raised by module '" << moduleName
						<< "' during initialization: " << e.what() );
	}

	verifyModuleIntegrity( module );
	syncModuleWithSystemState( module );

	return module;
}

Module* ModuleManager::createModule( const std::string& moduleName )
{
	Module* module = new Module;

	// add it to the list of loaded modules
	_modules.push_back( module );

	module->initialize( moduleName );

	return module;
}

void ModuleManager::updateModule( co::Module* module, co::ModuleState toState )
{
	assert( toState > co::ModuleState_None && toState < co::ModuleState_Aborted );

	co::ModuleState fromState = module->getState();
	if( fromState >= co::ModuleState_Disposed )
		return;

	if( fromState < co::ModuleState_Initialized )
		module->initialize();

	if( toState >= co::ModuleState_Integrated && fromState < co::ModuleState_Integrated )
		module->integrate();

	if( toState >= co::ModuleState_PresentationIntegrated && fromState < co::ModuleState_PresentationIntegrated )
		module->integratePresentation();

	if( toState >= co::ModuleState_Disintegrated && fromState < co::ModuleState_Disintegrated )
		module->disintegrate();

	if( toState >= co::ModuleState_Disposed )
		module->dispose();
}

void ModuleManager::verifyModuleIntegrity( co::Module* module )
{
	// all module types must have their reflectors installed
	co::ArrayRange<co::Type* const> types = module->getNamespace()->getTypes();
	try
	{
		for( ; types; types.popFirst() )
		{
			if( types.getFirst()->getReflector() == NULL )
			{
				/*
					Actually, if the type has no reflector, getReflector() should
					raise an exception. But just in case, we'll also check if it's NULL.
				 */
				throw std::exception();
			}
		}
	}
	catch( std::exception& )
	{
		assert( !types.isEmpty() );
		module->abort();
		CORAL_THROW( co::ModuleLoadException, "module '"
				<< module->getNamespace()->getFullName()
				<< "' was aborted because it did not provide a reflector for type '"
				<< types.getFirst()->getFullName() << "'" );
	}
}

void ModuleManager::syncModuleWithSystemState( co::Module* module )
{
	try
	{
		switch( co::getSystem()->getState() )
		{
			case co::SystemState_Initializing:
				// module is already initialized
				break;
			case co::SystemState_Integrating:
			case co::SystemState_Integrated:
				updateModule( module, co::ModuleState_Integrated );
				break;
			case co::SystemState_IntegratingPresentation:
			case co::SystemState_Running:
				updateModule( module, co::ModuleState_PresentationIntegrated );
				break;
			default:
				assert( false );
		}
	}
	catch( std::exception& e )
	{
		module->abort();
		CORAL_THROW( co::ModuleLoadException, "exception raised by module '"
						<< module->getNamespace()->getFullName()
						<< "' during update: " << e.what() );
	}
}

CORAL_EXPORT_COMPONENT( ModuleManager, ModuleManagerComponent );
