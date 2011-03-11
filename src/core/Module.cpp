/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#include "Module.h"
#include "tools/StringTokenizer.h"
#include <co/Coral.h>
#include <co/System.h>
#include <co/Namespace.h>
#include <co/TypeManager.h>
#include <co/ModulePartLoader.h>
#include <co/LifeCycleException.h>
#include <co/reserved/LibraryManager.h>
#include <cassert>
#include <iostream>

namespace co {

ModuleComponent::ModuleComponent()
{
	_state = ModuleState_None;
	_namespace = NULL;
	_rank = 0;
}

ModuleComponent::~ModuleComponent()
{
	assert( _state >= ModuleState_Disposed );
}

void ModuleComponent::initialize( const std::string& moduleName )
{
	// get or create a namespace for this module
	Namespace* ns = getSystem()->getTypes()->getRootNS();

	StringTokenizer st( moduleName, "." );
	while( st.nextToken() )
	{
		Namespace* childNS = ns->getChildNamespace( st.getToken() );
		if( !childNS )
			childNS = ns->defineChildNamespace( st.getToken() );

		ns = childNS;
	}

	_namespace = ns;

	static_cast<NamespaceComponent*>( ns )->setModule( this );
}

void ModuleComponent::addPart( ModulePart* part )
{
	if( _state != ModuleState_None )
		throw LifeCycleException( "cannot add a ModulePart to a Module after it has been initialized" );

	assert( part );
	_parts.push_back( part );
}

ModuleState ModuleComponent::getState()
{
	return _state;
}

Namespace* ModuleComponent::getNamespace()
{
	assert( _namespace );
	return _namespace;
}

ArrayRange<ModulePart* const> ModuleComponent::getParts()
{
	return _parts;
}

int32 ModuleComponent::getRank()
{
	return _rank;
}

void ModuleComponent::setRank( int32 rank )
{
	_rank = rank;
}

void ModuleComponent::initialize()
{
	if( _state != ModuleState_None )
		throw LifeCycleException( "the Module's state is not ModuleState_None" );

	for( ArrayRange<ModulePart*> ar( _parts ); ar; ar.popFirst() )
		ar.getFirst()->initialize( this );

	_state = ModuleState_Initialized;
}

void ModuleComponent::integrate()
{
	if( _state != ModuleState_Initialized )
		throw LifeCycleException( "the Module's state is not ModuleState_Initialized" );

	for( ArrayRange<ModulePart*> ar( _parts ); ar; ar.popFirst() )
		ar.getFirst()->integrate( this );

	_state = ModuleState_Integrated;
}

void ModuleComponent::integratePresentation()
{
	if( _state != ModuleState_Integrated )
		throw LifeCycleException( "the Module's state is not ModuleState_Integrated" );

	for( ArrayRange<ModulePart*> ar( _parts ); ar; ar.popFirst() )
		ar.getFirst()->integratePresentation( this );

	_state = ModuleState_PresentationIntegrated;
}

void ModuleComponent::disintegrate()
{
	if( _state != ModuleState_PresentationIntegrated )
		throw LifeCycleException( "the Module's state is not ModuleState_PresentationIntegrated" );

	for( ArrayRange<ModulePart*> ar( _parts ); ar; ar.popFirst() )
		ar.getFirst()->disintegrate( this );

	_state = ModuleState_Disintegrated;
}

void ModuleComponent::dispose()
{
	if( _state != ModuleState_Disintegrated )
		throw LifeCycleException( "the Module's state is not ModuleState_Disintegrated" );

	for( ArrayRange<ModulePart*> ar( _parts ); ar; ar.popFirst() )
		ar.getFirst()->dispose( this );

	_parts.clear();
	LibraryManager::flush();

	_state = ModuleState_Disposed;
}

void ModuleComponent::abort()
{
	if( _state >= ModuleState_Disintegrated )
		throw LifeCycleException( "the Module is being or was already disposed" );

	// just ignore exceptions raised by ModuleParts from this point on
	for( int i = 0; i < 2; ++i ) // this loop is just to avoid duplicating the 'catch' code
	{
		for( ArrayRange<ModulePart*> ar( _parts ); ar; ar.popFirst() )
		{
			try
			{
				if( i == 0 && _state >= ModuleState_Integrated )
					ar.getFirst()->disintegrate( this );

				if( i == 1 )
					ar.getFirst()->dispose( this );
			}
			catch( std::exception& e )
			{
				debug( Dbg_Critical, "Exception ignored while aborting module '%s': %s",
						_namespace->getFullName().c_str(), e.what() );
			}
		}
	}

	_parts.clear();
	LibraryManager::flush();

	_state = ModuleState_Aborted;
}

CORAL_EXPORT_COMPONENT( ModuleComponent, ModuleComponent );

} // namespace co
