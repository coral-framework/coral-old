/*
 * Coral - Lightweight C++ Component Framework.
 * See Copyright Notice in Coral.h
 */

#include <co/ParameterInfo.h>
#include <co/DynamicProxyHandler.h>
#include <co/Type.h>
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

class ParameterInfo_Proxy : public co::ParameterInfo
{
public:
	ParameterInfo_Proxy( co::DynamicProxyHandler* handler ) : _handler( handler )
	{
		_cookie = _handler->registerProxyInterface( co::disambiguate<co::Interface, co::ParameterInfo>( this ) );
	}

	virtual ~ParameterInfo_Proxy()
	{
		// empty
	}

	// co::Interface Methods:

	co::InterfaceType* getInterfaceType() { return co::typeOf<co::ParameterInfo>::get(); }
	co::Component* getInterfaceOwner() { return _handler->getInterfaceOwner(); }
	const std::string& getInterfaceName() { return _handler->getProxyInterfaceName( _cookie ); }
	void componentRetain() { _handler->componentRetain(); }
	void componentRelease() { _handler->componentRelease(); }

	// co.ParameterInfo Methods:

	bool getIsIn()
	{
		const co::Any& res = _handler->handleGetAttribute( _cookie, getAttribInfo<co::ParameterInfo>( 0 ) );
        return res.get< bool >();
	}

	bool getIsOut()
	{
		const co::Any& res = _handler->handleGetAttribute( _cookie, getAttribInfo<co::ParameterInfo>( 1 ) );
        return res.get< bool >();
	}

	const std::string& getName()
	{
		const co::Any& res = _handler->handleGetAttribute( _cookie, getAttribInfo<co::ParameterInfo>( 2 ) );
        return res.get< const std::string& >();
	}

	co::Type* getType()
	{
		const co::Any& res = _handler->handleGetAttribute( _cookie, getAttribInfo<co::ParameterInfo>( 3 ) );
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

class ParameterInfo_Reflector : public co::ReflectorBase
{
public:
	ParameterInfo_Reflector()
	{
		// empty
	}

	virtual ~ParameterInfo_Reflector()
	{
		// empty
	}

	co::Type* getType()
	{
		return co::typeOf<co::ParameterInfo>::get();
	}

	co::int32 getSize()
	{
		return sizeof(co::ParameterInfo);
	}

	co::Interface* newProxy( co::DynamicProxyHandler* handler )
	{
		checValidProxyHandler( handler );
		return co::disambiguate<co::Interface, co::ParameterInfo>( new co::ParameterInfo_Proxy( handler ) );
	}

	void getAttribute( const co::Any& instance, co::AttributeInfo* ai, co::Any& value )
	{
		co::ParameterInfo* p = checkInstance( instance, ai );
		switch( ai->getIndex() )
		{
		case 0:		value.set< bool >( p->getIsIn() ); break;
		case 1:		value.set< bool >( p->getIsOut() ); break;
		case 2:		value.set< const std::string& >( p->getName() ); break;
		case 3:		value.set< co::Type* >( p->getType() ); break;
		default:	raiseUnexpectedMemberIndex();
		}
	}

	void setAttribute( const co::Any& instance, co::AttributeInfo* ai, const co::Any& value )
	{
		co::ParameterInfo* p = checkInstance( instance, ai );
		switch( ai->getIndex() )
		{
		case 0:		raiseAttributeIsReadOnly( ai ); break;
		case 1:		raiseAttributeIsReadOnly( ai ); break;
		case 2:		raiseAttributeIsReadOnly( ai ); break;
		case 3:		raiseAttributeIsReadOnly( ai ); break;
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
	co::ParameterInfo* checkInstance( const co::Any& any, co::MemberInfo* member )
	{
		if( !member )
			throw co::IllegalArgumentException( "illegal null member info" );

		// make sure that 'any' is an instance of this type
		co::InterfaceType* myType = co::typeOf<co::ParameterInfo>::get();

		co::ParameterInfo* res;
		if( any.getKind() != co::TK_INTERFACE || !( res = dynamic_cast<co::ParameterInfo*>( any.getState().data.itf ) ) )
			CORAL_THROW( co::IllegalArgumentException, "expected a valid co::ParameterInfo*, but got " << any );

		// make sure that 'member' belongs to this type
		co::CompoundType* owner = member->getOwner();
		if( owner != myType )
			CORAL_THROW( co::IllegalArgumentException, "member '" << member->getName() << "' belongs to "
				<< owner->getFullName() << ", not to co.ParameterInfo" );

		return res;
	}
};

// ------ Reflector Creation Function ------ //

co::Reflector* __createParameterInfoReflector()
{
    return new ParameterInfo_Reflector;
}

} // namespace co
