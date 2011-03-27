/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#include "TypeTransaction.h"
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

TypeTransaction*
	TypeTransaction::sm_activeTransaction( NULL );

TypeTransaction::TypeTransaction()
{
	if( sm_activeTransaction )
	{
		debug( Dbg_Fatal, "Attempt to instantiate a ITypeTransaction while another "
			"instance is active. Concurrent type creation is unsafe and disallowed in this Coral version." );

		CORAL_THROW( NotSupportedException,
			"Only a single ITypeTransaction instance may exist at any moment in time" );
	}

	sm_activeTransaction = this;

	_commitAttempted = false;
	_commitSucceeded = false;
	_rolledBack = false;
}

TypeTransaction::~TypeTransaction()
{
	if( !_commitSucceeded && !_rolledBack )
		debug( Dbg_Critical, "ITypeTransaction not committed nor rolled back." );
}

void TypeTransaction::addTypeBuilder( ITypeBuilder* typeBuilder )
{
	_typeBuilders.push_back( typeBuilder );
}

Range<ITypeBuilder* const> TypeTransaction::getTypeBuilders()
{
	return _typeBuilders;
}

void TypeTransaction::commit()
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

void TypeTransaction::rollback()
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

CORAL_EXPORT_COMPONENT( TypeTransaction, TypeTransaction );

} // namespace co
