/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#include "TypeCreationTransaction.h"
#include "TypeBuilder.h"
#include "TypeManager.h"
#include "TypeSemanticChecker.h"
#include <co/Type.h>
#include <co/Coral.h>
#include <co/System.h>
#include <co/MissingInputException.h>
#include <co/NotSupportedException.h>
#include <sstream>
#include <iostream>

namespace co {

TypeCreationTransactionComponent*
	TypeCreationTransactionComponent::sm_activeTransaction( NULL );

TypeCreationTransactionComponent::TypeCreationTransactionComponent()
{
	if( sm_activeTransaction )
	{
		debug( Dbg_Fatal, "Attempt to instantiate a TypeCreationTransaction while another "
			"instance is active. Concurrent type creation is unsafe and disallowed in this Coral version." );

		CORAL_THROW( NotSupportedException,
			"Only a single TypeCreationTransaction instance may exist at any moment in time" );
	}

	sm_activeTransaction = this;

	_commitAttempted = false;
	_commitSucceeded = false;
	_rolledBack = false;
}

TypeCreationTransactionComponent::~TypeCreationTransactionComponent()
{
	if( !_commitSucceeded && !_rolledBack )
		debug( Dbg_Critical, "TypeCreationTransaction not committed nor rolled back." );
}

void TypeCreationTransactionComponent::addTypeBuilder( TypeBuilder* typeBuilder )
{
	_typeBuilders.push_back( typeBuilder );
}

ArrayRange<TypeBuilder* const> TypeCreationTransactionComponent::getTypeBuilders()
{
	return _typeBuilders;
}

void TypeCreationTransactionComponent::commit()
{
	if( _commitAttempted || _rolledBack )
	{
		const char* msg;
		if( _commitAttempted && ( !_commitSucceeded && !_rolledBack ) )
			msg = "the last commit has failed, the transaction can only be rolled back now";
		else if( _commitSucceeded )
			msg = "the transaction was already committed";
		else
			msg = "the transaction was already rolled back";		
		CORAL_THROW( NotSupportedException, msg );
	}

	_commitAttempted = true;

	// create all types not yet created
	for( TypeBuilderList::iterator it = _typeBuilders.begin(); it != _typeBuilders.end(); ++it )
	{
		static_cast<TypeBuilder*>( it->get() )->createType();
	}

	TypeManager* tm = dynamic_cast<TypeManager*>( getSystem()->getTypes() );
	assert( tm );
	CORAL_UNUSED( tm );

	// perform semantic checks on all types
	for( TypeBuilderList::iterator it = _typeBuilders.begin(); it != _typeBuilders.end(); ++it )
	{
		TypeSemanticChecker sc( ( *it )->createType() );
		sc.check();
	}

	_commitSucceeded = true;

	assert( sm_activeTransaction == this );
	sm_activeTransaction = NULL;
}

void TypeCreationTransactionComponent::rollback()
{
	if( _commitSucceeded || _rolledBack )
		CORAL_THROW( NotSupportedException, "the transaction is already dead (e.g committed or rolled back)" );

	assert( sm_activeTransaction == this );
	sm_activeTransaction = NULL;

	// destroy all types
	for( TypeBuilderList::iterator it = _typeBuilders.begin(); it != _typeBuilders.end(); ++it )
	{
		static_cast<TypeBuilderComponent*>( it->get() )->destroyType();
	}

	_rolledBack = true;
}

CORAL_EXPORT_COMPONENT( TypeCreationTransactionComponent, TypeCreationTransactionComponent );

} // namespace co
