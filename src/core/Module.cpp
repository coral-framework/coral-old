/*
 * Coral - A C++ Component Framework.
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

Module::Module()
{
	_state = co::ModuleState_None;
	_namespace = NULL;
	_priority = 0;
}

Module::~Module()
{
	assert( _state >= co::ModuleState_Disposed );
}

void Module::initialize( const std::string& moduleName )
{
	// get or create a namespace for this module
	co::Namespace* ns = co::getSystem()->getTypes()->getRootNS();

	co::StringTokenizer st( moduleName, "." );
	while( st.nextToken() )
	{
		co::Namespace* childNS = ns->getChildNamespace( st.getToken() );
		if( !childNS )
			childNS = ns->defineChildNamespace( st.getToken() );

		ns = childNS;
	}

	_namespace = ns;

	static_cast<Namespace*>( ns )->setModule( this );
}

void Module::addPart( co::ModulePart* part )
{
	if( _state != co::ModuleState_None )
		throw co::LifeCycleException( "cannot add a ModulePart to a Module after it has been initialized" );

	assert( part );
	_parts.push_back( part );
}

co::ModuleState Module::getState()
{
	return _state;
}

co::Namespace* Module::getNamespace()
{
	assert( _namespace );
	return _namespace;
}

co::ArrayRange<co::ModulePart* const> Module::getParts()
{
	return co::ArrayRange<co::ModulePart* const>();
}

co::int32 Module::getPriority()
{
	return _priority;
}

void Module::setPriority( co::int32 priority )
{
	_priority = priority;
}

void Module::initialize()
{
	if( _state != co::ModuleState_None )
		throw co::LifeCycleException( "the Module's state is not ModuleState_None" );

	for( co::ArrayRange<co::ModulePart*> ar( _parts ); ar; ar.popFirst() )
		ar.getFirst()->initialize( this );

	_state = co::ModuleState_Initialized;
}

void Module::integrate()
{
	if( _state != co::ModuleState_Initialized )
		throw co::LifeCycleException( "the Module's state is not ModuleState_Initialized" );

	for( co::ArrayRange<co::ModulePart*> ar( _parts ); ar; ar.popFirst() )
		ar.getFirst()->integrate( this );

	_state = co::ModuleState_Integrated;
}

void Module::integratePresentation()
{
	if( _state != co::ModuleState_Integrated )
		throw co::LifeCycleException( "the Module's state is not ModuleState_Integrated" );

	for( co::ArrayRange<co::ModulePart*> ar( _parts ); ar; ar.popFirst() )
		ar.getFirst()->integratePresentation( this );

	_state = co::ModuleState_PresentationIntegrated;
}

void Module::disintegrate()
{
	if( _state != co::ModuleState_PresentationIntegrated )
		throw co::LifeCycleException( "the Module's state is not ModuleState_PresentationIntegrated" );

	for( co::ArrayRange<co::ModulePart*> ar( _parts ); ar; ar.popFirst() )
		ar.getFirst()->disintegrate( this );

	_state = co::ModuleState_Disintegrated;
}

void Module::dispose()
{
	if( _state != co::ModuleState_Disintegrated )
		throw co::LifeCycleException( "the Module's state is not ModuleState_Disintegrated" );

	for( co::ArrayRange<co::ModulePart*> ar( _parts ); ar; ar.popFirst() )
		ar.getFirst()->dispose( this );

	_parts.clear();
	co::LibraryManager::flush();

	_state = co::ModuleState_Disposed;
}

void Module::abort()
{
	if( _state >= co::ModuleState_Disintegrated )
		throw co::LifeCycleException( "the Module is being or was already disposed" );

	// just ignore exceptions raised by ModuleParts from this point on
	for( int i = 0; i < 2; ++i ) // this loop is just to avoid duplicating the 'catch' code
	{
		for( co::ArrayRange<co::ModulePart*> ar( _parts ); ar; ar.popFirst() )
		{
			try
			{
				if( i == 0 && _state >= co::ModuleState_Integrated )
					ar.getFirst()->disintegrate( this );

				if( i == 1 )
					ar.getFirst()->dispose( this );
			}
			catch( std::exception& e )
			{
				// TODO call a handler function instead
				std::cerr << "Exception ignored while aborting module '"
							 << _namespace->getFullName() << "': " << e.what();
			}
		}
	}

	_parts.clear();
	co::LibraryManager::flush();

	_state = co::ModuleState_Aborted;
}

CORAL_EXPORT_COMPONENT( Module, ModuleComponent );
