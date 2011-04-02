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

	checkMemberDeclarations( _type );
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
	if( itf && itf->getBaseType() )
		checkMemberDeclarations( itf->getBaseType() );
}

} // namespace co
