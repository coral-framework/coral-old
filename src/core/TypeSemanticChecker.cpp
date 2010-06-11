/*
 * Coral - A C++ Component Framework.
 * See Copyright Notice in Coral.h
 */

#include "TypeSemanticChecker.h"
#include "AttributeAndMethodContainer.h"
#include <co/ArrayRange.h>
#include <co/MethodInfo.h>
#include <co/CompoundType.h>
#include <co/AttributeInfo.h>
#include <co/InterfaceType.h>
#include <co/SemanticException.h>
#include <co/reserved/LexicalUtilities.h>
#include <sstream>

namespace co {

TypeSemanticChecker::TypeSemanticChecker( Type* type ) : _type( type )
{
	// empty
}

void TypeSemanticChecker::check()
{
	// we don't need to check non-compound types
	if( dynamic_cast<CompoundType*>( _type ) == NULL )
		return;

	co::InterfaceType* interfaceType = dynamic_cast<InterfaceType*>( _type );
	if( interfaceType )
		checkInheritance( interfaceType );

	checkMemberDeclarations( _type );
}

void TypeSemanticChecker::checkInheritance( co::InterfaceType* interfaceType )
{
	// assert no multiple inheritance from the same type
	co::ArrayRange<co::InterfaceType* const> superTypes = interfaceType->getSuperInterfaces();
	for( ; superTypes; superTypes.popFirst() )
	{
		co::InterfaceType* currentSuper = superTypes.getFirst();

		// check for cyclic inheritance
		if( currentSuper->isSubTypeOf( interfaceType ) )
		{
			CORAL_THROW( SemanticException, "cyclic inheritance detected in type '"
				<< interfaceType->getFullName() << "' through supertype '"
				<< currentSuper->getFullName() << "'" );
		}

		/*
			Check for redundant direct inheritances, excluding this super-type from the list.
			This method actually checks if theres more than one way of reaching an ancestral
			(i.e. indirect super-type) from the _type's list of direct super-types.
		 */
		co::ArrayRange<co::InterfaceType* const> subList( superTypes );
		for( subList.popFirst(); subList; subList.popFirst() )
		{
			// check if there is more than one way of reaching a super-type
			co::InterfaceType* anotherSuper = subList.getFirst();

			co::InterfaceType* toBlame = NULL;
			if( anotherSuper->isSubTypeOf( currentSuper ) )
				toBlame = currentSuper;
			else if( currentSuper->isSubTypeOf( anotherSuper ) )
				toBlame = anotherSuper;

			if( toBlame )
			{
				CORAL_THROW( SemanticException, "type '" << interfaceType->getFullName()
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
	case co::TypeSemanticChecker::Attribute:	memberType = "attribute "; break;
	case co::TypeSemanticChecker::GetterMethod:	memberType = "getter method "; break;
	case co::TypeSemanticChecker::SetterMethod:	memberType = "setter method "; break;
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

void TypeSemanticChecker::insertAttributeDeclaration( co::AttributeInfo* attribute, co::Type* declaringType )
{
	insertMemberDeclaration( MemberDeclaration( Attribute,  attribute->getName(), declaringType ) );

	std::string name;
	co::LexicalUtilities::formatAccessor( attribute->getName(), co::LexicalUtilities::Getter, name );
	insertMemberDeclaration( MemberDeclaration( GetterMethod, name, declaringType ) );

	if( attribute->getIsReadOnly() )
		return;

	co::LexicalUtilities::formatAccessor( attribute->getName(), co::LexicalUtilities::Setter, name );
	insertMemberDeclaration( MemberDeclaration( SetterMethod, name, declaringType ) );
}

void TypeSemanticChecker::checkMemberDeclarations( co::Type* type )
{
	TypeSet::iterator it = _visitedTypes.find( type );
	if( it != _visitedTypes.end() )
		return;

	_visitedTypes.insert( type );

	AttributeAndMethodContainer* container = dynamic_cast<AttributeAndMethodContainer*>( type );

	if( container )
	{
		co::ArrayRange<co::MethodInfo* const> methods = container->getMemberMethods();
		for( ; methods; methods.popFirst() )
			insertMemberDeclaration( MemberDeclaration( Method, methods.getFirst()->getName(), type ) );
	
		co::ArrayRange<co::AttributeInfo* const> attributes = container->getMemberAttributes();
		for( ; attributes; attributes.popFirst() )
			insertAttributeDeclaration( attributes.getFirst(), type );
	}

	co::InterfaceType* interface = dynamic_cast<co::InterfaceType*>( type );
	if( interface )
	{
		co::ArrayRange<co::InterfaceType* const> superTypes = interface->getSuperInterfaces();
		for( ; superTypes; superTypes.popFirst() )
			checkMemberDeclarations( superTypes.getFirst() );
	}
}

} // namespace co
