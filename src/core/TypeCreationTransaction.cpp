/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#include "TypeCreationTransaction.h"
#include "TypeBuilder.h"
#include "TypeManager.h"
#include "TypeSemanticChecker.h"
#include <co/IType.h>
#include <co/Coral.h>
#include <co/ISystem.h>
#include <co/MissingInputException.h>
#include <co/NotSupportedException.h>
#include <sstream>
#include <iostream>

namespace co {

TypeCreationTransaction*
	TypeCreationTransaction::sm_activeTransaction( NULL );

TypeCreationTransaction::TypeCreationTransaction()
{
	if( sm_activeTransaction )
	{
		debug( Dbg_Fatal, "Attempt to instantiate a ITypeCreationTransaction while another "
			"instance is active. Concurrent type creation is unsafe and disallowed in this Coral version." );

		CORAL_THROW( NotSupportedException,
			"Only a single ITypeCreationTransaction instance may exist at any moment in time" );
	}

	sm_activeTransaction = this;

	_commitAttempted = false;
	_commitSucceeded = false;
	_rolledBack = false;
}

TypeCreationTransaction::~TypeCreationTransaction()
{
	if( !_commitSucceeded && !_rolledBack )
		debug( Dbg_Critical, "ITypeCreationTransaction not committed nor rolled back." );
}

void TypeCreationTransaction::addTypeBuilder( ITypeBuilder* typeBuilder )
{
	_typeBuilders.push_back( typeBuilder );
}

ArrayRange<ITypeBuilder* const> TypeCreationTransaction::getTypeBuilders()
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
		CORAL_THROW( NotSupportedException, msg );
	}

	_commitAttempted = true;

	// create all types not yet created
	for( TypeBuilderList::iterator it = _typeBuilders.begin(); it != _typeBuilders.end(); ++it )
	{
		static_cast<ITypeBuilder*>( it->get() )->createType();
	}

	ITypeManager* tm = dynamic_cast<ITypeManager*>( getSystem()->getTypes() );
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

void TypeCreationTransaction::rollback()
{
	if( _commitSucceeded || _rolledBack )
		CORAL_THROW( NotSupportedException, "the transaction is already dead (e.g committed or rolled back)" );

	assert( sm_activeTransaction == this );
	sm_activeTransaction = NULL;

	// destroy all types
	for( TypeBuilderList::iterator it = _typeBuilders.begin(); it != _typeBuilders.end(); ++it )
	{
		static_cast<TypeBuilder*>( it->get() )->destroyType();
	}

	_rolledBack = true;
}

CORAL_EXPORT_COMPONENT( TypeCreationTransaction, TypeCreationTransaction );

} // namespace co
