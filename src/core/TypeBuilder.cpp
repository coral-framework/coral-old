/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#include "TypeBuilder.h"

#include "Type.h"
#include "EnumType.h"
#include "Namespace.h"
#include "MethodInfo.h"
#include "StructType.h"
#include "TypeManager.h"
#include "AttributeInfo.h"
#include "MethodBuilder.h"
#include "InterfaceInfo.h"
#include "ExceptionType.h"
#include "ComponentType.h"
#include "InterfaceType.h"
#include "NativeClassType.h"
#include <co/Coral.h>
#include <co/IllegalNameException.h>
#include <co/MissingInputException.h>
#include <co/IllegalArgumentException.h>
#include <co/NotSupportedException.h>
#include <co/reserved/LexicalUtils.h>
#include <algorithm>
#include <sstream>

namespace co {

TypeBuilder::TypeBuilder( TypeKind kind ) : _kind( kind )
{
	_namespace = NULL;
	_typeWasCreated = false;
}

TypeBuilder::~TypeBuilder()
{
   // empty
}

void TypeBuilder::destroyType()
{
	// remove any array created for the type before we destroy it
	IType* arrayType = _namespace->getType( _type->getName() + "[]" );
	if( arrayType )
		_namespace->removeType( arrayType );

	_namespace->removeType( _type.get() );

	_kind = TK_NONE;
	_type = NULL;
}

void TypeBuilder::addMethod( MethodInfo* )
{
	CORAL_THROW( NotSupportedException, "the typeBuilder's kind is neither TK_NATIVECLASS nor TK_INTERFACE" );
}

INamespace* TypeBuilder::getNamespace()
{
	return _namespace.get();
}

TypeKind TypeBuilder::getKind()
{
	return _kind;
}

const std::string& TypeBuilder::getTypeName()
{
	return _name;
}

void TypeBuilder::defineIdentifier(  const std::string& )
{
	CORAL_THROW( NotSupportedException, "the typeBuilder's kind is not TK_ENUM" );
}

void TypeBuilder::defineAttribute( const std::string&, IType*, bool )
{
	CORAL_THROW( NotSupportedException, "the typeBuilder's kind is neither TK_STRUCT, TK_NATIVECLASS nor TK_INTERFACE" );
}

void TypeBuilder::defineSuperType( IType* )
{
	CORAL_THROW( NotSupportedException, "the typeBuilder's kind is not TK_INTERFACE" );
}

void TypeBuilder::defineInterface( const std::string&, IInterfaceType*, bool )
{
	CORAL_THROW( NotSupportedException, "the typeBuilder's kind is not TK_COMPONENT" );
}

IMethodBuilder* TypeBuilder::defineMethod( const std::string& )
{
	CORAL_THROW( NotSupportedException, "the typeBuilder's kind is neither TK_NATIVECLASS nor TK_INTERFACE" );
}

void TypeBuilder::defineNativeClass( const std::string&, const std::string& )
{
	CORAL_THROW( NotSupportedException, "the typeBuilder's kind is not TK_NATIVECLASS" );
}

IType* TypeBuilder::createType()
{
	if( _typeWasCreated )
		return _type.get();

	validate();
	fillType();

	_typeWasCreated = true;

	return _type.get();
}

void TypeBuilder::initialize( INamespace* ns, const std::string& name )
{
	assert( dynamic_cast<Namespace*>( ns ) );
	_namespace = static_cast<Namespace*>( ns );
	_name = name;

	// pre-allocate our empty type
	if( allocateType() )
	{
		TypeImpl* type = dynamic_cast<TypeImpl*>( _type.get() );
		assert( type );

		type->setType( _namespace.get(), _name, _kind );

		// add the type to its namespace (should be removed if it is rolled back)
		_namespace->addType( _type.get() );
	}

	assert( _type.isValid() );
}

void TypeBuilder::assertNotCreated()
{
	if( _typeWasCreated )
		CORAL_THROW( NotSupportedException, "type was already created" );
}

// ------ EnumTypeBuilder ------------------------------------------------------

class EnumTypeBuilder : public TypeBuilder
{
public:
	EnumTypeBuilder() : TypeBuilder( TK_ENUM )
	{
		_myType = NULL;
	}

