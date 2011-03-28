/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#include "TypeBuilder.h"

#include "Type.h"
#include "Enum.h"
#include "Namespace.h"
#include "Method.h"
#include "Struct.h"
#include "TypeManager.h"
#include "Field.h"
#include "MethodBuilder.h"
#include "Port.h"
#include "ExceptionType.h"
#include "Component.h"
#include "Interface.h"
#include "NativeClass.h"
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

void TypeBuilder::addMethod( Method* )
{
	CORAL_THROW( NotSupportedException, "the builder's type is not a class type" );
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
	CORAL_THROW( NotSupportedException, "the builder's type is not an enum" );
}

void TypeBuilder::defineField( const std::string&, IType*, bool )
{
	CORAL_THROW( NotSupportedException, "the builder's type is not a record type" );
}

void TypeBuilder::defineSuperType( IType* )
{
	CORAL_THROW( NotSupportedException, "the builder's type does not support inheritance" );
}

void TypeBuilder::definePort( const std::string&, IInterface*, bool )
{
	CORAL_THROW( NotSupportedException, "the builder's type is not a component" );
}

IMethodBuilder* TypeBuilder::defineMethod( const std::string& )
{
	CORAL_THROW( NotSupportedException, "the builder's type is not a class type" );
}

void TypeBuilder::defineNativeClass( const std::string&, const std::string& )
{
	CORAL_THROW( NotSupportedException, "the builder's type is not a native class" );
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
		CORAL_THROW( NotSupportedException, "the builder's type was already created" );
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
		_myType = new Enum;
		_type = _myType;
		return true;	}

	void validate()
	{
		if( _identifiers.empty() )
			CORAL_THROW( MissingInputException, "missing enum identifiers" );
	}

	void fillType()
	{
		for( Range<std::string> r( _identifiers ); r; r.popFirst() )
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
	Enum* _myType;
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

// ------ RecordTypeBuilder ----------------------------------------------------

class RecordTypeBuilder : public TypeBuilder
{
public:
	RecordTypeBuilder( TypeKind kind ) : TypeBuilder( kind )
	{;}

	void defineField( const std::string& name, IType* type, bool isReadOnly )
	{
		assertNotCreated();

		if( !type )
			CORAL_THROW( IllegalArgumentException, "illegal null type" );

		if( type->getKind() == TK_EXCEPTION || type->getKind() == TK_COMPONENT )
			CORAL_THROW( IllegalArgumentException, ( type->getKind() == TK_EXCEPTION ?
					"exception" : "component" ) << "s are illegal as field types" );

		// struct-specific checks
		if( _kind == TK_STRUCT )
		{
			if( isReadOnly )
				CORAL_THROW( IllegalArgumentException, "structs cannot have read-only field" );

			if( _type == type )
				CORAL_THROW( IllegalArgumentException, "a struct cannot contain itself recursively" );
		}

		if( !LexicalUtils::isValidIdentifier( name ) )
			CORAL_THROW( IllegalNameException, "field name '" << name << "' is not a valid identifier" );

		if( !LexicalUtils::isValidFieldName( name ) )
			CORAL_THROW( IllegalNameException, "field names must start with a lowercase letter" );

		size_t count = _fields.size();
		for( size_t i = 0; i < count; ++i )
		{
			if( _fields[i]->getName() == name )
				CORAL_THROW( IllegalNameException, "field name '" << name << "' clashes with a previous definition" );
		}

		Field* attr = new Field;
		attr->setType( type );
		attr->setName( name );
		attr->setIsReadOnly( isReadOnly );

		_fields.push_back( attr );
	}

protected:
	RefVector<IField> _fields;
};

// ------ StructTypeBuilder ----------------------------------------------------

class StructTypeBuilder : public RecordTypeBuilder
{
public:
	StructTypeBuilder() : RecordTypeBuilder( TK_STRUCT )
	{
		_myType = NULL;
	}

	bool allocateType()
	{
		assert( _myType == NULL );
		_myType = new Struct;
		_type = _myType;
		return true;
	}

	void validate()
	{
		if( _fields.empty() )
			CORAL_THROW( MissingInputException, "missing struct contents" );
	}

	void fillType()
	{
		_myType->addMembers( _fields );
		_myType->sortMembers( _myType );
	}

private:
	Struct* _myType;
};

// ------ ClassTypeBuilder -----------------------------------------------------

class ClassTypeBuilder : public RecordTypeBuilder
{
public:
	ClassTypeBuilder( TypeKind kind ) : RecordTypeBuilder( kind )
	{;}

	void addMethod( Method* methodInfo )
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
	RefVector<IMethod> _methods;
};

// ------ NativeClassTypeBuilder -----------------------------------------------

class NativeClassTypeBuilder : public ClassTypeBuilder
{
public:
	NativeClassTypeBuilder() : ClassTypeBuilder( TK_NATIVECLASS )
	{
		_myType = NULL;
	}

	bool allocateType()
	{
		assert( _myType == NULL );
		_myType = new NativeClass;
		_type = _myType;
		return true;
	}

	void validate()
	{
		if( _nativeHeader.empty() )
			CORAL_THROW( MissingInputException, "missing native header" );

		if( _nativeName.empty() )
			CORAL_THROW( MissingInputException, "missing native name" );
	}

	void fillType()
	{
		_myType->setNativeHeader( _nativeHeader );
		_myType->setNativeName( _nativeName );

		_myType->addMembers( _fields );
		_myType->addMembers( _methods );
		_myType->sortMembers( _myType );
	}

	void defineNativeClass( const std::string& nativeHeader, const std::string& nativeName )
	{
		assertNotCreated();

		if( nativeHeader.empty() )
			CORAL_THROW( IllegalArgumentException, "illegal empty header name" );

		if( nativeName.empty() )
			CORAL_THROW( IllegalArgumentException, "illegal empty native type name" );

		_nativeHeader = nativeHeader;
		_nativeName = nativeName;
	}

private:
	NativeClass* _myType;
	std::string _nativeHeader;
	std::string _nativeName;
};

// ------ InterfaceTypeBuilder -------------------------------------------------

class InterfaceTypeBuilder : public ClassTypeBuilder
{
public:
	InterfaceTypeBuilder() : ClassTypeBuilder( TK_INTERFACE )
	{
		_myType = NULL;
	}

	bool allocateType()
	{
		assert( _myType == NULL );

		// the 'co.IService' IInterface is pre-allocated by the ITypeManager
		if( _name == "IService" && _namespace->getName() == "co" )
		{
			_myType = static_cast<Interface*>( _namespace->getType( "IService" ) );
			_type = _myType;
			return false;
		}
		else
		{
			_myType = new Interface;
			_type = _myType;
			return true;
		}
	}

	void validate()
	{
		if( _fields.empty() && _methods.empty() && _superTypes.empty() )
			CORAL_THROW( MissingInputException, "missing interface contents" );
	}

	void fillType()
	{
		// when an interface has no explicit supertype, add the implicit 'co.IService' supertype
		if( _superTypes.empty() )
		{
			Interface* coInterfaceType =
				static_cast<Interface*>( typeOf<IService>::get() );
			// ... unless we're defining the co.IService itself
			if( _myType != coInterfaceType )
				_superTypes.push_back( coInterfaceType );
		}

		// add all super-types in the list
		size_t count = _superTypes.size();
		for( size_t i = 0; i < count; ++i )
		{
			Interface* superType = _superTypes[i].get();
			_myType->addSuperInterface( superType );
			superType->addSubInterface( _myType );
		}

		_myType->addMembers( _fields );
		_myType->addMembers( _methods );
		_myType->sortMembers( _myType );
	}

	void defineSuperType( IType* superType )
	{
		assertNotCreated();

		if( !superType )
			CORAL_THROW( IllegalArgumentException, "illegal null supertype" );

		Interface* interfaceType = dynamic_cast<Interface*>( superType );
		if( !interfaceType )
			CORAL_THROW( IllegalArgumentException, "illegal supertype - an interface was expected" );

		// check if the super-type is already contained in the _superTypes list
		size_t count = _superTypes.size();
		for( size_t i = 0; i < count; ++i )
		{
			Interface* anotherSuper = _superTypes[i].get();
			if( superType == anotherSuper )
				CORAL_THROW( NotSupportedException, "cannot inherit twice from '"
								<< superType->getFullName() << "'" );
		}

		_superTypes.push_back( interfaceType );
	}

private:
	Interface* _myType;
	RefVector<Interface> _superTypes;
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
		_myType = new Component;
		_type = _myType;
		return true;
	}

	void validate()
	{
		if( _interfaces.empty() )
			CORAL_THROW( MissingInputException, "missing component ports" );
	}

	void fillType()
	{
		_myType->addInterfaces( _interfaces );
		_myType->sortInterfaces();
	}

	void definePort( const std::string& name, IInterface* interface, bool isFacet )
	{
		assertNotCreated();

		if( !interface )
			CORAL_THROW( IllegalArgumentException, "illegal null interface" );

		if( !LexicalUtils::isValidIdentifier( name ) )
			CORAL_THROW( IllegalNameException, "port name '" << name << "' is not a valid indentifier");

		for( Range<IPort* const> r( _interfaces ); r; r.popFirst() )
		{
			if( r.getFirst()->getName() == name )
				CORAL_THROW( IllegalNameException, "port name '" << name << "' clashes with a previous definition" );
		}

		Port* port = new Port;
		port->setName( name );
		port->setType( interface );
		port->setIsFacet( isFacet );

		_interfaces.push_back( port );
	}

private:
	Component* _myType;
	RefVector<IPort> _interfaces;
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
