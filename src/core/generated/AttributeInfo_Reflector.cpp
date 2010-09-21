/*
 * Coral - A C++ Component Framework.
 * See Copyright Notice in Coral.h
 */

#include <co/AttributeInfo.h>
#include <co/DynamicProxyHandler.h>
#include <co/Type.h>
#include <co/CompoundType.h>
#include <co/MethodInfo.h>
#include <co/IllegalCastException.h>
#include <co/MissingInputException.h>
#include <co/IllegalArgumentException.h>
#include <co/reserved/ReflectorBase.h>
#include <sstream>
#include <cassert>

namespace co {

// ------ Proxy Interface ------ //

class AttributeInfo_Proxy : public co::AttributeInfo
{
public:
	AttributeInfo_Proxy( co::DynamicProxyHandler* handler ) : _handler( handler )
	{
		_cookie = _handler->registerProxyInterface( co::disambiguate<co::Interface, co::AttributeInfo>( this ) );
	}

	virtual ~AttributeInfo_Proxy()
	{
		// empty
	}

	// co::Interface Methods:

	co::InterfaceType* getInterfaceType() { return co::typeOf<co::AttributeInfo>::get(); }
	co::Component* getInterfaceOwner() { return _handler->getInterfaceOwner(); }
	const std::string& getInterfaceName() { return _handler->getProxyInterfaceName( _cookie ); }
	void componentRetain() { _handler->componentRetain(); }
	void componentRelease() { _handler->componentRelease(); }

	// co.MemberInfo Methods:

	co::uint32 getIndex()
	{
		co::Any res;
		_handler->handleGetAttribute( _cookie, getAttribInfo<co::MemberInfo>( 0 ), res );
		assert( res.containsObject() == false );
        return res.get< co::uint32 >();
	}

	const std::string& getName()
	{
		co::Any res;
		_handler->handleGetAttribute( _cookie, getAttribInfo<co::MemberInfo>( 1 ), res );
		assert( res.containsObject() == false );
        return res.get< const std::string& >();
	}

	co::CompoundType* getOwner()
	{
		co::Any res;
		_handler->handleGetAttribute( _cookie, getAttribInfo<co::MemberInfo>( 2 ), res );
		assert( res.containsObject() == false );
        return res.get< co::CompoundType* >();
	}

	// co.AttributeInfo Methods:

	bool getIsReadOnly()
	{
		co::Any res;
		_handler->handleGetAttribute( _cookie, getAttribInfo<co::AttributeInfo>( 0 ), res );
		assert( res.containsObject() == false );
        return res.get< bool >();
	}

	co::Type* getType()
	{
		co::Any res;
		_handler->handleGetAttribute( _cookie, getAttribInfo<co::AttributeInfo>( 1 ), res );
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

class AttributeInfo_Reflector : public co::ReflectorBase
{
public:
	AttributeInfo_Reflector()
	{
		// empty
	}

	virtual ~AttributeInfo_Reflector()
	{
		// empty
	}

	co::Type* getType()
	{
		return co::typeOf<co::AttributeInfo>::get();
	}

	co::int32 getSize()
	{
		return sizeof(co::AttributeInfo);
	}

	co::Interface* newProxy( co::DynamicProxyHandler* handler )
	{
		checValidProxyHandler( handler );
		return co::disambiguate<co::Interface, co::AttributeInfo>( new co::AttributeInfo_Proxy( handler ) );
	}

	void getAttribute( const co::Any& instance, co::AttributeInfo* ai, co::Any& value )
	{
		co::AttributeInfo* p = checkInstance( instance, ai );
		switch( ai->getIndex() )
		{
		case 0:		value.set< bool >( p->getIsReadOnly() ); break;
		case 1:		value.set< co::Type* >( p->getType() ); break;
		default:	raiseUnexpectedMemberIndex();
		}
	}

	void setAttribute( const co::Any& instance, co::AttributeInfo* ai, const co::Any& value )
	{
		co::AttributeInfo* p = checkInstance( instance, ai );
		switch( ai->getIndex() )
		{
		case 0:		raiseAttributeIsReadOnly( ai ); break;
		case 1:		raiseAttributeIsReadOnly( ai ); break;
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
	co::AttributeInfo* checkInstance( const co::Any& any, co::MemberInfo* member )
	{
		if( !member )
			throw co::IllegalArgumentException( "illegal null member info" );

		// make sure that 'any' is an instance of this type
		co::InterfaceType* myType = co::typeOf<co::AttributeInfo>::get();

		co::AttributeInfo* res;
		if( any.getKind() != co::TK_INTERFACE || !( res = dynamic_cast<co::AttributeInfo*>( any.getState().data.itf ) ) )
			CORAL_THROW( co::IllegalArgumentException, "expected a valid co::AttributeInfo*, but got " << any );

		// make sure that 'member' belongs to this type
		co::CompoundType* owner = member->getOwner();
		if( owner != myType )
			CORAL_THROW( co::IllegalArgumentException, "member '" << member->getName() << "' belongs to "
				<< owner->getFullName() << ", not to co.AttributeInfo" );

		return res;
	}
};

// ------ Reflector Creation Function ------ //

co::Reflector* __createAttributeInfoReflector()
{
    return new AttributeInfo_Reflector;
}

} // namespace co
