/*
 * Coral - A C++ Component Framework.
 * See Copyright Notice in Coral.h
 */

#include <co/DynamicProxyHandler.h>
#include <co/AttributeInfo.h>
#include <co/MethodInfo.h>
#include <co/IllegalCastException.h>
#include <co/MissingInputException.h>
#include <co/IllegalArgumentException.h>
#include <co/reserved/ReflectorBase.h>
#include <sstream>
#include <cassert>

namespace co {

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
		// empty
	}

	// co::Interface Methods:

	co::InterfaceType* getInterfaceType() { return co::typeOf<co::DynamicProxyHandler>::get(); }
	co::Component* getInterfaceOwner() { return _handler->getInterfaceOwner(); }
	const std::string& getInterfaceName() { return _handler->getProxyInterfaceName( _cookie ); }
	void componentRetain() { _handler->componentRetain(); }
	void componentRelease() { _handler->componentRelease(); }

	// co.DynamicProxyHandler Methods:

	const std::string& getProxyInterfaceName( co::int32 __cookie )
	{
		co::Any res, args[1];
		args[0].set< co::int32 >( __cookie );
		co::ArrayRange<co::Any const> range( args, 1 );
		_handler->handleMethodInvocation( _cookie, getMethodInfo<co::DynamicProxyHandler>( 0 ), range, res );
		assert( res.containsObject() == false );
		return res.get< const std::string& >();
	}

	void handleGetAttribute( co::int32 __cookie, co::AttributeInfo* __ai, co::Any& __value )
	{
		co::Any res, args[3];
		args[0].set< co::int32 >( __cookie );
		args[1].set< co::AttributeInfo* >( __ai );
		args[2].set< co::Any& >( __value );
		co::ArrayRange<co::Any const> range( args, 3 );
		_handler->handleMethodInvocation( _cookie, getMethodInfo<co::DynamicProxyHandler>( 1 ), range, res );
	}

	void handleMethodInvocation( co::int32 __cookie, co::MethodInfo* __mi, co::ArrayRange<co::Any const> __args, co::Any& __returnValue )
	{
		co::Any res, args[4];
		args[0].set< co::int32 >( __cookie );
		args[1].set< co::MethodInfo* >( __mi );
		args[2].set< co::ArrayRange<co::Any const> >( __args );
		args[3].set< co::Any& >( __returnValue );
		co::ArrayRange<co::Any const> range( args, 4 );
		_handler->handleMethodInvocation( _cookie, getMethodInfo<co::DynamicProxyHandler>( 2 ), range, res );
	}

	void handleSetAttribute( co::int32 __cookie, co::AttributeInfo* __ai, const co::Any& __value )
	{
		co::Any res, args[3];
		args[0].set< co::int32 >( __cookie );
		args[1].set< co::AttributeInfo* >( __ai );
		args[2].set< const co::Any& >( __value );
		co::ArrayRange<co::Any const> range( args, 3 );
		_handler->handleMethodInvocation( _cookie, getMethodInfo<co::DynamicProxyHandler>( 3 ), range, res );
	}

	co::int32 registerProxyInterface( co::Interface* __proxy )
	{
		co::Any res, args[1];
		args[0].set< co::Interface* >( __proxy );
		co::ArrayRange<co::Any const> range( args, 1 );
		_handler->handleMethodInvocation( _cookie, getMethodInfo<co::DynamicProxyHandler>( 4 ), range, res );
		assert( res.containsObject() == false );
		return res.get< co::int32 >();
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
		return co::disambiguate<co::Interface, co::DynamicProxyHandler>( new co::DynamicProxyHandler_Proxy( handler ) );
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

	void invokeMethod( const co::Any& instance, co::MethodInfo* mi, co::ArrayRange<co::Any const> args, co::Any& res )
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
					co::int32 __cookie = args[++argIndex].get< co::int32 >();
					argIndex = -1;
					res.set< const std::string& >( p->getProxyInterfaceName( __cookie ) );
				}
				break;
			case 1:
				{
					co::int32 __cookie = args[++argIndex].get< co::int32 >();
					co::AttributeInfo* __ai = args[++argIndex].get< co::AttributeInfo* >();
					co::Any& __value = args[++argIndex].get< co::Any& >();
					argIndex = -1;
					p->handleGetAttribute( __cookie, __ai, __value );
				}
				break;
			case 2:
				{
					co::int32 __cookie = args[++argIndex].get< co::int32 >();
					co::MethodInfo* __mi = args[++argIndex].get< co::MethodInfo* >();
					co::ArrayRange<co::Any const> __args = args[++argIndex].get< co::ArrayRange<co::Any const> >();
					co::Any& __returnValue = args[++argIndex].get< co::Any& >();
					argIndex = -1;
					p->handleMethodInvocation( __cookie, __mi, __args, __returnValue );
				}
				break;
			case 3:
				{
					co::int32 __cookie = args[++argIndex].get< co::int32 >();
					co::AttributeInfo* __ai = args[++argIndex].get< co::AttributeInfo* >();
					const co::Any& __value = args[++argIndex].get< const co::Any& >();
					argIndex = -1;
					p->handleSetAttribute( __cookie, __ai, __value );
				}
				break;
			case 4:
				{
					co::Interface* __proxy = args[++argIndex].get< co::Interface* >();
					argIndex = -1;
					res.set< co::int32 >( p->registerProxyInterface( __proxy ) );
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
		CORAL_UNUSED( res );
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

		return dynamic_cast<co::DynamicProxyHandler*>( instance.getState().data.itf );
	}
};

// ------ Reflector Creation Function ------ //

co::Reflector* __createDynamicProxyHandlerReflector()
{
    return new DynamicProxyHandler_Reflector;
}

} // namespace co
