/*******************************************************************************
** Reflection code generated for type 'co.DynamicProxyHandler'
**
** Created: Thu Jun 10 20:53:30 2010
**      by: Coral Compiler version 0.1.0
**
** WARNING! All changes made in this file will be lost when recompiling!
********************************************************************************/

#include <co/DynamicProxyHandler.h>
#include <co/reserved/ReflectorBase.h>
#include <co/DynamicProxyHandler.h>
#include <co/AttributeInfo.h>
#include <co/MethodInfo.h>
#include <co/MethodInfo.h>
#include <co/AttributeInfo.h>
#include <co/IllegalCastException.h>
#include <co/MissingInputException.h>
#include <co/IllegalArgumentException.h>
#include <sstream>

// ------ Proxy Interface ------ //

class DynamicProxyHandler_Proxy : public co::DynamicProxyHandler
{
public:
	DynamicProxyHandler_Proxy( co::DynamicProxyHandler* handler ) : _handler( handler )
	{
		_cookie = _handler->registerProxyInterface( co::disambiguate<co::Interface, co::DynamicProxyHandler>( this ) );
	}

	virtual ~DynamicProxyHandler_Proxy()
	{
		// empty destructor
	}

	// co::Interface Methods:

	co::InterfaceType* getInterfaceType() { return co::typeOf<co::DynamicProxyHandler>::get(); }
	co::Component* getInterfaceOwner() { return _handler->getInterfaceOwner(); }
	const std::string& getInterfaceName() { return _handler->getProxyInterfaceName( _cookie ); }
	void componentRetain() { _handler->componentRetain(); }
	void componentRelease() { _handler->componentRelease(); }

	// co::DynamicProxyHandler Methods:

	const std::string& getProxyInterfaceName( co::int32 cookie )
	{
		co::Any __res, __arg[1];
		__arg[0].set< co::int32 >( cookie );
		co::ArrayRange<co::Any const> __ar( __arg, 1 );
		_handler->handleMethodInvocation( _cookie, getMethodInfo<co::DynamicProxyHandler>( 0 ), __ar, __res );
		return __res.get< const std::string& >();
	}

	void handleGetAttribute( co::int32 cookie, co::AttributeInfo* ai, co::Any& value )
	{
		co::Any __res, __arg[3];
		__arg[0].set< co::int32 >( cookie );
		__arg[1].set< co::AttributeInfo* >( ai );
		__arg[2].set< co::Any& >( value );
		co::ArrayRange<co::Any const> __ar( __arg, 3 );
		_handler->handleMethodInvocation( _cookie, getMethodInfo<co::DynamicProxyHandler>( 1 ), __ar, __res );
	}

	void handleMethodInvocation( co::int32 cookie, co::MethodInfo* mi, co::ArrayRange<co::Any const> args, co::Any& returnValue )
	{
		co::Any __res, __arg[4];
		__arg[0].set< co::int32 >( cookie );
		__arg[1].set< co::MethodInfo* >( mi );
		__arg[2].set< co::ArrayRange<co::Any const> >( args );
		__arg[3].set< co::Any& >( returnValue );
		co::ArrayRange<co::Any const> __ar( __arg, 4 );
		_handler->handleMethodInvocation( _cookie, getMethodInfo<co::DynamicProxyHandler>( 2 ), __ar, __res );
	}

	void handleSetAttribute( co::int32 cookie, co::AttributeInfo* ai, const co::Any& value )
	{
		co::Any __res, __arg[3];
		__arg[0].set< co::int32 >( cookie );
		__arg[1].set< co::AttributeInfo* >( ai );
		__arg[2].set< const co::Any& >( value );
		co::ArrayRange<co::Any const> __ar( __arg, 3 );
		_handler->handleMethodInvocation( _cookie, getMethodInfo<co::DynamicProxyHandler>( 3 ), __ar, __res );
	}

