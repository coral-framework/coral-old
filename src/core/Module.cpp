/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#include "Module.h"
#include "tools/StringTokenizer.h"
#include <co/Coral.h>
#include <co/ISystem.h>
#include <co/INamespace.h>
#include <co/ITypeManager.h>
#include <co/IModulePartLoader.h>
#include <co/LifeCycleException.h>
#include <co/reserved/LibraryManager.h>
#include <cassert>
#include <iostream>

namespace co {

Module::Module()
{
	_state = ModuleState_None;
	_namespace = NULL;
	_rank = 0;
}

Module::~Module()
{
	assert( _state >= ModuleState_Disposed );
}

void Module::initialize( const std::string& moduleName )
{
	// get or create a namespace for this module
	INamespace* ns = getSystem()->getTypes()->getRootNS();

	StringTokenizer st( moduleName, "." );
	while( st.nextToken() )
	{
		INamespace* childNS = ns->getChildNamespace( st.getToken() );
		if( !childNS )
			childNS = ns->defineChildNamespace( st.getToken() );

		ns = childNS;
	}

	_namespace = ns;

	static_cast<Namespace*>( ns )->setModule( this );
}

void Module::addPart( IModulePart* part )
{
	if( _state != ModuleState_None )
		throw LifeCycleException( "cannot add a module part to a module after it has been initialized" );

	assert( part );
	_parts.push_back( part );
}

ModuleState Module::getState()
{
	return _state;
}

INamespace* Module::getNamespace()
{
	assert( _namespace );
	return _namespace;
}

Range<IModulePart* const> Module::getParts()
{
	return _parts;
}

int32 Module::getRank()
{
	return _rank;
}

void Module::setRank( int32 rank )
{
	_rank = rank;
}

void Module::initialize()
{
	if( _state != ModuleState_None )
		throw LifeCycleException( "the module's state is not ModuleState_None" );

	for( Range<IModulePart*> ar( _parts ); ar; ar.popFirst() )
		ar.getFirst()->initialize( this );

	_state = ModuleState_Initialized;
}

void Module::integrate()
{
	if( _state != ModuleState_Initialized )
		throw LifeCycleException( "the module's state is not ModuleState_Initialized" );

	for( Range<IModulePart*> ar( _parts ); ar; ar.popFirst() )
		ar.getFirst()->integrate( this );

	_state = ModuleState_Integrated;
}

void Module::integratePresentation()
{
	if( _state != ModuleState_Integrated )
		throw LifeCycleException( "the module's state is not ModuleState_Integrated" );

	for( Range<IModulePart*> ar( _parts ); ar; ar.popFirst() )
		ar.getFirst()->integratePresentation( this );

	_state = ModuleState_PresentationIntegrated;
}

void Module::disintegrate()
{
	if( _state != ModuleState_PresentationIntegrated )
		throw LifeCycleException( "the module's state is not ModuleState_PresentationIntegrated" );

	for( Range<IModulePart*> ar( _parts ); ar; ar.popFirst() )
		ar.getFirst()->disintegrate( this );

	_state = ModuleState_Disintegrated;
}

void Module::dispose()
{
	if( _state != ModuleState_Disintegrated )
		throw LifeCycleException( "the module's state is not ModuleState_Disintegrated" );

	for( Range<IModulePart*> ar( _parts ); ar; ar.popFirst() )
		ar.getFirst()->dispose( this );

	_parts.clear();
	LibraryManager::flush();

	_state = ModuleState_Disposed;
}

void Module::abort()
{
	if( _state >= ModuleState_Disintegrated )
		throw LifeCycleException( "the module is being or was already disposed" );

	// just ignore exceptions raised by ModuleParts from this point on
	for( int i = 0; i < 2; ++i ) // this loop is just to avoid duplicating the 'catch' code
	{
		for( Range<IModulePart*> ar( _parts ); ar; ar.popFirst() )
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

CORAL_EXPORT_COMPONENT( Module, Module );

} // namespace co
