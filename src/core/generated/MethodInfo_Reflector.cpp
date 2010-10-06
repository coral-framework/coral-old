/*
 * Coral - A C++ Component Framework.
 * See Copyright Notice in Coral.h
 */

#include <co/MethodInfo.h>
#include <co/DynamicProxyHandler.h>
#include <co/CompoundType.h>
#include <co/ExceptionType.h>
#include <co/Type.h>
#include <co/ParameterInfo.h>
#include <co/AttributeInfo.h>
#include <co/IllegalCastException.h>
#include <co/MissingInputException.h>
#include <co/IllegalArgumentException.h>
#include <co/reserved/ReflectorBase.h>
#include <sstream>
#include <cassert>

namespace co {

// ------ Proxy Interface ------ //

class MethodInfo_Proxy : public co::MethodInfo
{
public:
	MethodInfo_Proxy( co::DynamicProxyHandler* handler ) : _handler( handler )
	{
		_cookie = _handler->registerProxyInterface( co::disambiguate<co::Interface, co::MethodInfo>( this ) );
	}

	virtual ~MethodInfo_Proxy()
	{
		// empty
	}

	// co::Interface Methods:

	co::InterfaceType* getInterfaceType() { return co::typeOf<co::MethodInfo>::get(); }
	co::Component* getInterfaceOwner() { return _handler->getInterfaceOwner(); }
	const std::string& getInterfaceName() { return _handler->getProxyInterfaceName( _cookie ); }
	void componentRetain() { _handler->componentRetain(); }
	void componentRelease() { _handler->componentRelease(); }

	// co.MemberInfo Methods:

	co::uint32 getIndex()
	{
		const co::Any& res = _handler->handleGetAttribute( _cookie, getAttribInfo<co::MemberInfo>( 0 ) );
        return res.get< co::uint32 >();
	}

	const std::string& getName()
	{
		const co::Any& res = _handler->handleGetAttribute( _cookie, getAttribInfo<co::MemberInfo>( 1 ) );
        return res.get< const std::string& >();
	}

	co::CompoundType* getOwner()
	{
		const co::Any& res = _handler->handleGetAttribute( _cookie, getAttribInfo<co::MemberInfo>( 2 ) );
        return res.get< co::CompoundType* >();
	}

	// co.MethodInfo Methods:

	co::ArrayRange<co::ExceptionType* const> getExceptions()
	{
		const co::Any& res = _handler->handleGetAttribute( _cookie, getAttribInfo<co::MethodInfo>( 0 ) );
        return res.get< co::ArrayRange<co::ExceptionType* const> >();
	}

	co::ArrayRange<co::ParameterInfo* const> getParameters()
	{
		const co::Any& res = _handler->handleGetAttribute( _cookie, getAttribInfo<co::MethodInfo>( 1 ) );
        return res.get< co::ArrayRange<co::ParameterInfo* const> >();
	}

	co::Type* getReturnType()
	{
		const co::Any& res = _handler->handleGetAttribute( _cookie, getAttribInfo<co::MethodInfo>( 2 ) );
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

class MethodInfo_Reflector : public co::ReflectorBase
{
public:
	MethodInfo_Reflector()
	{
		// empty
	}

	virtual ~MethodInfo_Reflector()
	{
		// empty
	}

	co::Type* getType()
	{
		return co::typeOf<co::MethodInfo>::get();
	}

	co::int32 getSize()
	{
		return sizeof(co::MethodInfo);
	}

	co::Interface* newProxy( co::DynamicProxyHandler* handler )
	{
		checValidProxyHandler( handler );
		return co::disambiguate<co::Interface, co::MethodInfo>( new co::MethodInfo_Proxy( handler ) );
	}

	void getAttribute( const co::Any& instance, co::AttributeInfo* ai, co::Any& value )
	{
		co::MethodInfo* p = checkInstance( instance, ai );
		switch( ai->getIndex() )
		{
		case 0:		value.set< co::ArrayRange<co::ExceptionType* const> >( p->getExceptions() ); break;
		case 1:		value.set< co::ArrayRange<co::ParameterInfo* const> >( p->getParameters() ); break;
		case 2:		value.set< co::Type* >( p->getReturnType() ); break;
		default:	raiseUnexpectedMemberIndex();
		}
	}

	void setAttribute( const co::Any& instance, co::AttributeInfo* ai, const co::Any& value )
	{
		co::MethodInfo* p = checkInstance( instance, ai );
		switch( ai->getIndex() )
		{
		case 0:		raiseAttributeIsReadOnly( ai ); break;
		case 1:		raiseAttributeIsReadOnly( ai ); break;
		case 2:		raiseAttributeIsReadOnly( ai ); break;
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
	co::MethodInfo* checkInstance( const co::Any& any, co::MemberInfo* member )
	{
		if( !member )
			throw co::IllegalArgumentException( "illegal null member info" );

		// make sure that 'any' is an instance of this type
		co::InterfaceType* myType = co::typeOf<co::MethodInfo>::get();

		co::MethodInfo* res;
		if( any.getKind() != co::TK_INTERFACE || !( res = dynamic_cast<co::MethodInfo*>( any.getState().data.itf ) ) )
			CORAL_THROW( co::IllegalArgumentException, "expected a valid co::MethodInfo*, but got " << any );

		// make sure that 'member' belongs to this type
		co::CompoundType* owner = member->getOwner();
		if( owner != myType )
			CORAL_THROW( co::IllegalArgumentException, "member '" << member->getName() << "' belongs to "
				<< owner->getFullName() << ", not to co.MethodInfo" );

		return res;
	}
};

// ------ Reflector Creation Function ------ //

co::Reflector* __createMethodInfoReflector()
{
    return new MethodInfo_Reflector;
}

} // namespace co
