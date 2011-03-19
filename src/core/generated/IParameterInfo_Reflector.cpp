/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#include <co/IParameterInfo.h>
#include <co/IDynamicProxyHandler.h>
#include <co/IType.h>
#include <co/IMethodInfo.h>
#include <co/IAttributeInfo.h>
#include <co/IllegalCastException.h>
#include <co/MissingInputException.h>
#include <co/IllegalArgumentException.h>
#include <co/reserved/ReflectorBase.h>
#include <sstream>
#include <cassert>

namespace co {

// ------ Proxy Interface ------ //

class IParameterInfo_Proxy : public co::IParameterInfo
{
public:
	IParameterInfo_Proxy( co::IDynamicProxyHandler* handler ) : _handler( handler )
	{
		_cookie = _handler->registerProxyInterface( co::disambiguate<co::Interface, co::IParameterInfo>( this ) );
	}

	virtual ~IParameterInfo_Proxy()
	{
		// empty
	}

	// co::Interface Methods:

	co::IInterfaceType* getInterfaceType() { return co::typeOf<co::IParameterInfo>::get(); }
	co::IComponent* getInterfaceOwner() { return _handler->getInterfaceOwner(); }
	const std::string& getInterfaceName() { return _handler->getProxyInterfaceName( _cookie ); }
	void componentRetain() { _handler->componentRetain(); }
	void componentRelease() { _handler->componentRelease(); }

	// co.IParameterInfo Methods:

	bool getIsIn()
	{
		const co::Any& res = _handler->handleGetAttribute( _cookie, getAttribInfo<co::IParameterInfo>( 0 ) );
        return res.get< bool >();
	}

	bool getIsOut()
	{
		const co::Any& res = _handler->handleGetAttribute( _cookie, getAttribInfo<co::IParameterInfo>( 1 ) );
        return res.get< bool >();
	}

	const std::string& getName()
	{
		const co::Any& res = _handler->handleGetAttribute( _cookie, getAttribInfo<co::IParameterInfo>( 2 ) );
        return res.get< const std::string& >();
	}

	co::IType* getType()
	{
		const co::Any& res = _handler->handleGetAttribute( _cookie, getAttribInfo<co::IParameterInfo>( 3 ) );
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

class IParameterInfo_Reflector : public co::ReflectorBase
{
public:
	IParameterInfo_Reflector()
	{
		// empty
	}

	virtual ~IParameterInfo_Reflector()
	{
		// empty
	}

	co::IType* getType()
	{
		return co::typeOf<co::IParameterInfo>::get();
	}

	co::int32 getSize()
	{
		return sizeof(co::IParameterInfo);
	}

	co::Interface* newProxy( co::IDynamicProxyHandler* handler )
	{
		checValidProxyHandler( handler );
		return co::disambiguate<co::Interface, co::IParameterInfo>( new co::IParameterInfo_Proxy( handler ) );
	}

	void getAttribute( const co::Any& instance, co::IAttributeInfo* ai, co::Any& value )
	{
		co::IParameterInfo* p = checkInstance( instance, ai );
		switch( ai->getIndex() )
		{
		case 0:		value.set< bool >( p->getIsIn() ); break;
		case 1:		value.set< bool >( p->getIsOut() ); break;
		case 2:		value.set< const std::string& >( p->getName() ); break;
		case 3:		value.set< co::IType* >( p->getType() ); break;
		default:	raiseUnexpectedMemberIndex();
		}
	}

	void setAttribute( const co::Any& instance, co::IAttributeInfo* ai, const co::Any& value )
	{
		co::IParameterInfo* p = checkInstance( instance, ai );
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

	void invokeMethod( const co::Any& instance, co::IMethodInfo* mi, co::ArrayRange<co::Any const> args, co::Any& res )
	{
		checkInstance( instance, mi );
		raiseUnexpectedMemberIndex();
		CORAL_UNUSED( args );
		CORAL_UNUSED( res );
	}

private:
	co::IParameterInfo* checkInstance( const co::Any& any, co::IMemberInfo* member )
	{
		if( !member )
			throw co::IllegalArgumentException( "illegal null member info" );

		// make sure that 'any' is an instance of this type
		co::IInterfaceType* myType = co::typeOf<co::IParameterInfo>::get();

		co::IParameterInfo* res;
		if( any.getKind() != co::TK_INTERFACE || !( res = dynamic_cast<co::IParameterInfo*>( any.getState().data.itf ) ) )
			CORAL_THROW( co::IllegalArgumentException, "expected a valid co::IParameterInfo*, but got " << any );

		// make sure that 'member' belongs to this type
		co::ICompoundType* owner = member->getOwner();
		if( owner != myType )
			CORAL_THROW( co::IllegalArgumentException, "member '" << member->getName() << "' belongs to "
				<< owner->getFullName() << ", not to co.IParameterInfo" );

		return res;
	}
};

// ------ IReflector Creation Function ------ //

co::IReflector* __createIParameterInfoIReflector()
{
    return new IParameterInfo_Reflector;
}

} // namespace co
