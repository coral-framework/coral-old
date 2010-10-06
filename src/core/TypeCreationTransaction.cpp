/*
 * Coral - A C++ Component Framework.
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
#include <co/UnsupportedOperationException.h>
#include <sstream>
#include <iostream>

TypeCreationTransaction* TypeCreationTransaction::sm_activeTransaction( NULL );

TypeCreationTransaction::TypeCreationTransaction()
{
	if( sm_activeTransaction )
	{
		// TODO call a handler function instead
		std::cerr	<< "Attempt to instantiate a co::TypeCreationTransaction while another instance is active. "
					<< "Concurrent type creation is unsafe and disallowed in this Coral version.\n";

		assert( false );

		CORAL_THROW( co::UnsupportedOperationException,
			"Only a single co::TypeCreationTransaction instance may exist at any moment in time" );
	}

	sm_activeTransaction = this;

	_commitAttempted = false;
	_commitSucceeded = false;
	_rolledBack = false;
}

TypeCreationTransaction::~TypeCreationTransaction()
{
	if( !_commitSucceeded && !_rolledBack )
	{
		// TODO call a handler function instead
		std::cerr << "co::TypeCreationTransaction not committed nor rolled back.";
		assert( false );
	}
}

void TypeCreationTransaction::addTypeBuilder( co::TypeBuilder* typeBuilder )
{
	_typeBuilders.push_back( typeBuilder );
}

co::ArrayRange<co::TypeBuilder* const> TypeCreationTransaction::getTypeBuilders()
{
	return _typeBuilders;
}

void TypeCreationTransaction::commit()
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
		CORAL_THROW( co::UnsupportedOperationException, msg );
	}

	_commitAttempted = true;

	// create all types not yet created
	for( TypeBuilderList::iterator it = _typeBuilders.begin(); it != _typeBuilders.end(); ++it )
	{
		static_cast<TypeBuilder*>( it->get() )->createType();
	}

	TypeManager* tm = dynamic_cast<TypeManager*>( co::getSystem()->getTypes() );
	assert( tm );
	CORAL_UNUSED( tm );

	// perform semantic checks on all types
	for( TypeBuilderList::iterator it = _typeBuilders.begin(); it != _typeBuilders.end(); ++it )
	{
		co::TypeSemanticChecker sc( ( *it )->createType() );
		sc.check();
	}

	_commitSucceeded = true;

	assert( sm_activeTransaction == this );
	sm_activeTransaction = NULL;
}

void TypeCreationTransaction::rollback()
{
	if( _commitSucceeded || _rolledBack )
		CORAL_THROW( co::UnsupportedOperationException, "the transaction is already dead (e.g committed or rolled back)" );

	assert( sm_activeTransaction == this );
	sm_activeTransaction = NULL;

	// destroy all types
	for( TypeBuilderList::iterator it = _typeBuilders.begin(); it != _typeBuilders.end(); ++it )
	{
		static_cast<TypeBuilder*>( it->get() )->destroyType();
	}

	_rolledBack = true;
}

CORAL_EXPORT_COMPONENT( TypeCreationTransaction, TypeCreationTransactionComponent );
