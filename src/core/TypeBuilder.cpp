/*
 * Coral - A C++ Component Framework.
 * See Copyright Notice in Coral.h
 */

#include "TypeBuilder.h"

#include "Type.h"
#include "EnumType.h"
#include "Namespace.h"
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
#include <co/MethodInfo.h>
#include <co/IllegalNameException.h>
#include <co/MissingInputException.h>
#include <co/IllegalArgumentException.h>
#include <co/UnsupportedOperationException.h>
#include <co/reserved/LexicalUtilities.h>
#include <algorithm>
#include <sstream>

TypeBuilder::TypeBuilder( co::TypeKind kind ) : _kind( kind )
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
	Namespace* ns = static_cast<Namespace*>( _namespace.get() );

	// remove any array created for the type before we destroy it
	co::Type* arrayType = ns->getType( _type->getName() + "[]" );
	if( arrayType )
		ns->removeType( arrayType );

	ns->removeType( _type.get() );

	_kind = co::TK_NONE;
	_type = NULL;
}

void TypeBuilder::addMethod( co::MethodInfo* )
{
	CORAL_THROW( co::UnsupportedOperationException, "the typeBuilder's kind is neither TK_NATIVECLASS nor TK_INTERFACE" );
}

co::Namespace* TypeBuilder::getNamespace()
{
	return _namespace.get();
}

co::TypeKind TypeBuilder::getKind()
{
	return _kind;
}

const std::string& TypeBuilder::getTypeName()
{
	return _name;
}

void TypeBuilder::defineIdentifier(  const std::string& )
{
	CORAL_THROW( co::UnsupportedOperationException, "the typeBuilder's kind is not TK_ENUM" );
}

void TypeBuilder::defineAttribute( const std::string&, co::Type*, bool )
{
	CORAL_THROW( co::UnsupportedOperationException, "the typeBuilder's kind is neither TK_STRUCT, TK_NATIVECLASS nor TK_INTERFACE" );
}

void TypeBuilder::defineSuperType( co::Type* )
{
	CORAL_THROW( co::UnsupportedOperationException, "the typeBuilder's kind is not TK_INTERFACE" );
}

void TypeBuilder::defineInterface( const std::string&, co::InterfaceType*, bool )
{
	CORAL_THROW( co::UnsupportedOperationException, "the typeBuilder's kind is not TK_COMPONENT" );
}

co::MethodBuilder* TypeBuilder::defineMethod( const std::string& )
{
	CORAL_THROW( co::UnsupportedOperationException, "the typeBuilder's kind is neither TK_NATIVECLASS nor TK_INTERFACE" );
}

void TypeBuilder::defineNativeClass( const std::string&, const std::string& )
{
	CORAL_THROW( co::UnsupportedOperationException, "the typeBuilder's kind is not TK_NATIVECLASS" );
}

co::Type* TypeBuilder::createType()
{
	if( _typeWasCreated )
		return _type.get();

	validate();
	fillType();

	_typeWasCreated = true;

	return _type.get();
}

void TypeBuilder::initialize( co::Namespace* ns, const std::string& name )
{
	_namespace = ns;
	_name = name;
	
	// pre-allocate our empty type
	if( allocateType() )
	{
		TypeImpl* type = dynamic_cast<TypeImpl*>( _type.get() );
		assert( type );

		type->setType( _namespace.get(), _name, _kind );

		// add the type to its namespace (should be removed if it is rolled back)
		static_cast<Namespace*>( _namespace.get() )->addType( _type.get() );
	}

	assert( _type.isValid() );
}

void TypeBuilder::assertNotCreated()
{
	if( _typeWasCreated )
		CORAL_THROW( co::UnsupportedOperationException, "type was already created" );
}

// ------ EnumTypeBuilder -----------------------------------------------------

class EnumTypeBuilder : public TypeBuilder
{
public:
	EnumTypeBuilder() : TypeBuilder( co::TK_ENUM )
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
			CORAL_THROW( co::MissingInputException, "missing enum identifiers" );
	}

	void fillType()
	{
		for( co::ArrayRange<std::string> r( _identifiers ); r; r.popFirst() )
		{
			_myType->addIdentifier( r.getFirst() );
		}
	}

	void defineIdentifier( const std::string& name )
	{
		assertNotCreated();

		if( !co::LexicalUtilities::isValidIdentifier( name ) )
			CORAL_THROW( co::IllegalNameException, "invalid identifier '" << name << "'" );

		std::vector<std::string>::iterator it = std::find( _identifiers.begin(), _identifiers.end(), name );
		if( it != _identifiers.end() )
			CORAL_THROW( co::IllegalNameException, "invalid duplicate identifier '" << name << "'" );

		_identifiers.push_back( name );
	}

private:
	EnumType* _myType;
	std::vector<std::string> _identifiers;
};

// ------ ExceptionTypeBuilder ------------------------------------------------

