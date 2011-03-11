/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#include "Namespace.h"
#include "Module.h"
#include "TypeBuilder.h"
#include "TypeCreationTransaction.h"
#include <co/Type.h>
#include <co/IllegalNameException.h>
#include <co/IllegalArgumentException.h>
#include <co/reserved/LexicalUtils.h>
#include <algorithm>
#include <sstream>

namespace co {

// ------ Helper Comparators ---------------------------------------------------

inline int typeComparator( Type* type, const std::string& name )
{
	return type->getName().compare( name );
}

inline int namespaceComparator( Namespace* ns, const std::string& name )
{
	return ns->getName().compare( name );
}

// ------ Namespace ------------------------------------------------------------

NamespaceComponent::NamespaceComponent() : _parent( 0 )
{
	// empty
}

NamespaceComponent::~NamespaceComponent()
{
	// empty
}

void NamespaceComponent::setParentAndName( Namespace* parent, const std::string& name )
{
	assert( _fullName.empty() );

	_parent = parent;
	_name = name;

	// compute our 'fullName'
	if( _parent && _parent->getParentNamespace() )
	{
		_fullName = _parent->getFullName();
		_fullName.append( "." );
	}

	_fullName.append( name );
}

void NamespaceComponent::setModule( Module* module )
{
	_module = module;
}

void NamespaceComponent::addType( Type* type )
{
	assert( type->getNamespace() == this );
	
	const std::string& name = type->getName();

	size_t pos;
	if( _childNamespaces.sortedFind( name, namespaceComparator, pos ) )
		throwClashingNamespace( name );

	if( !_types.sortedInsert( name, type, typeComparator ) )
		throwClashingType( name );
}

void NamespaceComponent::removeType( Type* type )
{
	assert( type->getNamespace() == this );

	const std::string& name = type->getName();

	size_t pos;
	bool typeExists = _types.sortedFind( name, typeComparator, pos );
	assert( typeExists );
	CORAL_UNUSED( typeExists );

	_types.erase( _types.begin() + pos );
}

const std::string& NamespaceComponent::getName()
{
	return _name;
}

const std::string& NamespaceComponent::getFullName()
{
	return _fullName;
}

Namespace* NamespaceComponent::getParentNamespace()
{
	return _parent;
}

ArrayRange<Type* const> NamespaceComponent::getTypes()
{
	return _types;
}

ArrayRange<Namespace* const> NamespaceComponent::getChildNamespaces()
{
	return _childNamespaces;
}

Module* NamespaceComponent::getModule()
{
	return _module.get();
}

Type* NamespaceComponent::getType( const std::string& name )
{
	size_t pos;
	if( _types.sortedFind( name, typeComparator, pos ) )
		return _types[pos].get();
	return NULL;
}

Namespace* NamespaceComponent::getChildNamespace( const std::string& name )
{
	size_t pos;
	if( _childNamespaces.sortedFind( name, namespaceComparator, pos ) )
		return _childNamespaces[pos].get();
	return NULL;
}

TypeBuilder* NamespaceComponent::defineType( const std::string& name, TypeKind typeKind, TypeCreationTransaction* transaction )
{
	if( typeKind <= TK_ARRAY || typeKind > TK_COMPONENT )
	{
		CORAL_THROW( IllegalArgumentException, "'" << typeKind <<  "' is not a user-definable type kind." );
	}

	if( !LexicalUtils::isValidIdentifier( name ) )
	{
		CORAL_THROW( IllegalNameException, "'" << name << "' is not a valid identifier." );
	}

	size_t pos;
	if( _types.sortedFind( name, typeComparator, pos ) && _types[pos]->getFullName() != "co.Interface" )
		throwClashingType( name );

	if( _childNamespaces.sortedFind( name, namespaceComparator, pos ) )
		throwClashingNamespace( name );

	TypeBuilder* tb = TypeBuilderComponent::create( typeKind, this, name );

	static_cast<TypeCreationTransactionComponent*>( transaction )->addTypeBuilder( tb );

	return tb;
}

Namespace* NamespaceComponent::defineChildNamespace( const std::string& name )
{
	NamespaceComponent* ns = new NamespaceComponent();
	ns->setParentAndName( this, name );

	if( !_childNamespaces.sortedInsert( name, ns, namespaceComparator ) )
	{
		delete ns;
		throwClashingNamespace( name );
	}

	return ns;
}

std::string NamespaceComponent::getNamespaceName()
{
	if( !_parent )
		return "root namespace";
	
	const char PREFIX[] = "namespace '";
	std::string str;
	str.reserve( CORAL_ARRAY_LENGTH( PREFIX ) + _fullName.length() + 2 );
	str += PREFIX;
	str += _fullName;
	str += '\'';
	return str;
}

void NamespaceComponent::throwClashingType( const std::string& name )
{
	CORAL_THROW( IllegalNameException, getNamespaceName() <<  " already contains a type called '" << name << "'" );
}

void NamespaceComponent::throwClashingNamespace( const std::string& name )
{
	CORAL_THROW( IllegalNameException, getNamespaceName() << " already contains a namespace called '" << name << "'" );
}

CORAL_EXPORT_COMPONENT( NamespaceComponent, NamespaceComponent );

} // namespace co
