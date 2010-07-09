/*******************************************************************************
** Reflection code generated for type 'co.Component'
**
** Created: Fri Jul 02 00:14:15 2010
**      by: Coral Compiler version 0.1.0
**
** WARNING! All changes made in this file will be lost when recompiling!
********************************************************************************/

#include <co/Component.h>
#include <co/reserved/ReflectorBase.h>
#include <co/DynamicProxyHandler.h>
#include <co/ComponentType.h>
#include <co/InterfaceInfo.h>
#include <co/MethodInfo.h>
#include <co/AttributeInfo.h>
#include <co/IllegalCastException.h>
#include <co/MissingInputException.h>
#include <co/IllegalArgumentException.h>
#include <sstream>
#include <cassert>

namespace co {

// ------ Proxy Interface ------ //

class Component_Proxy : public co::Component
{
public:
	Component_Proxy( co::DynamicProxyHandler* handler ) : _handler( handler )
	{
		_cookie = _handler->registerProxyInterface( co::disambiguate<co::Interface, co::Component>( this ) );
	}

	virtual ~Component_Proxy()
	{
		// empty
	}

	// co::Interface Methods:

	co::InterfaceType* getInterfaceType() { return co::typeOf<co::Component>::get(); }
	co::Component* getInterfaceOwner() { return _handler->getInterfaceOwner(); }
	const std::string& getInterfaceName() { return _handler->getProxyInterfaceName( _cookie ); }
	void componentRetain() { _handler->componentRetain(); }
	void componentRelease() { _handler->componentRelease(); }

	// co::Component Methods:

	co::ComponentType* getComponentType()
	{
		co::Any __res;
		_handler->handleGetAttribute( _cookie, getAttribInfo<co::Component>( 0 ), __res );
        return __res.get< co::ComponentType* >();
	}

	void bindInterface( co::InterfaceInfo* clientInterface, co::Interface* instance )
	{
		co::Any __res, __arg[2];
		__arg[0].set< co::InterfaceInfo* >( clientInterface );
		__arg[1].set< co::Interface* >( instance );
		co::ArrayRange<co::Any const> __ar( __arg, 2 );
		_handler->handleMethodInvocation( _cookie, getMethodInfo<co::Component>( 1 ), __ar, __res );
	}

	co::Interface* getInterface( co::InterfaceInfo* interfaceInfo )
	{
		co::Any __res, __arg[1];
		__arg[0].set< co::InterfaceInfo* >( interfaceInfo );
		co::ArrayRange<co::Any const> __ar( __arg, 1 );
		_handler->handleMethodInvocation( _cookie, getMethodInfo<co::Component>( 2 ), __ar, __res );
		return __res.get< co::Interface* >();
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

class Component_Reflector : public co::ReflectorBase
{
public:
	Component_Reflector()
	{
		// empty
	}

	virtual ~Component_Reflector()
	{
		// empty
	}

	co::Type* getType()
	{
		return co::typeOf<co::Component>::get();
	}

	co::int32 getSize()
	{
		return sizeof(co::Component);
	}

	co::Interface* newProxy( co::DynamicProxyHandler* handler )
	{
		checValidProxyHandler( handler );
		return co::disambiguate<co::Interface, co::Component>( new co::Component_Proxy( handler ) );
	}

	void getAttribute( const co::Any& instance, co::AttributeInfo* ai, co::Any& value )
	{
		co::Component* p = checkInstance( instance, ai );
		switch( ai->getIndex() )
		{
		case 0:		value.set< co::ComponentType* >( p->getComponentType() ); break;
		default:	raiseUnexpectedMemberIndex();
		}
	}

	void setAttribute( const co::Any& instance, co::AttributeInfo* ai, const co::Any& value )
	{
		co::Component* p = checkInstance( instance, ai );
		switch( ai->getIndex() )
		{
		case 0:		raiseAttributeIsReadOnly( ai ); break;
		default:	raiseUnexpectedMemberIndex();
		}
		CORAL_UNUSED( p );
		CORAL_UNUSED( value );
	}

	void invokeMethod( const co::Any& instance, co::MethodInfo* mi, co::ArrayRange<co::Any const> args, co::Any& __res )
	{
		co::Component* p = checkInstance( instance, mi );
		checkNumArguments( mi, args.getSize() );
		int argIndex = -1;
		try
		{
			switch( mi->getIndex() )
			{
			case 1:
				{
					co::InterfaceInfo* clientInterface = args[++argIndex].get< co::InterfaceInfo* >();
					co::Interface* instance = args[++argIndex].get< co::Interface* >();
					argIndex = -1;
					p->bindInterface( clientInterface, instance );
				}
				break;
			case 2:
				{
					co::InterfaceInfo* interfaceInfo = args[++argIndex].get< co::InterfaceInfo* >();
					argIndex = -1;
					__res.set< co::Interface* >( p->getInterface( interfaceInfo ) );
				}
				break;
			default:
				raiseUnexpectedMemberIndex();
			}
		}
		catch( co::IllegalCastException& e )
		{
			if( argIndex == -1 )
				throw; // just re-throw if the exception is not related to 'args'
			raiseArgumentTypeException( mi, argIndex, e );
		}
		catch( ... )
		{
			throw;
		}
		CORAL_UNUSED( __res );
	}

private:
	co::Component* checkInstance( const co::Any& instance, co::MemberInfo* member )
	{
		if( !member )
			throw co::IllegalArgumentException( "illegal null member info" );

		co::InterfaceType* myType = co::typeOf<co::Component>::get();

		// make sure that 'instance' is an instance of this type
		if( instance.getKind() != co::TK_INTERFACE ||
			!instance.getInterfaceType()->isSubTypeOf( myType ) ||
			instance.getState().data.ptr == NULL )
			CORAL_THROW( co::IllegalArgumentException, "expected a valid co::Component*, but got " << instance );

		// make sure that 'member' belongs to this type
		co::CompoundType* owner = member->getOwner();
		if( owner != myType )
			CORAL_THROW( co::IllegalArgumentException, "member '" << member->getName() << "' belongs to "
				<< owner->getFullName() << ", not to " << myType->getFullName() );

		return reinterpret_cast<co::Component*>( instance.getState().data.ptr );
	}
};

// ------ Reflector Creation Function ------ //

co::Reflector* __createComponentReflector()
{
    return new Component_Reflector;
}

} // namespace co