	co::int32 registerProxyInterface( co::Interface* proxy )
	{
		co::Any __res, __arg[1];
		__arg[0].set< co::Interface* >( proxy );
		co::ArrayRange<co::Any const> __ar( __arg, 1 );
		_handler->handleMethodInvocation( _cookie, getMethodInfo<co::DynamicProxyHandler>( 4 ), __ar, __res );
		return __res.get< co::int32 >();
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

class DynamicProxyHandler_Reflector : public co::ReflectorBase
{
public:
	DynamicProxyHandler_Reflector()
	{
		// empty
	}

	virtual ~DynamicProxyHandler_Reflector()
	{
		// empty
	}

	co::Type* getType()
	{
		return co::typeOf<co::DynamicProxyHandler>::get();
	}

	co::int32 getSize()
	{
		return sizeof(co::DynamicProxyHandler);
	}

	co::Interface* newProxy( co::DynamicProxyHandler* handler )
	{
		checValidProxyHandler( handler );
		return co::disambiguate<co::Interface, co::DynamicProxyHandler>( new DynamicProxyHandler_Proxy( handler ) );
	}

	void getAttribute( const co::Any& instance, co::AttributeInfo* ai, co::Any& value )
	{
		checkInstance( instance, ai );
		raiseUnexpectedMemberIndex();
		CORAL_UNUSED( value );
	}

	void setAttribute( const co::Any& instance, co::AttributeInfo* ai, const co::Any& value )
	{
		checkInstance( instance, ai );
		raiseUnexpectedMemberIndex();
		CORAL_UNUSED( value );
	}

	void invokeMethod( const co::Any& instance, co::MethodInfo* mi, co::ArrayRange<co::Any const> args, co::Any& __res )
	{
		co::DynamicProxyHandler* p = checkInstance( instance, mi );
		checkNumArguments( mi, args.getSize() );
		int argIndex = -1;
		try
		{
			switch( mi->getIndex() )
			{
			case 0:
				{
					co::int32 cookie = args[++argIndex].get< co::int32 >();
					argIndex = -1;
					__res.set< const std::string& >( p->getProxyInterfaceName( cookie ) );
				}
				break;
			case 1:
				{
					co::int32 cookie = args[++argIndex].get< co::int32 >();
					co::AttributeInfo* ai = args[++argIndex].get< co::AttributeInfo* >();
					co::Any& value = args[++argIndex].get< co::Any& >();
					argIndex = -1;
					p->handleGetAttribute( cookie, ai, value );
				}
				break;
			case 2:
				{
					co::int32 cookie = args[++argIndex].get< co::int32 >();
					co::MethodInfo* mi = args[++argIndex].get< co::MethodInfo* >();
					co::ArrayRange<co::Any const> args = args[++argIndex].get< co::ArrayRange<co::Any const> >();
					co::Any& returnValue = args[++argIndex].get< co::Any& >();
					argIndex = -1;
					p->handleMethodInvocation( cookie, mi, args, returnValue );
				}
				break;
			case 3:
				{
					co::int32 cookie = args[++argIndex].get< co::int32 >();
					co::AttributeInfo* ai = args[++argIndex].get< co::AttributeInfo* >();
					const co::Any& value = args[++argIndex].get< const co::Any& >();
					argIndex = -1;
					p->handleSetAttribute( cookie, ai, value );
				}
				break;
			case 4:
				{
					co::Interface* proxy = args[++argIndex].get< co::Interface* >();
					argIndex = -1;
					__res.set< co::int32 >( p->registerProxyInterface( proxy ) );
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
	co::DynamicProxyHandler* checkInstance( const co::Any& instance, co::MemberInfo* member )
	{
		if( !member )
			throw co::IllegalArgumentException( "illegal null member info" );

		co::InterfaceType* myType = co::typeOf<co::DynamicProxyHandler>::get();

		// make sure that 'instance' is an instance of this type
		if( instance.getKind() != co::TK_INTERFACE ||
			!instance.getInterfaceType()->isSubTypeOf( myType ) ||
			instance.getState().data.ptr == NULL )
			CORAL_THROW( co::IllegalArgumentException, "expected a valid co::DynamicProxyHandler*, but got " << instance );

		// make sure that 'member' belongs to this type
		co::CompoundType* owner = member->getOwner();
		if( owner != myType )
			CORAL_THROW( co::IllegalArgumentException, "member '" << member->getName() << "' belongs to "
				<< owner->getFullName() << ", not to " << myType->getFullName() );

		return reinterpret_cast<co::DynamicProxyHandler*>( instance.getState().data.ptr );
	}
};

// ------ Reflector Creation Function ------ //

namespace __coral {

co::Reflector* createDynamicProxyHandlerReflector()
{
    return new DynamicProxyHandler_Reflector;
}

} // namespace __coral
