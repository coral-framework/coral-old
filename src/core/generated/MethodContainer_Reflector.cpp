/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#include <co/MethodContainer.h>
#include <co/DynamicProxyHandler.h>
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

class MethodContainer_Proxy : public co::MethodContainer
{
public:
	MethodContainer_Proxy( co::DynamicProxyHandler* handler ) : _handler( handler )
	{
		_cookie = _handler->registerProxyInterface( co::disambiguate<co::Interface, co::MethodContainer>( this ) );
	}

	virtual ~MethodContainer_Proxy()
	{
		// empty
	}

	// co::Interface Methods:

	co::InterfaceType* getInterfaceType() { return co::typeOf<co::MethodContainer>::get(); }
	co::Component* getInterfaceOwner() { return _handler->getInterfaceOwner(); }
	const std::string& getInterfaceName() { return _handler->getProxyInterfaceName( _cookie ); }
	void componentRetain() { _handler->componentRetain(); }
	void componentRelease() { _handler->componentRelease(); }

	// co.MethodContainer Methods:

	co::ArrayRange<co::MethodInfo* const> getMemberMethods()
	{
		const co::Any& res = _handler->handleGetAttribute( _cookie, getAttribInfo<co::MethodContainer>( 0 ) );
        return res.get< co::ArrayRange<co::MethodInfo* const> >();
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

class MethodContainer_Reflector : public co::ReflectorBase
{
public:
	MethodContainer_Reflector()
	{
		// empty
	}

	virtual ~MethodContainer_Reflector()
	{
		// empty
	}

	co::Type* getType()
	{
		return co::typeOf<co::MethodContainer>::get();
	}

	co::int32 getSize()
	{
		return sizeof(co::MethodContainer);
	}

	co::Interface* newProxy( co::DynamicProxyHandler* handler )
	{
		checValidProxyHandler( handler );
		return co::disambiguate<co::Interface, co::MethodContainer>( new co::MethodContainer_Proxy( handler ) );
	}

	void getAttribute( const co::Any& instance, co::AttributeInfo* ai, co::Any& value )
	{
		co::MethodContainer* p = checkInstance( instance, ai );
		switch( ai->getIndex() )
		{
		case 0:		value.set< co::ArrayRange<co::MethodInfo* const> >( p->getMemberMethods() ); break;
		default:	raiseUnexpectedMemberIndex();
		}
	}

	void setAttribute( const co::Any& instance, co::AttributeInfo* ai, const co::Any& value )
	{
		co::MethodContainer* p = checkInstance( instance, ai );
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
	co::MethodContainer* checkInstance( const co::Any& any, co::MemberInfo* member )
	{
		if( !member )
			throw co::IllegalArgumentException( "illegal null member info" );

		// make sure that 'any' is an instance of this type
		co::InterfaceType* myType = co::typeOf<co::MethodContainer>::get();

		co::MethodContainer* res;
		if( any.getKind() != co::TK_INTERFACE || !( res = dynamic_cast<co::MethodContainer*>( any.getState().data.itf ) ) )
			CORAL_THROW( co::IllegalArgumentException, "expected a valid co::MethodContainer*, but got " << any );

		// make sure that 'member' belongs to this type
		co::CompoundType* owner = member->getOwner();
		if( owner != myType )
			CORAL_THROW( co::IllegalArgumentException, "member '" << member->getName() << "' belongs to "
				<< owner->getFullName() << ", not to co.MethodContainer" );

		return res;
	}
};

// ------ Reflector Creation Function ------ //

co::Reflector* __createMethodContainerReflector()
{
    return new MethodContainer_Reflector;
}

} // namespace co
