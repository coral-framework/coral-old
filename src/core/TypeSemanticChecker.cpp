/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#include "TypeSemanticChecker.h"
#include "ClassType.h"
#include <co/Range.h>
#include <co/IMethod.h>
#include <co/ICompositeType.h>
#include <co/IField.h>
#include <co/IInterface.h>
#include <co/SemanticException.h>
#include <co/reserved/LexicalUtils.h>
#include <sstream>

namespace co {

TypeSemanticChecker::TypeSemanticChecker( IType* type ) : _type( type )
{
	// empty
}

void TypeSemanticChecker::check()
{
	// we don't need to check non-compound types
	if( dynamic_cast<ICompositeType*>( _type ) == NULL )
		return;

	co::IInterface* itf = dynamic_cast<IInterface*>( _type );
	if( itf )
		checkInheritance( itf );

	checkMemberDeclarations( _type );
}

void TypeSemanticChecker::checkInheritance( co::IInterface* itf )
{
	// assert no multiple inheritance from the same type
	co::Range<co::IInterface* const> superTypes = itf->getSuperInterfaces();
	for( ; superTypes; superTypes.popFirst() )
	{
		co::IInterface* currentSuper = superTypes.getFirst();

		// check for cyclic inheritance
		if( currentSuper->isSubTypeOf( itf ) )
		{
			CORAL_THROW( SemanticException, "cyclic inheritance detected in type '"
				<< itf->getFullName() << "' through supertype '"
				<< currentSuper->getFullName() << "'" );
		}

		/*
			Check for redundant direct inheritances, excluding this super-type from the list.
			This method actually checks if theres more than one way of reaching an ancestral
			(i.e. indirect super-type) from the _type's list of direct super-types.
		 */
		co::Range<co::IInterface* const> subList( superTypes );
		for( subList.popFirst(); subList; subList.popFirst() )
		{
			// check if there is more than one way of reaching a super-type
			co::IInterface* anotherSuper = subList.getFirst();

			co::IInterface* toBlame = NULL;
			if( anotherSuper->isSubTypeOf( currentSuper ) )
				toBlame = currentSuper;
			else if( currentSuper->isSubTypeOf( anotherSuper ) )
				toBlame = anotherSuper;

			if( toBlame )
			{
				CORAL_THROW( SemanticException, "type '" << itf->getFullName()
					<< "' inherits more than once from supertype '" << toBlame->getFullName() << "'" );
			}
		}
	}
}

std::ostream& operator<<( std::ostream& out, const co::TypeSemanticChecker::MemberDeclaration& md )
{
	const char* memberType = NULL;
	switch( md.getMemberType() )
	{
	case co::TypeSemanticChecker::Field:		memberType = "field "; break;
	case co::TypeSemanticChecker::FieldGetter:	memberType = "field getter "; break;
	case co::TypeSemanticChecker::FieldSetter:	memberType = "field setter "; break;
	case co::TypeSemanticChecker::Method:		memberType = "method "; break;
	default:
		assert( false );
	}

	return out << memberType << "'" << md.getMemberName() << "' (from '"
				<< md.getDeclaringType()->getFullName() << "')";
}

void TypeSemanticChecker::insertMemberDeclaration( const MemberDeclaration& memberDeclaration )
{
	MemberSet::iterator it = _memberDeclarations.find( memberDeclaration );
	if( it != _memberDeclarations.end() )
	{
		CORAL_THROW( SemanticException, "name clash in type '" << _type->getFullName()
						<< "' between " << *it << " and " << memberDeclaration );
	}

	_memberDeclarations.insert( memberDeclaration );
}

void TypeSemanticChecker::insertFieldDeclaration( co::IField* field, co::IType* declaringType )
{
	insertMemberDeclaration( MemberDeclaration( Field,  field->getName(), declaringType ) );

	std::string name;
	co::LexicalUtils::formatAccessor( field->getName(), co::LexicalUtils::Getter, name );
	insertMemberDeclaration( MemberDeclaration( FieldGetter, name, declaringType ) );

	if( field->getIsReadOnly() )
		return;

	co::LexicalUtils::formatAccessor( field->getName(), co::LexicalUtils::Setter, name );
	insertMemberDeclaration( MemberDeclaration( FieldSetter, name, declaringType ) );
}

void TypeSemanticChecker::checkMemberDeclarations( co::IType* type )
{
	TypeSet::iterator it = _visitedTypes.find( type );
	if( it != _visitedTypes.end() )
		return;

	_visitedTypes.insert( type );

	IClassType* classType = dynamic_cast<IClassType*>( type );
	if( classType )
	{
		co::Range<co::IMethod* const> methods = classType->getMethods();
		for( ; methods; methods.popFirst() )
			insertMemberDeclaration( MemberDeclaration( Method, methods.getFirst()->getName(), type ) );

		co::Range<co::IField* const> fields = classType->getFields();
		for( ; fields; fields.popFirst() )
			insertFieldDeclaration( fields.getFirst(), type );
	}

	co::IInterface* itf = dynamic_cast<co::IInterface*>( type );
	if( itf )
	{
		co::Range<co::IInterface* const> superTypes = itf->getSuperInterfaces();
		for( ; superTypes; superTypes.popFirst() )
			checkMemberDeclarations( superTypes.getFirst() );
	}
}

} // namespace co
