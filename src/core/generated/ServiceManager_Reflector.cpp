/*******************************************************************************
** Reflection code generated for type 'co.ServiceManager'
**
** Created: Fri Jul 02 00:14:15 2010
**      by: Coral Compiler version 0.1.0
**
** WARNING! All changes made in this file will be lost when recompiling!
********************************************************************************/

#include <co/ServiceManager.h>
#include <co/reserved/ReflectorBase.h>
#include <co/DynamicProxyHandler.h>
#include <co/InterfaceType.h>
#include <co/MethodInfo.h>
#include <co/AttributeInfo.h>
#include <co/IllegalCastException.h>
#include <co/MissingInputException.h>
#include <co/IllegalArgumentException.h>
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

	// co::ServiceManager Methods:

	bool getIsLazy()
	{
		co::Any __res;
		_handler->handleGetAttribute( _cookie, getAttribInfo<co::ServiceManager>( 0 ), __res );
        return __res.get< bool >();
	}

	void setIsLazy( bool isLazy )
	{
		co::Any __arg;
		__arg.set< bool >( isLazy );
		_handler->handleSetAttribute( _cookie, getAttribInfo<co::ServiceManager>( 0 ), __arg );
	}

	void addService( co::InterfaceType* serviceType, co::Interface* serviceInstance )
	{
		co::Any __res, __arg[2];
		__arg[0].set< co::InterfaceType* >( serviceType );
		__arg[1].set< co::Interface* >( serviceInstance );
		co::ArrayRange<co::Any const> __ar( __arg, 2 );
		_handler->handleMethodInvocation( _cookie, getMethodInfo<co::ServiceManager>( 1 ), __ar, __res );
	}

	void addServiceForType( co::InterfaceType* serviceType, co::InterfaceType* clientType, co::Interface* serviceInstance )
	{
		co::Any __res, __arg[3];
		__arg[0].set< co::InterfaceType* >( serviceType );
		__arg[1].set< co::InterfaceType* >( clientType );
		__arg[2].set< co::Interface* >( serviceInstance );
		co::ArrayRange<co::Any const> __ar( __arg, 3 );
		_handler->handleMethodInvocation( _cookie, getMethodInfo<co::ServiceManager>( 2 ), __ar, __res );
	}

	void addServiceImplementation( co::InterfaceType* serviceType, const std::string& componentName )
	{
		co::Any __res, __arg[2];
		__arg[0].set< co::InterfaceType* >( serviceType );
		__arg[1].set< const std::string& >( componentName );
		co::ArrayRange<co::Any const> __ar( __arg, 2 );
		_handler->handleMethodInvocation( _cookie, getMethodInfo<co::ServiceManager>( 3 ), __ar, __res );
	}

	void addServiceImplementationForType( co::InterfaceType* serviceType, co::InterfaceType* clientType, const std::string& componentName )
	{
		co::Any __res, __arg[3];
		__arg[0].set< co::InterfaceType* >( serviceType );
		__arg[1].set< co::InterfaceType* >( clientType );
		__arg[2].set< const std::string& >( componentName );
		co::ArrayRange<co::Any const> __ar( __arg, 3 );
		_handler->handleMethodInvocation( _cookie, getMethodInfo<co::ServiceManager>( 4 ), __ar, __res );
	}

	co::Interface* getService( co::InterfaceType* serviceType )
	{
		co::Any __res, __arg[1];
		__arg[0].set< co::InterfaceType* >( serviceType );
		co::ArrayRange<co::Any const> __ar( __arg, 1 );
		_handler->handleMethodInvocation( _cookie, getMethodInfo<co::ServiceManager>( 5 ), __ar, __res );
		return __res.get< co::Interface* >();
	}

	co::Interface* getServiceForInstance( co::InterfaceType* serviceType, co::Interface* clientInstance )
	{
		co::Any __res, __arg[2];
		__arg[0].set< co::InterfaceType* >( serviceType );
		__arg[1].set< co::Interface* >( clientInstance );
		co::ArrayRange<co::Any const> __ar( __arg, 2 );
		_handler->handleMethodInvocation( _cookie, getMethodInfo<co::ServiceManager>( 6 ), __ar, __res );
		return __res.get< co::Interface* >();
	}

	co::Interface* getServiceForType( co::InterfaceType* serviceType, co::InterfaceType* clientType )
	{
		co::Any __res, __arg[2];
		__arg[0].set< co::InterfaceType* >( serviceType );
		__arg[1].set< co::InterfaceType* >( clientType );
		co::ArrayRange<co::Any const> __ar( __arg, 2 );
		_handler->handleMethodInvocation( _cookie, getMethodInfo<co::ServiceManager>( 7 ), __ar, __res );
		return __res.get< co::Interface* >();
	}

	void removeService( co::InterfaceType* serviceType )
	{
		co::Any __res, __arg[1];
		__arg[0].set< co::InterfaceType* >( serviceType );
		co::ArrayRange<co::Any const> __ar( __arg, 1 );
		_handler->handleMethodInvocation( _cookie, getMethodInfo<co::ServiceManager>( 8 ), __ar, __res );
	}

	void removeServiceForType( co::InterfaceType* serviceType, co::InterfaceType* clientType )
	{
		co::Any __res, __arg[2];
		__arg[0].set< co::InterfaceType* >( serviceType );
		__arg[1].set< co::InterfaceType* >( clientType );
		co::ArrayRange<co::Any const> __ar( __arg, 2 );
		_handler->handleMethodInvocation( _cookie, getMethodInfo<co::ServiceManager>( 9 ), __ar, __res );
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

	void invokeMethod( const co::Any& instance, co::MethodInfo* mi, co::ArrayRange<co::Any const> args, co::Any& __res )
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
					co::InterfaceType* serviceType = args[++argIndex].get< co::InterfaceType* >();
					co::Interface* serviceInstance = args[++argIndex].get< co::Interface* >();
					argIndex = -1;
					p->addService( serviceType, serviceInstance );
				}
				break;
			case 2:
				{
					co::InterfaceType* serviceType = args[++argIndex].get< co::InterfaceType* >();
					co::InterfaceType* clientType = args[++argIndex].get< co::InterfaceType* >();
					co::Interface* serviceInstance = args[++argIndex].get< co::Interface* >();
					argIndex = -1;
					p->addServiceForType( serviceType, clientType, serviceInstance );
				}
				break;
			case 3:
				{
					co::InterfaceType* serviceType = args[++argIndex].get< co::InterfaceType* >();
					const std::string& componentName = args[++argIndex].get< const std::string& >();
					argIndex = -1;
					p->addServiceImplementation( serviceType, componentName );
				}
				break;
			case 4:
				{
					co::InterfaceType* serviceType = args[++argIndex].get< co::InterfaceType* >();
					co::InterfaceType* clientType = args[++argIndex].get< co::InterfaceType* >();
					const std::string& componentName = args[++argIndex].get< const std::string& >();
					argIndex = -1;
					p->addServiceImplementationForType( serviceType, clientType, componentName );
				}
				break;
			case 5:
				{
					co::InterfaceType* serviceType = args[++argIndex].get< co::InterfaceType* >();
					argIndex = -1;
					__res.set< co::Interface* >( p->getService( serviceType ) );
				}
				break;
			case 6:
				{
					co::InterfaceType* serviceType = args[++argIndex].get< co::InterfaceType* >();
					co::Interface* clientInstance = args[++argIndex].get< co::Interface* >();
					argIndex = -1;
					__res.set< co::Interface* >( p->getServiceForInstance( serviceType, clientInstance ) );
				}
				break;
			case 7:
				{
					co::InterfaceType* serviceType = args[++argIndex].get< co::InterfaceType* >();
					co::InterfaceType* clientType = args[++argIndex].get< co::InterfaceType* >();
					argIndex = -1;
					__res.set< co::Interface* >( p->getServiceForType( serviceType, clientType ) );
				}
				break;
			case 8:
				{
					co::InterfaceType* serviceType = args[++argIndex].get< co::InterfaceType* >();
					argIndex = -1;
					p->removeService( serviceType );
				}
				break;
			case 9:
				{
					co::InterfaceType* serviceType = args[++argIndex].get< co::InterfaceType* >();
					co::InterfaceType* clientType = args[++argIndex].get< co::InterfaceType* >();
					argIndex = -1;
					p->removeServiceForType( serviceType, clientType );
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
	co::ServiceManager* checkInstance( const co::Any& instance, co::MemberInfo* member )
	{
		if( !member )
			throw co::IllegalArgumentException( "illegal null member info" );

		co::InterfaceType* myType = co::typeOf<co::ServiceManager>::get();

		// make sure that 'instance' is an instance of this type
		if( instance.getKind() != co::TK_INTERFACE ||
			!instance.getInterfaceType()->isSubTypeOf( myType ) ||
			instance.getState().data.ptr == NULL )
			CORAL_THROW( co::IllegalArgumentException, "expected a valid co::ServiceManager*, but got " << instance );

		// make sure that 'member' belongs to this type
		co::CompoundType* owner = member->getOwner();
		if( owner != myType )
			CORAL_THROW( co::IllegalArgumentException, "member '" << member->getName() << "' belongs to "
				<< owner->getFullName() << ", not to " << myType->getFullName() );

		return reinterpret_cast<co::ServiceManager*>( instance.getState().data.ptr );
	}
};

// ------ Reflector Creation Function ------ //

co::Reflector* __createServiceManagerReflector()
{
    return new ServiceManager_Reflector;
}

} // namespace co
