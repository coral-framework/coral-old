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
#include <co/IllegalStateException.h>
#include <co/MissingInputException.h>
#include <co/NotSupportedException.h>
#include <co/IllegalArgumentException.h>
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

void TypeBuilder::addMethod( Method* )
{
	CORAL_THROW( NotSupportedException, "the builder's type is not a class type" );
}

void TypeBuilder::validate()
{
	// does nothing by default
}

void TypeBuilder::commit()
{
	// does nothing by default
}

void TypeBuilder::rollback()
{
	// remove any array created for the type before we destroy it
	IType* arrayType = _namespace->getType( _type->getName() + "[]" );
	if( arrayType )
		_namespace->removeType( arrayType );

	_namespace->removeType( _type.get() );

	_kind = TK_NONE;
	_type = NULL;
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

void TypeBuilder::defineBaseType( IType* )
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

// ------ EnumBuilder ------------------------------------------------------

class EnumBuilder : public TypeBuilder
{
public:
	EnumBuilder() : TypeBuilder( TK_ENUM )
	{
		_myType = NULL;
	}

	bool allocateType()
	{
		assert( _myType == NULL );
		_myType = new Enum;
		_type = _myType;
		return true;
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

	void fillType()
	{
		if( _identifiers.empty() )
			CORAL_THROW( MissingInputException, "missing enum identifiers" );

		for( Range<std::string> r( _identifiers ); r; r.popFirst() )
		{
			_myType->addIdentifier( r.getFirst() );
		}
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

	void fillType()
	{
		// empty
	}

private:
	ExceptionType* _myType;
};

// ------ RecordTypeBuilder ----------------------------------------------------

class CompositeTypeBuilder : public TypeBuilder
{
public:
	CompositeTypeBuilder( TypeKind kind ) : TypeBuilder( kind )
	{;}

	inline IMember* getMember( const std::string& name )
	{
		return static_cast<ICompositeType*>( _type.get() )->getMember( name );
	}

	inline const char* describe( IMember* m )
	{
		return m->getKind() == MK_METHOD ? "method" : "field";
	}

	// Reusable method to handle member clashes.
	void handleMemberClash( const char* what, IMember* m, IMember* found )
	{
		if( !found || m == found ) return;

		std::stringstream ss;
		ss << "name clash in type '" << _type->getFullName() << "' between ";
		ss << what << " '" << m->getName() << "' ";

		ICompositeType* mOwner = m->getOwner();
		ICompositeType* foundOwner = found->getOwner();

		if( mOwner == foundOwner )
		{
			ss << "and " << describe( found ) << " '" << found->getName() << "'";
		}
		else
		{
			ss << "(from '" << mOwner->getFullName() << "') and " << describe( found );
			ss << " '" << found->getName() << "' (from '" << foundOwner->getFullName() << "')";
		}

		throw IllegalNameException( ss.str() );
	}

	// Template method that looks for possible collisions with 'm'.
	virtual void checkForMemberClashes( IMember* m ) = 0;

	void validate()
	{
		Range<IMember* const> members = static_cast<ICompositeType*>( _type.get() )->getMembers();
		for( ; members; members.popFirst() )
			checkForMemberClashes( members.getFirst() );
	}
};

// ------ RecordTypeBuilder ----------------------------------------------------

class RecordTypeBuilder : public CompositeTypeBuilder
{
public:
	RecordTypeBuilder( TypeKind kind ) : CompositeTypeBuilder( kind )
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
				CORAL_THROW( IllegalArgumentException, "structs cannot have read-only fields" );

			if( _type == type )
				CORAL_THROW( IllegalArgumentException, "a struct cannot contain itself recursively" );
		}

		if( !LexicalUtils::isValidIdentifier( name ) )
			CORAL_THROW( IllegalNameException, "field name '" << name << "' is not a valid identifier" );

		if( !LexicalUtils::isValidFieldName( name ) )
			CORAL_THROW( IllegalNameException, "field names must start with a lowercase letter" );

		Field* attr = new Field;
		attr->setType( type );
		attr->setName( name );
		attr->setIsReadOnly( isReadOnly );

		_fields.push_back( attr );
	}

	void checkForMemberClashes( IMember* m )
	{
		handleMemberClash( "field", m, getMember( m->getName() ) );
	}

protected:
	RefVector<IField> _fields;
};

// ------ StructBuilder ----------------------------------------------------

class StructBuilder : public RecordTypeBuilder
{
public:
	StructBuilder() : RecordTypeBuilder( TK_STRUCT )
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

	void fillType()
	{
		if( _fields.empty() )
			CORAL_THROW( MissingInputException, "missing struct contents" );

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


	void checkForMemberClashes( IMember* m )
	{
		const std::string& name = m->getName();
		if( m->getKind() == MK_METHOD )
		{
			handleMemberClash( "method", m, getMember( name ) );
		}
		else
		{
			assert( m->getKind() == MK_FIELD );

			handleMemberClash( "field", m, getMember( name ) );

			std::string accessor;
			LexicalUtils::formatAccessor( name, LexicalUtils::Getter, accessor );
			handleMemberClash( "the getter of field", m, getMember( accessor ) );

			if( static_cast<IField*>( m )->getIsReadOnly() == false )
			{
				LexicalUtils::formatAccessor( name, LexicalUtils::Setter, accessor );
				handleMemberClash( "the setter of field", m, getMember( accessor ) );
			}
		}
	}

protected:
	RefVector<IMethod> _methods;
};

// ------ NativeClassBuilder -----------------------------------------------

class NativeClassBuilder : public ClassTypeBuilder
{
public:
	NativeClassBuilder() : ClassTypeBuilder( TK_NATIVECLASS )
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

	void fillType()
	{
		if( _nativeHeader.empty() )
			CORAL_THROW( MissingInputException, "missing native header" );

		if( _nativeName.empty() )
			CORAL_THROW( MissingInputException, "missing native name" );

		_myType->setNativeHeader( _nativeHeader );
		_myType->setNativeName( _nativeName );

		_myType->addMembers( _fields );
		_myType->addMembers( _methods );
		_myType->sortMembers( _myType );
	}

private:
	NativeClass* _myType;
	std::string _nativeHeader;
	std::string _nativeName;
};

// ------ InterfaceBuilder -------------------------------------------------

class InterfaceBuilder : public ClassTypeBuilder
{
public:
	InterfaceBuilder() : ClassTypeBuilder( TK_INTERFACE )
	{
		_myType = NULL;
		_baseType = NULL;
	}

	bool allocateType()
	{
		assert( _myType == NULL );

		// the 'co.IService' IInterface is pre-allocated by the ITypeManager
		if( _name == "IService" && _namespace->getFullName() == "co" )
		{
			_myType = static_cast<Interface*>( _namespace->getType( "IService" ) );
			_type = _myType;
			return false;
		}

		_myType = new Interface;
		_type = _myType;

		// by default, all interfaces inherit from co.IService
		_baseType = static_cast<Interface*>( co::typeOf<co::IService>::get() );
		_myType->setBaseType( _baseType );

		return true;
	}

	void defineBaseType( IType* baseType )
	{
		assertNotCreated();

		if( _baseType && _baseType->getFullName() != "co.IService" )
			CORAL_THROW( NotSupportedException, "multiple inheritance not supported" );

		if( !baseType )
			CORAL_THROW( IllegalArgumentException, "illegal null baseType" );

		co::TypeKind kind = baseType->getKind();
		if( kind != co::TK_INTERFACE )
			CORAL_THROW( IllegalArgumentException, "illegal baseType (interface expected, got "
							<< co::TK_STRINGS[kind] << ")" );

		_baseType = static_cast<Interface*>( baseType );
		_myType->setBaseType( _baseType );
	}

	void fillType()
	{
		_myType->addMembers( _fields );
		_myType->addMembers( _methods );
		_myType->sortMembers( _myType );
	}

	void validate()
	{
		// check for cyclic inheritance
		IInterface* base = _baseType;
		while( base )
		{
			if( base == _myType )
				CORAL_THROW( IllegalStateException, "cyclic inheritance detected'" );
			base = base->getBaseType();
		}

		_myType->updateSuperTypes();

		// check for member clashes, considering all super types
		IInterface* type = _myType;
		Range<IInterface* const> superTypes = type->getSuperTypes();
		while( 1 )
		{
			Range<IMember* const> members = type->getMembers();
			for( ; members; members.popFirst() )
				checkForMemberClashes( members.getFirst() );

			if( !superTypes )
				break;

			type = superTypes.getFirst();
			superTypes.popFirst();
		}
	}

	void commit()
	{
		if( _baseType )
			_baseType->addSubType( _myType );
	}

private:
	Interface* _myType;
	Interface* _baseType;
};

// ------ ComponentBuilder -------------------------------------------------

class ComponentBuilder : public TypeBuilder
{
public:
	ComponentBuilder() : TypeBuilder( TK_COMPONENT )
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

	void definePort( const std::string& name, IInterface* type, bool isFacet )
	{
		assertNotCreated();

		if( !type )
			CORAL_THROW( IllegalArgumentException, "illegal null type" );

		if( !LexicalUtils::isValidIdentifier( name ) )
			CORAL_THROW( IllegalNameException, "port name '" << name << "' is not a valid indentifier");

		Port* port = new Port;
		port->setName( name );
		port->setType( type );
		port->setIsFacet( isFacet );

		_interfaces.push_back( port );
	}

	void fillType()
	{
		if( _interfaces.empty() )
			CORAL_THROW( MissingInputException, "missing component ports" );

		_myType->addPorts( _interfaces );
		_myType->sortPorts();
	}

	void validate()
	{
		Range<IPort* const> ports = _myType->getPorts();
		IPort* lastPort = ports.getFirst();
		for( ports.popFirst(); ports; ports.popFirst() )
		{
			if( lastPort->getName() == ports.getFirst()->getName() )
				CORAL_THROW( IllegalNameException, "duplicate port '" << lastPort->getName()
					<< "' in component '" << _myType->getFullName() << "'" );
		}
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
	case TK_ENUM:			tb = new EnumBuilder;			break;
	case TK_EXCEPTION:		tb = new ExceptionTypeBuilder;	break;
	case TK_STRUCT:			tb = new StructBuilder;			break;
	case TK_NATIVECLASS:	tb = new NativeClassBuilder;	break;
	case TK_INTERFACE:		tb = new InterfaceBuilder;		break;
	case TK_COMPONENT:		tb = new ComponentBuilder;		break;
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

	void fillType()
	{
		raiseNoTypeBuilderException();
	}

private:
	void raiseNoTypeBuilderException()
	{
		throw NotSupportedException();
	}
};

CORAL_EXPORT_COMPONENT( NoTypeBuilder, TypeBuilder );

} // namespace co
