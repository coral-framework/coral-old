/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#include <co/IAttributeInfo.h>
#include <co/IDynamicProxyHandler.h>
#include <co/ICompoundType.h>
#include <co/IType.h>
#include <co/IMethodInfo.h>
#include <co/IllegalCastException.h>
#include <co/MissingInputException.h>
#include <co/IllegalArgumentException.h>
#include <co/reserved/ReflectorBase.h>
#include <sstream>
#include <cassert>

namespace co {

// ------ Proxy Interface ------ //

class IAttributeInfo_Proxy : public co::IAttributeInfo
{
public:
	IAttributeInfo_Proxy( co::IDynamicProxyHandler* handler ) : _handler( handler )
	{
		_cookie = _handler->registerProxyInterface( co::disambiguate<co::Interface, co::IAttributeInfo>( this ) );
	}

	virtual ~IAttributeInfo_Proxy()
	{
		// empty
	}

	// co::Interface Methods:

	co::IInterfaceType* getInterfaceType() { return co::typeOf<co::IAttributeInfo>::get(); }
	co::IComponent* getInterfaceOwner() { return _handler->getInterfaceOwner(); }
	const std::string& getInterfaceName() { return _handler->getProxyInterfaceName( _cookie ); }
	void componentRetain() { _handler->componentRetain(); }
	void componentRelease() { _handler->componentRelease(); }

	// co.IMemberInfo Methods:

	co::uint16 getIndex()
	{
		const co::Any& res = _handler->handleGetAttribute( _cookie, getAttribInfo<co::IMemberInfo>( 0 ) );
        return res.get< co::uint16 >();
	}

	const std::string& getName()
	{
		const co::Any& res = _handler->handleGetAttribute( _cookie, getAttribInfo<co::IMemberInfo>( 1 ) );
        return res.get< const std::string& >();
	}

	co::ICompoundType* getOwner()
	{
		const co::Any& res = _handler->handleGetAttribute( _cookie, getAttribInfo<co::IMemberInfo>( 2 ) );
        return res.get< co::ICompoundType* >();
	}

	// co.IAttributeInfo Methods:

	bool getIsReadOnly()
	{
		const co::Any& res = _handler->handleGetAttribute( _cookie, getAttribInfo<co::IAttributeInfo>( 0 ) );
        return res.get< bool >();
	}

	co::IType* getType()
	{
		const co::Any& res = _handler->handleGetAttribute( _cookie, getAttribInfo<co::IAttributeInfo>( 1 ) );
        return res.get< co::IType* >();
	}

protected:
	template<typename T>
	co::IAttributeInfo* getAttribInfo( co::uint32 index )
	{
		return co::typeOf<T>::get()->getMemberAttributes()[index];
	}

	template<typename T>
	co::IMethodInfo* getMethodInfo( co::uint32 index )
	{
		return co::typeOf<T>::get()->getMemberMethods()[index];
	}

private:
	co::IDynamicProxyHandler* _handler;
	co::uint32 _cookie;
};

// ------ IReflector ------ //

class IAttributeInfo_Reflector : public co::ReflectorBase
{
public:
	IAttributeInfo_Reflector()
	{
		// empty
	}

	virtual ~IAttributeInfo_Reflector()
	{
		// empty
	}

	co::IType* getType()
	{
		return co::typeOf<co::IAttributeInfo>::get();
	}

	co::int32 getSize()
	{
		return sizeof(co::IAttributeInfo);
	}

	co::Interface* newProxy( co::IDynamicProxyHandler* handler )
	{
		checValidProxyHandler( handler );
		return co::disambiguate<co::Interface, co::IAttributeInfo>( new co::IAttributeInfo_Proxy( handler ) );
	}

	void getAttribute( const co::Any& instance, co::IAttributeInfo* ai, co::Any& value )
	{
		co::IAttributeInfo* p = checkInstance( instance, ai );
		switch( ai->getIndex() )
		{
		case 0:		value.set< bool >( p->getIsReadOnly() ); break;
		case 1:		value.set< co::IType* >( p->getType() ); break;
		default:	raiseUnexpectedMemberIndex();
		}
	}

	void setAttribute( const co::Any& instance, co::IAttributeInfo* ai, const co::Any& value )
	{
		co::IAttributeInfo* p = checkInstance( instance, ai );
		switch( ai->getIndex() )
		{
		case 0:		raiseAttributeIsReadOnly( ai ); break;
		case 1:		raiseAttributeIsReadOnly( ai ); break;
		default:	raiseUnexpectedMemberIndex();
		}
		CORAL_UNUSED( p );
		CORAL_UNUSED( value );
	}

	void invokeMethod( const co::Any& instance, co::IMethodInfo* mi, co::ArrayRange<co::Any const> args, co::Any& res )
	{
		checkInstance( instance, mi );
		raiseUnexpectedMemberIndex();
		CORAL_UNUSED( args );
		CORAL_UNUSED( res );
	}

private:
	co::IAttributeInfo* checkInstance( const co::Any& any, co::IMemberInfo* member )
	{
		if( !member )
			throw co::IllegalArgumentException( "illegal null member info" );

		// make sure that 'any' is an instance of this type
		co::IInterfaceType* myType = co::typeOf<co::IAttributeInfo>::get();

		co::IAttributeInfo* res;
		if( any.getKind() != co::TK_INTERFACE || !( res = dynamic_cast<co::IAttributeInfo*>( any.getState().data.itf ) ) )
			CORAL_THROW( co::IllegalArgumentException, "expected a valid co::IAttributeInfo*, but got " << any );

		// make sure that 'member' belongs to this type
		co::ICompoundType* owner = member->getOwner();
		if( owner != myType )
			CORAL_THROW( co::IllegalArgumentException, "member '" << member->getName() << "' belongs to "
				<< owner->getFullName() << ", not to co.IAttributeInfo" );

		return res;
	}
};

// ------ IReflector Creation Function ------ //

co::IReflector* __createIAttributeInfoIReflector()
{
    return new IAttributeInfo_Reflector;
}

} // namespace co
