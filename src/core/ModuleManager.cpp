/*
 * Coral - A lightweight C++ component framework
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

namespace co {

ModuleManagerComponent::ModuleManagerComponent()
{
	_binaryCompatibilityChecking = true;
}

ModuleManagerComponent::~ModuleManagerComponent()
{
	// empty
}

void ModuleManagerComponent::initialize()
{
	// install the default ModulePartLoader
	_loaders.push_back( new ModulePartLoaderComponent );
}

static bool sortByIncreasingRank( Module* a, Module* b )
{
	return a->getRank() < b->getRank();
}

void ModuleManagerComponent::updateModules( ModuleState state )
{
	size_t numModules = _modules.size();

	std::vector<Module*> sortedModules;
	sortedModules.reserve( numModules );

	for( size_t i = 0; i < numModules; ++i )
		sortedModules.push_back( _modules[i].get() );

	std::sort( sortedModules.begin(), sortedModules.end(), sortByIncreasingRank );

	for( size_t i = 0; i < numModules; ++i )
	{
		Module* module = sortedModules[i];
		try
		{
			updateModule( module, state );
		}
		catch( std::exception& e )
		{
			module->abort();
			debug( Dbg_Warning, "Module '%s' aborted due to exception: %s",
				module->getNamespace()->getFullName().c_str(), e.what() );
		}
	}
}

ArrayRange<Module* const> ModuleManagerComponent::getModules()
{
	return _modules;
}

ArrayRange<ModulePartLoader* const> ModuleManagerComponent::getLoaders()
{
	return _loaders;
}

bool ModuleManagerComponent::getBinaryCompatibilityChecking()
{
	return _binaryCompatibilityChecking;
}

void ModuleManagerComponent::setBinaryCompatibilityChecking( bool enabled )
{
	_binaryCompatibilityChecking = enabled;
}

Module* ModuleManagerComponent::findModule( const std::string& moduleName )
{
	Namespace* ns = getSystem()->getTypes()->findNamespace( moduleName );
	return ns ? ns->getModule() : NULL;
}

void ModuleManagerComponent::installLoader( ModulePartLoader* loader )
{
	if( !loader )
		throw IllegalArgumentException( "illegal null loader" );

	// assert there are no duplicates
	assert( std::find( _loaders.begin(), _loaders.end(), loader ) == _loaders.end() );

	_loaders.push_back( loader );
}

void ModuleManagerComponent::uninstallLoader( ModulePartLoader* loader )
{
	ModulePartLoaderList::iterator newEnd = std::remove( _loaders.begin(), _loaders.end(), loader );

	if( newEnd == _loaders.end() )
		throw IllegalArgumentException( "the specified loader was not found" );

	_loaders.erase( newEnd, _loaders.end() );
}

bool ModuleManagerComponent::isLoadable( const std::string& moduleName )
{
	for( ModulePartLoaderList::iterator it = _loaders.begin(); it != _loaders.end(); ++it )
		if( it->get()->canLoadModulePart( moduleName ) )
			return true;

	return false;
}

Module* ModuleManagerComponent::load( const std::string& moduleName )
{
	SystemState systemState = getSystem()->getState();

	if( systemState < SystemState_Initializing )
		throw LifeCycleException( "cannot load modules before the system is set up" );

	if( systemState > SystemState_Running )
		throw LifeCycleException( "cannot load modules while the system is being torn down" );

	if( _loaders.empty() )
		throw ModuleLoadException( "there are no installed module loaders" );

	// check if the module was already loaded
	Module* alreadyLoaded = findModule( moduleName );
	if( alreadyLoaded )
		return alreadyLoaded;

	// load ModuleParts
	ModuleComponent* module = NULL;
	for( ModulePartLoaderList::iterator it = _loaders.begin(); it != _loaders.end(); ++it )
	{
		ModulePartLoader* loader = it->get();
		if( loader->canLoadModulePart( moduleName ) )
		{
			try
			{
				RefPtr<ModulePart> part = loader->loadModulePart( moduleName );
				if( !part.isValid() )
					throw Exception( "loader returned a null ModulePart" );

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

				CORAL_THROW( ModuleLoadException, "error loading module '" << moduleName << "': " << e.what() );
			}
		}
	}

	if( !module )
		CORAL_THROW( ModuleLoadException, "none of the module loaders recognized '"
						<< moduleName << "' as a module" );

	// initialize the module
	try
	{
		module->initialize();
	}
	catch( std::exception& e )
	{
		module->abort();
		CORAL_THROW( ModuleLoadException, "exception raised by module '" << moduleName
						<< "' during initialization: " << e.what() );
	}

	verifyModuleIntegrity( module );
	syncModuleWithSystemState( module );

	return module;
}

ModuleComponent* ModuleManagerComponent::createModule( const std::string& moduleName )
{
	ModuleComponent* module = new ModuleComponent;

	// add it to the list of loaded modules
	_modules.push_back( module );

	module->initialize( moduleName );

	return module;
}

void ModuleManagerComponent::updateModule( Module* module, ModuleState toState )
{
	assert( toState > ModuleState_None && toState < ModuleState_Aborted );

	ModuleState fromState = module->getState();
	if( fromState >= ModuleState_Disposed )
		return;

	if( fromState < ModuleState_Initialized )
		module->initialize();

	if( toState >= ModuleState_Integrated && fromState < ModuleState_Integrated )
		module->integrate();

	if( toState >= ModuleState_PresentationIntegrated && fromState < ModuleState_PresentationIntegrated )
		module->integratePresentation();

	if( toState >= ModuleState_Disintegrated && fromState < ModuleState_Disintegrated )
		module->disintegrate();

	if( toState >= ModuleState_Disposed )
		module->dispose();
}

void ModuleManagerComponent::verifyModuleIntegrity( Module* module )
{
	// all module types must have their reflectors installed
	ArrayRange<Type* const> types = module->getNamespace()->getTypes();
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
	catch( std::exception& e )
	{
		assert( !types.isEmpty() );
		module->abort();
		CORAL_THROW( ModuleLoadException, "module '"
				<< module->getNamespace()->getFullName()
				<< "' was aborted because it did not provide a reflector for type '"
				<< types.getFirst()->getFullName() << "' (" << e.what() << ")" );
	}
}

void ModuleManagerComponent::syncModuleWithSystemState( Module* module )
{
	try
	{
		switch( getSystem()->getState() )
		{
			case SystemState_Initializing:
				// module is already initialized
				break;
			case SystemState_Integrating:
			case SystemState_Integrated:
				updateModule( module, ModuleState_Integrated );
				break;
			case SystemState_IntegratingPresentation:
			case SystemState_Running:
				updateModule( module, ModuleState_PresentationIntegrated );
				break;
			default:
				assert( false );
		}
	}
	catch( std::exception& e )
	{
		module->abort();
		CORAL_THROW( ModuleLoadException, "exception raised by module '"
						<< module->getNamespace()->getFullName()
						<< "' during update: " << e.what() );
	}
}

CORAL_EXPORT_COMPONENT( ModuleManagerComponent, ModuleManagerComponent );

} // namespace co