	bool allocateType()
	{
		assert( _myType == NULL );
		_myType = new EnumType;
		_type = _myType;
		return true;	}

	void validate()
	{
		if( _identifiers.empty() )
			CORAL_THROW( MissingInputException, "missing enum identifiers" );
	}

	void fillType()
	{
		for( ArrayRange<std::string> r( _identifiers ); r; r.popFirst() )
		{
			_myType->addIdentifier( r.getFirst() );
		}
	}

	void defineIdentifier( const std::string& name )
	{
		assertNotCreated();

		if( !LexicalUtils::isValidIdentifier( name ) )
			CORAL_THROW( IllegalNameException, "invalid identifier '" << name << "'" );

		std::vector<std::string>::iterator it = std::find( _identifiers.begin(), _identifiers.end(), name );
		if( it != _identifiers.end() )
			CORAL_THROW( IllegalNameException, "invalid duplicate identifier '" << name << "'" );

		_identifiers.push_back( name );
	}

private:
	EnumType* _myType;
	std::vector<std::string> _identifiers;
};

// ------ ExceptionTypeBuilder -------------------------------------------------

class ExceptionTypeBuilder : public TypeBuilder
{
public:
	ExceptionTypeBuilder() : TypeBuilder( TK_EXCEPTION )
	{
		_myType = NULL;
	}

	bool allocateType()
	{
		assert( _myType == NULL );
		_myType = new ExceptionType;
		_type = _myType;
		return true;
	}

	void validate()
	{
		// empty
	}

	void fillType()
	{
		// empty
	}

private:
	ExceptionType* _myType;
};

// ------ AttributeContainerTypeBuilder ----------------------------------------

class AttributeContainerTypeBuilder : public TypeBuilder
{
public:
	AttributeContainerTypeBuilder( TypeKind kind ) : TypeBuilder( kind )
	{;}

	void defineAttribute( const std::string& name, IType* type, bool isReadOnly )
	{
		assertNotCreated();

		if( !type )
			CORAL_THROW( IllegalArgumentException, "illegal null type" );

		if( type->getKind() == TK_EXCEPTION || type->getKind() == TK_COMPONENT )
			CORAL_THROW( IllegalArgumentException, ( type->getKind() == TK_EXCEPTION ?
					"exception" : "component" ) << "s are illegal as attribute types" );

		// struct-specific checks
		if( _kind == TK_STRUCT )
		{
			if( isReadOnly )
				CORAL_THROW( IllegalArgumentException, "structs cannot have read-only attributes" );

			if( _type == type )
				CORAL_THROW( IllegalArgumentException, "a struct cannot contain itself recursively" );
		}

		if( !LexicalUtils::isValidIdentifier( name ) )
			CORAL_THROW( IllegalNameException, "attribute name '" << name << "' is not a valid identifier" );

		if( !LexicalUtils::isValidAttributeName( name ) )
			CORAL_THROW( IllegalNameException, "attribute names must start with a lowercase letter" );

		size_t count = _attributes.size();
		for( size_t i = 0; i < count; ++i )
		{
			if( _attributes[i]->getName() == name )
				CORAL_THROW( IllegalNameException, "attribute name '" << name << "' clashes with a previous definition" );
		}

		AttributeInfo* attr = new AttributeInfo;
		attr->setType( type );
		attr->setName( name );
		attr->setIsReadOnly( isReadOnly );

		_attributes.push_back( attr );
	}

protected:
	RefVector<IAttributeInfo> _attributes;
};

// ------ StructTypeBuilder ----------------------------------------------------

class StructTypeBuilder : public AttributeContainerTypeBuilder
{
public:
	StructTypeBuilder() : AttributeContainerTypeBuilder( TK_STRUCT )
	{
		_myType = NULL;
	}

	bool allocateType()
	{
		assert( _myType == NULL );
		_myType = new StructType;
		_type = _myType;
		return true;
	}

	void validate()
	{
		if( _attributes.empty() )
			CORAL_THROW( MissingInputException, "missing struct contents" );
	}

	void fillType()
	{
		_myType->addMembers( _attributes );
		_myType->sortMembers( _myType );
	}

private:
	StructType* _myType;
};

// ------ MethodContainerTypeBuilder -------------------------------------------

class MethodContainerTypeBuilder : public AttributeContainerTypeBuilder
{
public:
	MethodContainerTypeBuilder( TypeKind kind ) : AttributeContainerTypeBuilder( kind )
	{;}

