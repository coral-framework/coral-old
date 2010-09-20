/*
 * Coral - A C++ Component Framework.
 * See Copyright Notice in Coral.h
 */

#include <co/ArrayType.h>
#include <co/DynamicProxyHandler.h>
#include <co/Reflector.h>
#include <co/Namespace.h>
#include <co/Uuid.h>
#include <co/MethodInfo.h>
#include <co/AttributeInfo.h>
#include <co/IllegalCastException.h>
#include <co/MissingInputException.h>
#include <co/IllegalArgumentException.h>
#include <co/reserved/ReflectorBase.h>
#include <sstream>
#include <cassert>

namespace co {

// ------ Proxy Interface ------ //

class ArrayType_Proxy : public co::ArrayType
{
public:
	ArrayType_Proxy( co::DynamicProxyHandler* handler ) : _handler( handler )
	{
		_cookie = _handler->registerProxyInterface( co::disambiguate<co::Interface, co::ArrayType>( this ) );
	}

	virtual ~ArrayType_Proxy()
	{
		// empty
	}

	// co::Interface Methods:

	co::InterfaceType* getInterfaceType() { return co::typeOf<co::ArrayType>::get(); }
	co::Component* getInterfaceOwner() { return _handler->getInterfaceOwner(); }
	const std::string& getInterfaceName() { return _handler->getProxyInterfaceName( _cookie ); }
	void componentRetain() { _handler->componentRetain(); }
	void componentRelease() { _handler->componentRelease(); }

	// co.Type Methods:

	const co::Uuid& getBinarySignature()
	{
		co::Any res;
		_handler->handleGetAttribute( _cookie, getAttribInfo<co::Type>( 0 ), res );
		assert( res.containsObject() == false );
        return res.get< const co::Uuid& >();
	}

	const std::string& getFullName()
	{
		co::Any res;
		_handler->handleGetAttribute( _cookie, getAttribInfo<co::Type>( 1 ), res );
		assert( res.containsObject() == false );
        return res.get< const std::string& >();
	}

	const co::Uuid& getFullSignature()
	{
		co::Any res;
		_handler->handleGetAttribute( _cookie, getAttribInfo<co::Type>( 2 ), res );
		assert( res.containsObject() == false );
        return res.get< const co::Uuid& >();
	}

	co::TypeKind getKind()
	{
		co::Any res;
		_handler->handleGetAttribute( _cookie, getAttribInfo<co::Type>( 3 ), res );
		assert( res.containsObject() == false );
        return res.get< co::TypeKind >();
	}

	const std::string& getName()
	{
		co::Any res;
		_handler->handleGetAttribute( _cookie, getAttribInfo<co::Type>( 4 ), res );
		assert( res.containsObject() == false );
        return res.get< const std::string& >();
	}

	co::Namespace* getNamespace()
	{
		co::Any res;
		_handler->handleGetAttribute( _cookie, getAttribInfo<co::Type>( 5 ), res );
		assert( res.containsObject() == false );
        return res.get< co::Namespace* >();
	}

	co::Reflector* getReflector()
	{
		co::Any res;
		_handler->handleGetAttribute( _cookie, getAttribInfo<co::Type>( 6 ), res );
		assert( res.containsObject() == false );
        return res.get< co::Reflector* >();
	}

	void setReflector( co::Reflector* __reflector )
	{
		co::Any arg;
		arg.set< co::Reflector* >( __reflector );
		_handler->handleSetAttribute( _cookie, getAttribInfo<co::Type>( 6 ), arg );
	}

	// co.ArrayType Methods:

	co::Type* getElementType()
	{
		co::Any res;
		_handler->handleGetAttribute( _cookie, getAttribInfo<co::ArrayType>( 0 ), res );
		assert( res.containsObject() == false );
        return res.get< co::Type* >();
	}

protected:
	template<typename T>
	co::AttributeInfo* getAttribInfo( co::uint32 index )
	{
		return co::typeOf<T>::get()->getMemberAttributes()[index];
	}

	template<typename T>
	co::MethodInfo* getMethodInfo( co::uint32 index )
	{
		return co::typeOf<T>::get()->getMemberMethods()[index];
	}

private:
	co::DynamicProxyHandler* _handler;
	co::uint32 _cookie;
};

// ------ Reflector ------ //

class ArrayType_Reflector : public co::ReflectorBase
{
public:
	ArrayType_Reflector()
	{
		// empty
	}

	virtual ~ArrayType_Reflector()
	{
		// empty
	}

	co::Type* getType()
	{
		return co::typeOf<co::ArrayType>::get();
	}

	co::int32 getSize()
	{
		return sizeof(co::ArrayType);
	}

	co::Interface* newProxy( co::DynamicProxyHandler* handler )
	{
		checValidProxyHandler( handler );
		return co::disambiguate<co::Interface, co::ArrayType>( new co::ArrayType_Proxy( handler ) );
	}

	void getAttribute( const co::Any& instance, co::AttributeInfo* ai, co::Any& value )
	{
		co::ArrayType* p = checkInstance( instance, ai );
		switch( ai->getIndex() )
		{
		case 0:		value.set< co::Type* >( p->getElementType() ); break;
		default:	raiseUnexpectedMemberIndex();
		}
	}

	void setAttribute( const co::Any& instance, co::AttributeInfo* ai, const co::Any& value )
	{
		co::ArrayType* p = checkInstance( instance, ai );
		switch( ai->getIndex() )
		{
		case 0:		raiseAttributeIsReadOnly( ai ); break;
		default:	raiseUnexpectedMemberIndex();
		}
		CORAL_UNUSED( p );
		CORAL_UNUSED( value );
	}

	void invokeMethod( const co::Any& instance, co::MethodInfo* mi, co::ArrayRange<co::Any const> args, co::Any& res )
	{
		checkInstance( instance, mi );
		raiseUnexpectedMemberIndex();
		CORAL_UNUSED( args );
		CORAL_UNUSED( res );
	}

private:
	co::ArrayType* checkInstance( const co::Any& instance, co::MemberInfo* member )
	{
		if( !member )
			throw co::IllegalArgumentException( "illegal null member info" );

		co::InterfaceType* myType = co::typeOf<co::ArrayType>::get();

		// make sure that 'instance' is an instance of this type
		if( instance.getKind() != co::TK_INTERFACE ||
			!instance.getInterfaceType()->isSubTypeOf( myType ) ||
			instance.getState().data.ptr == NULL )
			CORAL_THROW( co::IllegalArgumentException, "expected a valid co::ArrayType*, but got " << instance );

		// make sure that 'member' belongs to this type
		co::CompoundType* owner = member->getOwner();
		if( owner != myType )
			CORAL_THROW( co::IllegalArgumentException, "member '" << member->getName() << "' belongs to "
				<< owner->getFullName() << ", not to " << myType->getFullName() );

		return dynamic_cast<co::ArrayType*>( instance.getState().data.itf );
	}
};

// ------ Reflector Creation Function ------ //

co::Reflector* __createArrayTypeReflector()
{
    return new ArrayType_Reflector;
}

} // namespace co
