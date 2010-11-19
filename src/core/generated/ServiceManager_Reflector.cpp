/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#include <co/ServiceManager.h>
#include <co/DynamicProxyHandler.h>
#include <co/InterfaceType.h>
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

class ServiceManager_Proxy : public co::ServiceManager
{
public:
	ServiceManager_Proxy( co::DynamicProxyHandler* handler ) : _handler( handler )
	{
		_cookie = _handler->registerProxyInterface( co::disambiguate<co::Interface, co::ServiceManager>( this ) );
	}

	virtual ~ServiceManager_Proxy()
	{
		// empty
	}

	// co::Interface Methods:

	co::InterfaceType* getInterfaceType() { return co::typeOf<co::ServiceManager>::get(); }
	co::Component* getInterfaceOwner() { return _handler->getInterfaceOwner(); }
	const std::string& getInterfaceName() { return _handler->getProxyInterfaceName( _cookie ); }
	void componentRetain() { _handler->componentRetain(); }
	void componentRelease() { _handler->componentRelease(); }

	// co.ServiceManager Methods:

	bool getIsLazy()
	{
		const co::Any& res = _handler->handleGetAttribute( _cookie, getAttribInfo<co::ServiceManager>( 0 ) );
        return res.get< bool >();
	}

	void setIsLazy( bool isLazy_ )
	{
		co::Any arg;
		arg.set< bool >( isLazy_ );
		_handler->handleSetAttribute( _cookie, getAttribInfo<co::ServiceManager>( 0 ), arg );
	}

	void addService( co::InterfaceType* serviceType_, co::Interface* serviceInstance_ )
	{
		co::Any args[2];
		args[0].set< co::InterfaceType* >( serviceType_ );
		args[1].set< co::Interface* >( serviceInstance_ );
		co::ArrayRange<co::Any const> range( args, 2 );
		_handler->handleMethodInvocation( _cookie, getMethodInfo<co::ServiceManager>( 0 ), range );
	}

	void addServiceForType( co::InterfaceType* serviceType_, co::InterfaceType* clientType_, co::Interface* serviceInstance_ )
	{
		co::Any args[3];
		args[0].set< co::InterfaceType* >( serviceType_ );
		args[1].set< co::InterfaceType* >( clientType_ );
		args[2].set< co::Interface* >( serviceInstance_ );
		co::ArrayRange<co::Any const> range( args, 3 );
		_handler->handleMethodInvocation( _cookie, getMethodInfo<co::ServiceManager>( 1 ), range );
	}

	void addServiceImplementation( co::InterfaceType* serviceType_, const std::string& componentName_ )
	{
		co::Any args[2];
		args[0].set< co::InterfaceType* >( serviceType_ );
		args[1].set< const std::string& >( componentName_ );
		co::ArrayRange<co::Any const> range( args, 2 );
		_handler->handleMethodInvocation( _cookie, getMethodInfo<co::ServiceManager>( 2 ), range );
	}

	void addServiceImplementationForType( co::InterfaceType* serviceType_, co::InterfaceType* clientType_, const std::string& componentName_ )
	{
		co::Any args[3];
		args[0].set< co::InterfaceType* >( serviceType_ );
		args[1].set< co::InterfaceType* >( clientType_ );
		args[2].set< const std::string& >( componentName_ );
		co::ArrayRange<co::Any const> range( args, 3 );
		_handler->handleMethodInvocation( _cookie, getMethodInfo<co::ServiceManager>( 3 ), range );
	}

	co::Interface* getService( co::InterfaceType* serviceType_ )
	{
		co::Any args[1];
		args[0].set< co::InterfaceType* >( serviceType_ );
		co::ArrayRange<co::Any const> range( args, 1 );
		const co::Any& res = _handler->handleMethodInvocation( _cookie, getMethodInfo<co::ServiceManager>( 4 ), range );
		return res.get< co::Interface* >();
	}

	co::Interface* getServiceForInstance( co::InterfaceType* serviceType_, co::Interface* clientInstance_ )
	{
		co::Any args[2];
		args[0].set< co::InterfaceType* >( serviceType_ );
		args[1].set< co::Interface* >( clientInstance_ );
		co::ArrayRange<co::Any const> range( args, 2 );
		const co::Any& res = _handler->handleMethodInvocation( _cookie, getMethodInfo<co::ServiceManager>( 5 ), range );
		return res.get< co::Interface* >();
	}

	co::Interface* getServiceForType( co::InterfaceType* serviceType_, co::InterfaceType* clientType_ )
	{
		co::Any args[2];
		args[0].set< co::InterfaceType* >( serviceType_ );
		args[1].set< co::InterfaceType* >( clientType_ );
		co::ArrayRange<co::Any const> range( args, 2 );
		const co::Any& res = _handler->handleMethodInvocation( _cookie, getMethodInfo<co::ServiceManager>( 6 ), range );
		return res.get< co::Interface* >();
	}

	void removeService( co::InterfaceType* serviceType_ )
	{
		co::Any args[1];
		args[0].set< co::InterfaceType* >( serviceType_ );
		co::ArrayRange<co::Any const> range( args, 1 );
		_handler->handleMethodInvocation( _cookie, getMethodInfo<co::ServiceManager>( 7 ), range );
	}

