/*
 * Coral - Lightweight C++ Component Framework
 * See Copyright Notice in Coral.h
 */

#include "Namespace.h"
#include "Module.h"
#include "TypeBuilder.h"
#include "TypeTransaction.h"
#include <co/IType.h>
#include <co/IllegalNameException.h>
#include <co/IllegalArgumentException.h>
#include <co/reserved/LexicalUtils.h>
#include <sstream>

namespace co {

Namespace::Namespace() : _parent( 0 )
{
	// empty
}

Namespace::~Namespace()
{
	// empty
}

void Namespace::setParentAndName( INamespace* parent, const std::string& name )
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

void Namespace::setModule( IModule* module )
{
	_module = module;
}

void Namespace::addType( IType* type )
{
	assert( type->getNamespace() == this );

	const std::string& name = type->getName();

	if( findChildNamespace( name ) )
		throwClashingNamespace( name );

	if( !_types.sortedInsert( name, type, typeComparator ) )
		throwClashingType( name );
}

void Namespace::removeType( IType* type )
{
	assert( type->getNamespace() == this );

	const std::string& name = type->getName();

	bool removed = _types.sortedRemove( name, typeComparator );
	assert( removed );
	CORAL_UNUSED( removed );
}

const std::string& Namespace::getName()
{
	return _name;
}

const std::string& Namespace::getFullName()
{
	return _fullName;
}

INamespace* Namespace::getParentNamespace()
{
	return _parent;
}

Range<IType* const> Namespace::getTypes()
{
	return _types;
}

Range<INamespace* const> Namespace::getChildNamespaces()
{
	return _childNamespaces;
}

IModule* Namespace::getModule()
{
	return _module.get();
}

IType* Namespace::getType( const std::string& name )
{
	return findType( name );
}

INamespace* Namespace::getChildNamespace( const std::string& name )
{
	return findChildNamespace( name );
}

ITypeBuilder* Namespace::defineType( const std::string& name, TypeKind typeKind, ITypeTransaction* transaction )
{
	if( typeKind <= TK_ARRAY || typeKind > TK_COMPONENT )
		CORAL_THROW( IllegalArgumentException, "'" << typeKind <<  "' is not a user-definable type kind." );

	if( !LexicalUtils::isValidIdentifier( name ) )
		CORAL_THROW( IllegalNameException, "'" << name << "' is not a valid identifier." );

	if( !transaction )
		throw IllegalArgumentException( "illegal null transaction" );

	IType* type;
	if( ( type = findType( name ) ) && type->getFullName() != "co.IService" )
		throwClashingType( name );

	if( findChildNamespace( name ) )
		throwClashingNamespace( name );

	ITypeBuilder* tb = TypeBuilder::create( typeKind, this, name );

	static_cast<TypeTransaction*>( transaction )->addTypeBuilder( tb );

	return tb;
}

INamespace* Namespace::defineChildNamespace( const std::string& name )
{
	Namespace* ns = new Namespace();
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
	CORAL_THROW( IllegalNameException, getNamespaceName() <<  " already contains a type called '" << name << "'" );
}

void Namespace::throwClashingNamespace( const std::string& name )
{
	CORAL_THROW( IllegalNameException, getNamespaceName() << " already contains a namespace called '" << name << "'" );
}

CORAL_EXPORT_COMPONENT( Namespace, Namespace );

} // namespace co