	void addMethod( MethodInfo* methodInfo )
	{
		if( _typeWasCreated )
			CORAL_THROW( NotSupportedException, "illegal to add a method after a type is created" );
		_methods.push_back( methodInfo );
	}

	IMethodBuilder* defineMethod( const std::string& name )
	{
		assertNotCreated();

		if( !LexicalUtils::isValidIdentifier( name ) )
			CORAL_THROW( IllegalNameException, "method name '" << name << "' is not a valid identifier" );

		MethodBuilder* methodBuilder = new MethodBuilder;
		methodBuilder->init( this, name );

		return methodBuilder;
	}

protected:
	RefVector<IMethodInfo> _methods;
};

// ------ NativeClassTypeBuilder -----------------------------------------------

class NativeClassTypeBuilder : public MethodContainerTypeBuilder
{
public:
	NativeClassTypeBuilder() : MethodContainerTypeBuilder( TK_NATIVECLASS )
	{
		_myType = NULL;
	}

	bool allocateType()
	{
		assert( _myType == NULL );
		_myType = new NativeClassType;
		_type = _myType;
		return true;
	}

	void validate()
	{
		if( _attributes.empty() && _methods.empty() )
			CORAL_THROW( MissingInputException, "missing native class contents" );

		if( _nativeHeaderFile.empty() )
			CORAL_THROW( MissingInputException, "missing native class header name" );

		if( _nativeName.empty() )
			CORAL_THROW( MissingInputException, "missing native class name" );
	}

	void fillType()
	{
		_myType->setNativeHeaderFile( _nativeHeaderFile );
		_myType->setNativeName( _nativeName );

		_myType->addMembers( _attributes );
		_myType->addMembers( _methods );
		_myType->sortMembers( _myType );
	}

	void defineNativeClass( const std::string& nativeHeaderFile, const std::string& nativeName )
	{
		assertNotCreated();

		if( nativeHeaderFile.empty() )
			CORAL_THROW( IllegalArgumentException, "illegal empty header name" );

		if( nativeName.empty() )
			CORAL_THROW( IllegalArgumentException, "illegal empty native type name" );

		_nativeHeaderFile = nativeHeaderFile;
		_nativeName = nativeName;
	}

private:
	NativeClassType* _myType;
	std::string _nativeHeaderFile;
	std::string _nativeName;
};

// ------ InterfaceTypeBuilder -------------------------------------------------

class InterfaceTypeBuilder : public MethodContainerTypeBuilder
{
public:
	InterfaceTypeBuilder() : MethodContainerTypeBuilder( TK_INTERFACE )
	{
		_myType = NULL;
	}

	bool allocateType()
	{
		assert( _myType == NULL );

		// the 'co.Interface' IInterfaceType is pre-allocated by the ITypeManager
		if( _name == "Interface" && _namespace->getName() == "co" )
		{
			_myType = static_cast<InterfaceType*>( _namespace->getType( "Interface" ) );
			_type = _myType;
			return false;
		}
		else
		{
			_myType = new InterfaceType;
			_type = _myType;
			return true;
		}
	}

	void validate()
	{
		if( _attributes.empty() && _methods.empty() && _superTypes.empty() )
			CORAL_THROW( MissingInputException, "missing interface contents" );
	}

	void fillType()
	{
		// if this interface has no explicit super-types, add the implicit 'co.Interface' super-type to the list
		if( _superTypes.empty() )
		{
			InterfaceType* coInterfaceType =
				static_cast<InterfaceType*>( typeOf<Interface>::get() );
			// ... unless we're defining the co.Interface itself
			if( _myType != coInterfaceType )
				_superTypes.push_back( coInterfaceType );
		}

		// add all super-types in the list
		size_t count = _superTypes.size();
		for( size_t i = 0; i < count; ++i )
		{
			InterfaceType* superType = _superTypes[i].get();
			_myType->addSuperInterface( superType );
			superType->addSubInterface( _myType );
		}

		_myType->addMembers( _attributes );
		_myType->addMembers( _methods );
		_myType->sortMembers( _myType );
	}

