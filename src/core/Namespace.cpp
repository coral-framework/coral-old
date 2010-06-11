/*
 * Coral - A C++ Component Framework.
 * See Copyright Notice in Coral.h
 */

#include "Namespace.h"
#include "Module.h"
#include "TypeBuilder.h"
#include "TypeCreationTransaction.h"
#include <co/Type.h>
#include <co/IllegalNameException.h>
#include <co/IllegalArgumentException.h>
#include <co/reserved/LexicalUtilities.h>
#include <algorithm>
#include <sstream>

// Helper Comparators:

int typeComparator( co::Type* type, const std::string& name )
{
	return type->getName().compare( name );
}

int namespaceComparator( co::Namespace* ns, const std::string& name )
{
	return ns->getName().compare( name );
}

// --- Namespace ---

Namespace::Namespace() : _parent( 0 )
{
	// empty
}

Namespace::~Namespace()
{
	// empty
}

void Namespace::setParentAndName( co::Namespace* parent, const std::string& name )
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

void Namespace::setModule( co::Module* module )
{
	_module = module;
}

void Namespace::addType( co::Type* type )
{
	assert( type->getNamespace() == this );
	
	const std::string& name = type->getName();

	int pos;
	if( _childNamespaces.sortedFind( name, namespaceComparator, pos ) )
		throwClashingNamespace( name );

	if( !_types.sortedInsert( name, type, typeComparator ) )
		throwClashingType( name );
}

void Namespace::removeType( co::Type* type )
{
	assert( type->getNamespace() == this );

	const std::string& name = type->getName();

	int pos;
	bool typeExists = _types.sortedFind( name, typeComparator, pos );
	assert( typeExists );
	CORAL_UNUSED( typeExists );

	_types.erase( _types.begin() + pos );
}

const std::string& Namespace::getName()
{
	return _name;
}

const std::string& Namespace::getFullName()
{
	return _fullName;
}

co::Namespace* Namespace::getParentNamespace()
{
	return _parent;
}

co::ArrayRange<co::Type* const> Namespace::getTypes()
{
	return _types;
}

co::ArrayRange<co::Namespace* const> Namespace::getChildNamespaces()
{
	return _childNamespaces;
}

co::Module* Namespace::getModule()
{
	return _module.get();
}

co::Type* Namespace::getType( const std::string& name )
{
	int pos;
	if( _types.sortedFind( name, typeComparator, pos ) )
		return _types[pos].get();
	return NULL;
}

co::Namespace* Namespace::getChildNamespace( const std::string& name )
{
	int pos;
	if( _childNamespaces.sortedFind( name, namespaceComparator, pos ) )
		return _childNamespaces[pos].get();
	return NULL;
}

co::TypeBuilder* Namespace::defineType( const std::string& name, co::TypeKind typeKind, co::TypeCreationTransaction* transaction )
{
	if( typeKind <= co::TK_ARRAY || typeKind > co::TK_COMPONENT )
	{
		CORAL_THROW( co::IllegalArgumentException, "'" << typeKind <<  "' is not a user-definable type kind." );
	}

	if( !co::LexicalUtilities::isValidIdentifier( name ) )
	{
		CORAL_THROW( co::IllegalNameException, "'" << name << "' is not a valid identifier." );
	}

	int pos;
	if( _types.sortedFind( name, typeComparator, pos ) && _types[pos]->getFullName() != "co.Interface" )
		throwClashingType( name );

	if( _childNamespaces.sortedFind( name, namespaceComparator, pos ) )
		throwClashingNamespace( name );

	TypeBuilder* tb = TypeBuilder::create( typeKind, this, name );
	
	TypeCreationTransaction* tct = dynamic_cast<TypeCreationTransaction*>( transaction );
	assert( tct );

	tct->addTypeBuilder( tb );

	return tb;
}

co::Namespace* Namespace::defineChildNamespace( const std::string& name )
{
	::Namespace* ns = new ::Namespace();
	ns->setParentAndName( this, name );

	if( !_childNamespaces.sortedInsert( name, ns, namespaceComparator ) )
	{
		delete ns;
		throwClashingNamespace( name );
	}

	return ns;
}

std::string Namespace::getNamespaceName()
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

void Namespace::throwClashingType( const std::string& name )
{
	CORAL_THROW( co::IllegalNameException, getNamespaceName() <<  " already contains a type called '" << name << "'" );
}

void Namespace::throwClashingNamespace( const std::string& name )
{
	CORAL_THROW( co::IllegalNameException, getNamespaceName() << " already contains a namespace called '" << name << "'" );
}

CORAL_EXPORT_COMPONENT( Namespace, NamespaceComponent );