	void removeServiceForType( co::InterfaceType* serviceType_, co::InterfaceType* clientType_ )
	{
		co::Any args[2];
		args[0].set< co::InterfaceType* >( serviceType_ );
		args[1].set< co::InterfaceType* >( clientType_ );
		co::ArrayRange<co::Any const> range( args, 2 );
		_handler->handleMethodInvocation( _cookie, getMethodInfo<co::ServiceManager>( 8 ), range );
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

class ServiceManager_Reflector : public co::ReflectorBase
{
public:
	ServiceManager_Reflector()
	{
		// empty
	}

	virtual ~ServiceManager_Reflector()
	{
		// empty
	}

	co::Type* getType()
	{
		return co::typeOf<co::ServiceManager>::get();
	}

	co::int32 getSize()
	{
		return sizeof(co::ServiceManager);
	}

	co::Interface* newProxy( co::DynamicProxyHandler* handler )
	{
		checValidProxyHandler( handler );
		return co::disambiguate<co::Interface, co::ServiceManager>( new co::ServiceManager_Proxy( handler ) );
	}

	void getAttribute( const co::Any& instance, co::AttributeInfo* ai, co::Any& value )
	{
		co::ServiceManager* p = checkInstance( instance, ai );
		switch( ai->getIndex() )
		{
		case 0:		value.set< bool >( p->getIsLazy() ); break;
		default:	raiseUnexpectedMemberIndex();
		}
	}

	void setAttribute( const co::Any& instance, co::AttributeInfo* ai, const co::Any& value )
	{
		co::ServiceManager* p = checkInstance( instance, ai );
		switch( ai->getIndex() )
		{
		case 0:		p->setIsLazy( value.get< bool >() ); break;
		default:	raiseUnexpectedMemberIndex();
		}
		CORAL_UNUSED( p );
		CORAL_UNUSED( value );
	}

	void invokeMethod( const co::Any& instance, co::MethodInfo* mi, co::ArrayRange<co::Any const> args, co::Any& res )
	{
		co::ServiceManager* p = checkInstance( instance, mi );
		checkNumArguments( mi, args.getSize() );
		int argIndex = -1;
		try
		{
			switch( mi->getIndex() )
			{
			case 1:
				{
					co::InterfaceType* serviceType_ = args[++argIndex].get< co::InterfaceType* >();
					co::Interface* serviceInstance_ = args[++argIndex].get< co::Interface* >();
					argIndex = -1;
					p->addService( serviceType_, serviceInstance_ );
				}
				break;
			case 2:
				{
					co::InterfaceType* serviceType_ = args[++argIndex].get< co::InterfaceType* >();
					co::InterfaceType* clientType_ = args[++argIndex].get< co::InterfaceType* >();
					co::Interface* serviceInstance_ = args[++argIndex].get< co::Interface* >();
					argIndex = -1;
					p->addServiceForType( serviceType_, clientType_, serviceInstance_ );
				}
				break;
			case 3:
				{
					co::InterfaceType* serviceType_ = args[++argIndex].get< co::InterfaceType* >();
					const std::string& componentName_ = args[++argIndex].get< const std::string& >();
					argIndex = -1;
					p->addServiceImplementation( serviceType_, componentName_ );
				}
				break;
			case 4:
				{
					co::InterfaceType* serviceType_ = args[++argIndex].get< co::InterfaceType* >();
					co::InterfaceType* clientType_ = args[++argIndex].get< co::InterfaceType* >();
					const std::string& componentName_ = args[++argIndex].get< const std::string& >();
					argIndex = -1;
					p->addServiceImplementationForType( serviceType_, clientType_, componentName_ );
				}
				break;
			case 5:
				{
					co::InterfaceType* serviceType_ = args[++argIndex].get< co::InterfaceType* >();
					argIndex = -1;
					res.set< co::Interface* >( p->getService( serviceType_ ) );
				}
				break;
			case 6:
				{
					co::InterfaceType* serviceType_ = args[++argIndex].get< co::InterfaceType* >();
					co::Interface* clientInstance_ = args[++argIndex].get< co::Interface* >();
					argIndex = -1;
					res.set< co::Interface* >( p->getServiceForInstance( serviceType_, clientInstance_ ) );
				}
				break;
			case 7:
				{
					co::InterfaceType* serviceType_ = args[++argIndex].get< co::InterfaceType* >();
					co::InterfaceType* clientType_ = args[++argIndex].get< co::InterfaceType* >();
					argIndex = -1;
					res.set< co::Interface* >( p->getServiceForType( serviceType_, clientType_ ) );
				}
				break;
			case 8:
				{
					co::InterfaceType* serviceType_ = args[++argIndex].get< co::InterfaceType* >();
					argIndex = -1;
					p->removeService( serviceType_ );
				}
				break;
			case 9:
				{
					co::InterfaceType* serviceType_ = args[++argIndex].get< co::InterfaceType* >();
					co::InterfaceType* clientType_ = args[++argIndex].get< co::InterfaceType* >();
					argIndex = -1;
					p->removeServiceForType( serviceType_, clientType_ );
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
	co::ServiceManager* checkInstance( const co::Any& any, co::MemberInfo* member )
	{
		if( !member )
			throw co::IllegalArgumentException( "illegal null member info" );

		// make sure that 'any' is an instance of this type
		co::InterfaceType* myType = co::typeOf<co::ServiceManager>::get();

		co::ServiceManager* res;
		if( any.getKind() != co::TK_INTERFACE || !( res = dynamic_cast<co::ServiceManager*>( any.getState().data.itf ) ) )
			CORAL_THROW( co::IllegalArgumentException, "expected a valid co::ServiceManager*, but got " << any );

		// make sure that 'member' belongs to this type
		co::CompoundType* owner = member->getOwner();
		if( owner != myType )
			CORAL_THROW( co::IllegalArgumentException, "member '" << member->getName() << "' belongs to "
				<< owner->getFullName() << ", not to co.ServiceManager" );

		return res;
	}
};

// ------ Reflector Creation Function ------ //

co::Reflector* __createServiceManagerReflector()
{
    return new ServiceManager_Reflector;
}

} // namespace co