	void defineSuperType( IType* superType )
	{
		assertNotCreated();

		if( !superType )
			CORAL_THROW( IllegalArgumentException, "illegal null supertype" );

		InterfaceType* interfaceType = dynamic_cast<InterfaceType*>( superType );
		if( !interfaceType )
			CORAL_THROW( IllegalArgumentException, "illegal supertype - an interface was expected" );

		// check if the super-type is already contained in the _superTypes list
		size_t count = _superTypes.size();
		for( size_t i = 0; i < count; ++i )
		{
			InterfaceType* anotherSuper = _superTypes[i].get();
			if( superType == anotherSuper )
				CORAL_THROW( NotSupportedException, "cannot inherit twice from '"
								<< superType->getFullName() << "'" );
		}

		_superTypes.push_back( interfaceType );
	}

private:
	InterfaceType* _myType;
	RefVector<InterfaceType> _superTypes;
};

// ------ ComponentTypeBuilder -------------------------------------------------

class ComponentTypeBuilder : public TypeBuilder
{
public:
	ComponentTypeBuilder() : TypeBuilder( TK_COMPONENT )
	{
		_myType = NULL;
	}

	bool allocateType()
	{
		assert( _myType == NULL );
		_myType = new ComponentType;
		_type = _myType;
		return true;
	}

	void validate()
	{
		if( _interfaces.empty() )
			CORAL_THROW( MissingInputException, "missing component interfaces" );
	}

	void fillType()
	{
		_myType->addInterfaces( _interfaces );
		_myType->sortInterfaces();
	}

	void defineInterface( const std::string& name, IInterfaceType* interface, bool isFacet )
	{
		assertNotCreated();

		if( !interface )
			CORAL_THROW( IllegalArgumentException, "illegal null interface" );

		if( !LexicalUtils::isValidIdentifier( name ) )
			CORAL_THROW( IllegalNameException, "interface name '" << name << "' is not a valid indentifier");

		for( ArrayRange<IInterfaceInfo* const> r( _interfaces ); r; r.popFirst() )
		{
			if( r.getFirst()->getName() == name )
				CORAL_THROW( IllegalNameException, "interface name '" << name << "' clashes with a previous definition" );
		}

		InterfaceInfo* interfaceInfo = new InterfaceInfo;
		interfaceInfo->setName( name );
		interfaceInfo->setType( interface );
		interfaceInfo->setIsFacet( isFacet );

		_interfaces.push_back( interfaceInfo );
	}

private:
	ComponentType* _myType;
	RefVector<IInterfaceInfo> _interfaces;
};

// ------ ITypeBuilder Factory Method -------------------------------------------

ITypeBuilder* TypeBuilder::create( TypeKind kind, INamespace* ns, const std::string& name )
{
	TypeBuilder* tb = NULL;
	switch( kind )
	{
	case TK_ENUM:			tb = new EnumTypeBuilder;			break;
	case TK_EXCEPTION:		tb = new ExceptionTypeBuilder;		break;
	case TK_STRUCT:			tb = new StructTypeBuilder;			break;
	case TK_NATIVECLASS:	tb = new NativeClassTypeBuilder;	break;
	case TK_INTERFACE:		tb = new InterfaceTypeBuilder;		break;
	case TK_COMPONENT:		tb = new ComponentTypeBuilder;		break;
	default:
		assert( false );
	}

	assert( tb );
	tb->initialize( ns, name );

	return tb;
}

// ------ NoTypeBuilder --------------------------------------------------------

/*
	This is a dummy TypeBuilder implementation that is instantiated
	when the Reflection API is used (instead of INamespace::defineType()).
 */
class NoTypeBuilder : public TypeBuilder
{
public:
	NoTypeBuilder() : TypeBuilder( TK_NONE )
	{;}

	bool allocateType()
	{
		raiseNoTypeBuilderException();
		return false;
	}

	void validate()
	{
		raiseNoTypeBuilderException();
	}

	void fillType()
	{
		raiseNoTypeBuilderException();
	}

private:
	void raiseNoTypeBuilderException()
	{
		throw NotSupportedException( "" );
	}
};

CORAL_EXPORT_COMPONENT( NoTypeBuilder, TypeBuilder );

} // namespace co
