/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#include <co/Type.h>
#include <co/DynamicProxyHandler.h>
#include <co/Namespace.h>
#include <co/Reflector.h>
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

class Type_Proxy : public co::Type
{
public:
	Type_Proxy( co::DynamicProxyHandler* handler ) : _handler( handler )
	{
		_cookie = _handler->registerProxyInterface( co::disambiguate<co::Interface, co::Type>( this ) );
	}

	virtual ~Type_Proxy()
	{
		// empty
	}

	// co::Interface Methods:

	co::InterfaceType* getInterfaceType() { return co::typeOf<co::Type>::get(); }
	co::Component* getInterfaceOwner() { return _handler->getInterfaceOwner(); }
	const std::string& getInterfaceName() { return _handler->getProxyInterfaceName( _cookie ); }
	void componentRetain() { _handler->componentRetain(); }
	void componentRelease() { _handler->componentRelease(); }

	// co.Type Methods:

	const co::Uuid& getBinarySignature()
	{
		const co::Any& res = _handler->handleGetAttribute( _cookie, getAttribInfo<co::Type>( 0 ) );
        return res.get< const co::Uuid& >();
	}

	const std::string& getFullName()
	{
		const co::Any& res = _handler->handleGetAttribute( _cookie, getAttribInfo<co::Type>( 1 ) );
        return res.get< const std::string& >();
	}

	const co::Uuid& getFullSignature()
	{
		const co::Any& res = _handler->handleGetAttribute( _cookie, getAttribInfo<co::Type>( 2 ) );
        return res.get< const co::Uuid& >();
	}

	co::TypeKind getKind()
	{
		const co::Any& res = _handler->handleGetAttribute( _cookie, getAttribInfo<co::Type>( 3 ) );
        return res.get< co::TypeKind >();
	}

	const std::string& getName()
	{
		const co::Any& res = _handler->handleGetAttribute( _cookie, getAttribInfo<co::Type>( 4 ) );
        return res.get< const std::string& >();
	}

	co::Namespace* getNamespace()
	{
		const co::Any& res = _handler->handleGetAttribute( _cookie, getAttribInfo<co::Type>( 5 ) );
        return res.get< co::Namespace* >();
	}

	co::Reflector* getReflector()
	{
		const co::Any& res = _handler->handleGetAttribute( _cookie, getAttribInfo<co::Type>( 6 ) );
        return res.get< co::Reflector* >();
	}

	void setReflector( co::Reflector* reflector_ )
	{
		co::Any arg;
		arg.set< co::Reflector* >( reflector_ );
		_handler->handleSetAttribute( _cookie, getAttribInfo<co::Type>( 6 ), arg );
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

class Type_Reflector : public co::ReflectorBase
{
public:
	Type_Reflector()
	{
		// empty
	}

	virtual ~Type_Reflector()
	{
		// empty
	}

	co::Type* getType()
	{
		return co::typeOf<co::Type>::get();
	}

	co::int32 getSize()
	{
		return sizeof(co::Type);
	}

	co::Interface* newProxy( co::DynamicProxyHandler* handler )
	{
		checValidProxyHandler( handler );
		return co::disambiguate<co::Interface, co::Type>( new co::Type_Proxy( handler ) );
	}

	void getAttribute( const co::Any& instance, co::AttributeInfo* ai, co::Any& value )
	{
		co::Type* p = checkInstance( instance, ai );
		switch( ai->getIndex() )
		{
		case 0:		value.set< const co::Uuid& >( p->getBinarySignature() ); break;
		case 1:		value.set< const std::string& >( p->getFullName() ); break;
		case 2:		value.set< const co::Uuid& >( p->getFullSignature() ); break;
		case 3:		value.set< co::TypeKind >( p->getKind() ); break;
		case 4:		value.set< const std::string& >( p->getName() ); break;
		case 5:		value.set< co::Namespace* >( p->getNamespace() ); break;
		case 6:		value.set< co::Reflector* >( p->getReflector() ); break;
		default:	raiseUnexpectedMemberIndex();
		}
	}

	void setAttribute( const co::Any& instance, co::AttributeInfo* ai, const co::Any& value )
	{
		co::Type* p = checkInstance( instance, ai );
		switch( ai->getIndex() )
		{
		case 0:		raiseAttributeIsReadOnly( ai ); break;
		case 1:		raiseAttributeIsReadOnly( ai ); break;
		case 2:		raiseAttributeIsReadOnly( ai ); break;
		case 3:		raiseAttributeIsReadOnly( ai ); break;
		case 4:		raiseAttributeIsReadOnly( ai ); break;
		case 5:		raiseAttributeIsReadOnly( ai ); break;
		case 6:		p->setReflector( value.get< co::Reflector* >() ); break;
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
	co::Type* checkInstance( const co::Any& any, co::MemberInfo* member )
	{
		if( !member )
			throw co::IllegalArgumentException( "illegal null member info" );

		// make sure that 'any' is an instance of this type
		co::InterfaceType* myType = co::typeOf<co::Type>::get();

		co::Type* res;
		if( any.getKind() != co::TK_INTERFACE || !( res = dynamic_cast<co::Type*>( any.getState().data.itf ) ) )
			CORAL_THROW( co::IllegalArgumentException, "expected a valid co::Type*, but got " << any );

		// make sure that 'member' belongs to this type
		co::CompoundType* owner = member->getOwner();
		if( owner != myType )
			CORAL_THROW( co::IllegalArgumentException, "member '" << member->getName() << "' belongs to "
				<< owner->getFullName() << ", not to co.Type" );

		return res;
	}
};

// ------ Reflector Creation Function ------ //

co::Reflector* __createTypeReflector()
{
    return new Type_Reflector;
}

} // namespace co