class ExceptionTypeBuilder : public TypeBuilder
{
public:
	ExceptionTypeBuilder() : TypeBuilder( co::TK_EXCEPTION )
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

// ------ AttributeContainerTypeBuilder -------------------------------------------

class AttributeContainerTypeBuilder : public TypeBuilder
{
public:
	AttributeContainerTypeBuilder( co::TypeKind kind ) : TypeBuilder( kind )
	{;}

	void defineAttribute( const std::string& name, co::Type* type, bool isReadOnly )
	{
		assertNotCreated();

		if( !type )
			CORAL_THROW( co::IllegalArgumentException, "illegal null type" );

		if( type->getKind() == co::TK_EXCEPTION || type->getKind() == co::TK_COMPONENT )
			CORAL_THROW( co::IllegalArgumentException, ( type->getKind() == co::TK_EXCEPTION ?
					"exception" : "component" ) << "s are illegal as attribute types" );

		// struct-specific checks
		if( _kind == co::TK_STRUCT )
		{
			if( isReadOnly )
				CORAL_THROW( co::IllegalArgumentException, "structs cannot have read-only attributes" );

			if( _type == type )
				CORAL_THROW( co::IllegalArgumentException, "a struct cannot contain itself recursively" );
		}

		if( !co::LexicalUtilities::isValidIdentifier( name ) )
			CORAL_THROW( co::IllegalNameException, "attribute name '" << name << "' is not a valid identifier" );

		if( !co::LexicalUtilities::isValidAttributeName( name ) )
			CORAL_THROW( co::IllegalNameException, "attribute names must start with a lowercase letter" );

		for( co::RefVector<co::AttributeInfo>::iterator it = _attributes.begin(); it != _attributes.end(); ++it )
		{
			if( ( *it )->getName() == name )
				CORAL_THROW( co::IllegalNameException, "attribute name '" << name << "' clashes with a previous definition" );
		}

		AttributeInfo* attr = new AttributeInfo;
		attr->setType( type );
		attr->setName( name );
		attr->setIsReadOnly( isReadOnly );

		_attributes.push_back( attr );
	}

protected:
	co::RefVector<co::AttributeInfo> _attributes;
};

// ------ StructTypeBuilder ---------------------------------------------------

class StructTypeBuilder : public AttributeContainerTypeBuilder
{
public:
	StructTypeBuilder() : AttributeContainerTypeBuilder( co::TK_STRUCT )
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
			CORAL_THROW( co::MissingInputException, "missing struct contents" );
	}

	void fillType()
	{
		_myType->addMembers( _attributes );
		_myType->sortMembers( _myType );
	}

private:
	StructType* _myType;
};

// ------ MethodContainerTypeBuilder ----------------------------------------------------

class MethodContainerTypeBuilder : public AttributeContainerTypeBuilder
{
public:
	MethodContainerTypeBuilder( co::TypeKind kind ) : AttributeContainerTypeBuilder( kind )
	{;}

	void addMethod( co::MethodInfo* methodInfo )
	{
		if( _typeWasCreated )
			CORAL_THROW( co::UnsupportedOperationException, "illegal to add a method after a type is created" );
		_methods.push_back( methodInfo );
	}

	co::MethodBuilder* defineMethod( const std::string& name )
	{
		assertNotCreated();

		if( !co::LexicalUtilities::isValidIdentifier( name ) )
			CORAL_THROW( co::IllegalNameException, "method name '" << name << "' is not a valid identifier" );

		MethodBuilder* methodBuilder = new MethodBuilder;
		methodBuilder->init( this, name );

		return methodBuilder;
	}

protected:
	co::RefVector<co::MethodInfo> _methods;
};


// ------ NativeClassTypeBuilder ----------------------------------------------------

class NativeClassTypeBuilder : public MethodContainerTypeBuilder
{
public:
	NativeClassTypeBuilder() : MethodContainerTypeBuilder( co::TK_NATIVECLASS )
	{
		_myType = NULL;
	}

	bool allocateType()
	{
		assert( _myType == NULL );
		_myType = new NativeClassType;
		_type = _myType;
		return true;	}

	void validate()
	{
		if( _attributes.empty() && _methods.empty() )
			CORAL_THROW( co::MissingInputException, "missing native class contents" );

		if( _nativeHeaderFile.empty() )
			CORAL_THROW( co::MissingInputException, "missing native class header name" );

		if( _nativeName.empty() )
			CORAL_THROW( co::MissingInputException, "missing native class name" );
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
			CORAL_THROW( co::IllegalArgumentException, "illegal empty header name" );

		if( nativeName.empty() )
			CORAL_THROW( co::IllegalArgumentException, "illegal empty native type name" );

		_nativeHeaderFile = nativeHeaderFile;
		_nativeName = nativeName;
	}

private:
	NativeClassType* _myType;
	std::string _nativeHeaderFile;
	std::string _nativeName;
};

// ------ InterfaceTypeBuilder ----------------------------------------------------

class InterfaceTypeBuilder : public MethodContainerTypeBuilder
{
public:
	InterfaceTypeBuilder() : MethodContainerTypeBuilder( co::TK_INTERFACE )
	{
		_myType = NULL;
	}

	bool allocateType()
	{
		assert( _myType == NULL );

		// the 'co.Interface' InterfaceType is pre-allocated by the TypeManager
		if( _name == "Interface" && _namespace->getName() == "co" )
		{
			_myType = dynamic_cast<InterfaceType*>( _namespace->getType( "Interface" ) );
			assert( _myType );
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
			CORAL_THROW( co::MissingInputException, "missing interface contents" );
	}

	void fillType()
	{
		// if this interface has no explicit super-types, add the implicit 'co.Interface' super-type to the list
		if( _superTypes.empty() )
		{
			InterfaceType* coInterfaceType = dynamic_cast<InterfaceType*>( co::typeOf<co::Interface>::get() );
			assert( coInterfaceType );
			// ... unless we're defining the co.Interface itself
			if( _myType != coInterfaceType )
				_superTypes.push_back( coInterfaceType );
		}

		// add all super-types in the list
		for( co::ArrayRange<co::InterfaceType* const> r( _superTypes ); r; r.popFirst() )
		{
			InterfaceType* superType = dynamic_cast<InterfaceType*>( r.getFirst() );
			assert( superType );
			_myType->addSuperInterface( superType );
			superType->addSubInterface( _myType );
		}

		_myType->addMembers( _attributes );
		_myType->addMembers( _methods );
		_myType->sortMembers( _myType );
	}

	void defineSuperType( co::Type* superType )
	{
		assertNotCreated();

		if( !superType )
			CORAL_THROW( co::IllegalArgumentException, "illegal null supertype" );

		co::InterfaceType* interfaceType = dynamic_cast<co::InterfaceType*>( superType );
		if( !interfaceType )
			CORAL_THROW( co::IllegalArgumentException, "illegal supertype - an interface was expected" );

		// check if the super-type is already contained in the _superTypes list
		for( co::ArrayRange<co::InterfaceType* const> r( _superTypes ); r; r.popFirst() )
		{
			if( r.getFirst() == superType )
				CORAL_THROW( co::UnsupportedOperationException,
								"cannot inherit twice from '" << superType->getFullName() << "'" );
		}

		_superTypes.push_back( interfaceType );
	}

private:
	InterfaceType* _myType;
	co::RefVector<co::InterfaceType> _superTypes;
};

// ------ ComponentTypeBuilder ----------------------------------------------------

class ComponentTypeBuilder : public TypeBuilder
{
public:
	ComponentTypeBuilder() : TypeBuilder( co::TK_COMPONENT )
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
			CORAL_THROW( co::MissingInputException, "missing component interfaces" );
	}

	void fillType()
	{
		_myType->addInterfaces( _interfaces );
		_myType->sortInterfaces();
	}

	void defineInterface( const std::string& name, co::InterfaceType* interface, bool isProvided )
	{
		assertNotCreated();

		if( !interface )
			CORAL_THROW( co::IllegalArgumentException, "illegal null interface" );

		if( !co::LexicalUtilities::isValidIdentifier( name ) )
			CORAL_THROW( co::IllegalNameException, "interface name '" << name << "' is not a valid indentifier");

		for( co::ArrayRange<co::InterfaceInfo* const> r( _interfaces ); r; r.popFirst() )
		{
			if( r.getFirst()->getName() == name )
				CORAL_THROW( co::IllegalNameException, "interface name '" << name << "' clashes with a previous definition" );
		}

		InterfaceInfo* interfaceInfo = new InterfaceInfo;
		interfaceInfo->setName( name );
		interfaceInfo->setType( interface );
		interfaceInfo->setIsProvided( isProvided );

		_interfaces.push_back( interfaceInfo );
	}

private:
	ComponentType* _myType;
	co::RefVector<co::InterfaceInfo> _interfaces;
};

// ------ TypeBuilder Factory Method ------------------------------------------

TypeBuilder* TypeBuilder::create( co::TypeKind kind, co::Namespace* ns, const std::string& name )
{
	TypeBuilder* tb = NULL;
	switch( kind )
	{
	case co::TK_ENUM:			tb = new EnumTypeBuilder;			break;
	case co::TK_EXCEPTION:		tb = new ExceptionTypeBuilder;		break;
	case co::TK_STRUCT:			tb = new StructTypeBuilder;			break;
	case co::TK_NATIVECLASS:	tb = new NativeClassTypeBuilder;	break;
	case co::TK_INTERFACE:		tb = new InterfaceTypeBuilder;		break;
	case co::TK_COMPONENT:		tb = new ComponentTypeBuilder;		break;
	default:
		assert( false );
	}

	assert( tb );
	tb->initialize( ns, name );

	return tb;
}

// ------ NoTypeBuilder -------------------------------------------------------

/*
	This is a dummy TypeBuilderComponent implementation that is instantiated
	when the Reflection API is used (instead of co::Namespace::defineType()).
 */
class NoTypeBuilder : public TypeBuilder
{
public:
	NoTypeBuilder() : TypeBuilder( co::TK_NONE )
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
		throw co::UnsupportedOperationException( "" );
	}
};

CORAL_EXPORT_COMPONENT( NoTypeBuilder, TypeBuilderComponent );
