/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#include <co/IServiceManager.h>
#include <co/IDynamicProxyHandler.h>
#include <co/IInterfaceType.h>
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

class IServiceManager_Proxy : public co::IServiceManager
{
public:
	IServiceManager_Proxy( co::IDynamicProxyHandler* handler ) : _handler( handler )
	{
		_cookie = _handler->registerProxyInterface( co::disambiguate<co::Interface, co::IServiceManager>( this ) );
	}

	virtual ~IServiceManager_Proxy()
	{
		// empty
	}

	// co::Interface Methods:

	co::IInterfaceType* getInterfaceType() { return co::typeOf<co::IServiceManager>::get(); }
	co::IComponent* getInterfaceOwner() { return _handler->getInterfaceOwner(); }
	const std::string& getInterfaceName() { return _handler->getProxyInterfaceName( _cookie ); }
	void componentRetain() { _handler->componentRetain(); }
	void componentRelease() { _handler->componentRelease(); }

	// co.IServiceManager Methods:

	bool getIsLazy()
	{
		const co::Any& res = _handler->handleGetAttribute( _cookie, getAttribInfo<co::IServiceManager>( 0 ) );
        return res.get< bool >();
	}

	void setIsLazy( bool isLazy_ )
	{
		co::Any arg;
		arg.set< bool >( isLazy_ );
		_handler->handleSetAttribute( _cookie, getAttribInfo<co::IServiceManager>( 0 ), arg );
	}

	void addService( co::IInterfaceType* serviceType_, co::Interface* serviceInstance_ )
	{
		co::Any args[2];
		args[0].set< co::IInterfaceType* >( serviceType_ );
		args[1].set< co::Interface* >( serviceInstance_ );
		co::ArrayRange<co::Any const> range( args, 2 );
		_handler->handleMethodInvocation( _cookie, getMethodInfo<co::IServiceManager>( 0 ), range );
	}

	void addServiceForType( co::IInterfaceType* serviceType_, co::IInterfaceType* clientType_, co::Interface* serviceInstance_ )
	{
		co::Any args[3];
		args[0].set< co::IInterfaceType* >( serviceType_ );
		args[1].set< co::IInterfaceType* >( clientType_ );
		args[2].set< co::Interface* >( serviceInstance_ );
		co::ArrayRange<co::Any const> range( args, 3 );
		_handler->handleMethodInvocation( _cookie, getMethodInfo<co::IServiceManager>( 1 ), range );
	}

	void addServiceImplementation( co::IInterfaceType* serviceType_, const std::string& componentName_ )
	{
		co::Any args[2];
		args[0].set< co::IInterfaceType* >( serviceType_ );
		args[1].set< const std::string& >( componentName_ );
		co::ArrayRange<co::Any const> range( args, 2 );
		_handler->handleMethodInvocation( _cookie, getMethodInfo<co::IServiceManager>( 2 ), range );
	}

	void addServiceImplementationForType( co::IInterfaceType* serviceType_, co::IInterfaceType* clientType_, const std::string& componentName_ )
	{
		co::Any args[3];
		args[0].set< co::IInterfaceType* >( serviceType_ );
		args[1].set< co::IInterfaceType* >( clientType_ );
		args[2].set< const std::string& >( componentName_ );
		co::ArrayRange<co::Any const> range( args, 3 );
		_handler->handleMethodInvocation( _cookie, getMethodInfo<co::IServiceManager>( 3 ), range );
	}

	co::Interface* getService( co::IInterfaceType* serviceType_ )
	{
		co::Any args[1];
		args[0].set< co::IInterfaceType* >( serviceType_ );
		co::ArrayRange<co::Any const> range( args, 1 );
		const co::Any& res = _handler->handleMethodInvocation( _cookie, getMethodInfo<co::IServiceManager>( 4 ), range );
		return res.get< co::Interface* >();
	}

	co::Interface* getServiceForInstance( co::IInterfaceType* serviceType_, co::Interface* clientInstance_ )
	{
		co::Any args[2];
		args[0].set< co::IInterfaceType* >( serviceType_ );
		args[1].set< co::Interface* >( clientInstance_ );
		co::ArrayRange<co::Any const> range( args, 2 );
		const co::Any& res = _handler->handleMethodInvocation( _cookie, getMethodInfo<co::IServiceManager>( 5 ), range );
		return res.get< co::Interface* >();
	}

	co::Interface* getServiceForType( co::IInterfaceType* serviceType_, co::IInterfaceType* clientType_ )
	{
		co::Any args[2];
		args[0].set< co::IInterfaceType* >( serviceType_ );
		args[1].set< co::IInterfaceType* >( clientType_ );
		co::ArrayRange<co::Any const> range( args, 2 );
		const co::Any& res = _handler->handleMethodInvocation( _cookie, getMethodInfo<co::IServiceManager>( 6 ), range );
		return res.get< co::Interface* >();
	}

	void removeService( co::IInterfaceType* serviceType_ )
	{
		co::Any args[1];
		args[0].set< co::IInterfaceType* >( serviceType_ );
		co::ArrayRange<co::Any const> range( args, 1 );
		_handler->handleMethodInvocation( _cookie, getMethodInfo<co::IServiceManager>( 7 ), range );
	}

	void removeServiceForType( co::IInterfaceType* serviceType_, co::IInterfaceType* clientType_ )
	{
		co::Any args[2];
		args[0].set< co::IInterfaceType* >( serviceType_ );
		args[1].set< co::IInterfaceType* >( clientType_ );
		co::ArrayRange<co::Any const> range( args, 2 );
		_handler->handleMethodInvocation( _cookie, getMethodInfo<co::IServiceManager>( 8 ), range );
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

class IServiceManager_Reflector : public co::ReflectorBase
{
public:
	IServiceManager_Reflector()
	{
		// empty
	}

	virtual ~IServiceManager_Reflector()
	{
		// empty
	}

	co::IType* getType()
	{
		return co::typeOf<co::IServiceManager>::get();
	}

	co::int32 getSize()
	{
		return sizeof(co::IServiceManager);
	}

	co::Interface* newProxy( co::IDynamicProxyHandler* handler )
	{
		checValidProxyHandler( handler );
		return co::disambiguate<co::Interface, co::IServiceManager>( new co::IServiceManager_Proxy( handler ) );
	}

	void getAttribute( const co::Any& instance, co::IAttributeInfo* ai, co::Any& value )
	{
		co::IServiceManager* p = checkInstance( instance, ai );
		switch( ai->getIndex() )
		{
		case 0:		value.set< bool >( p->getIsLazy() ); break;
		default:	raiseUnexpectedMemberIndex();
		}
	}

	void setAttribute( const co::Any& instance, co::IAttributeInfo* ai, const co::Any& value )
	{
		co::IServiceManager* p = checkInstance( instance, ai );
		switch( ai->getIndex() )
		{
		case 0:		p->setIsLazy( value.get< bool >() ); break;
		default:	raiseUnexpectedMemberIndex();
		}
		CORAL_UNUSED( p );
		CORAL_UNUSED( value );
	}

	void invokeMethod( const co::Any& instance, co::IMethodInfo* mi, co::ArrayRange<co::Any const> args, co::Any& res )
	{
		co::IServiceManager* p = checkInstance( instance, mi );
		checkNumArguments( mi, args.getSize() );
		int argIndex = -1;
		try
		{
			switch( mi->getIndex() )
			{
			case 1:
				{
					co::IInterfaceType* serviceType_ = args[++argIndex].get< co::IInterfaceType* >();
					co::Interface* serviceInstance_ = args[++argIndex].get< co::Interface* >();
					argIndex = -1;
					p->addService( serviceType_, serviceInstance_ );
				}
				break;
			case 2:
				{
					co::IInterfaceType* serviceType_ = args[++argIndex].get< co::IInterfaceType* >();
					co::IInterfaceType* clientType_ = args[++argIndex].get< co::IInterfaceType* >();
					co::Interface* serviceInstance_ = args[++argIndex].get< co::Interface* >();
					argIndex = -1;
					p->addServiceForType( serviceType_, clientType_, serviceInstance_ );
				}
				break;
			case 3:
				{
					co::IInterfaceType* serviceType_ = args[++argIndex].get< co::IInterfaceType* >();
					const std::string& componentName_ = args[++argIndex].get< const std::string& >();
					argIndex = -1;
					p->addServiceImplementation( serviceType_, componentName_ );
				}
				break;
			case 4:
				{
					co::IInterfaceType* serviceType_ = args[++argIndex].get< co::IInterfaceType* >();
					co::IInterfaceType* clientType_ = args[++argIndex].get< co::IInterfaceType* >();
					const std::string& componentName_ = args[++argIndex].get< const std::string& >();
					argIndex = -1;
					p->addServiceImplementationForType( serviceType_, clientType_, componentName_ );
				}
				break;
			case 5:
				{
					co::IInterfaceType* serviceType_ = args[++argIndex].get< co::IInterfaceType* >();
					argIndex = -1;
					res.set< co::Interface* >( p->getService( serviceType_ ) );
				}
				break;
			case 6:
				{
					co::IInterfaceType* serviceType_ = args[++argIndex].get< co::IInterfaceType* >();
					co::Interface* clientInstance_ = args[++argIndex].get< co::Interface* >();
					argIndex = -1;
					res.set< co::Interface* >( p->getServiceForInstance( serviceType_, clientInstance_ ) );
				}
				break;
			case 7:
				{
					co::IInterfaceType* serviceType_ = args[++argIndex].get< co::IInterfaceType* >();
					co::IInterfaceType* clientType_ = args[++argIndex].get< co::IInterfaceType* >();
					argIndex = -1;
					res.set< co::Interface* >( p->getServiceForType( serviceType_, clientType_ ) );
				}
				break;
			case 8:
				{
					co::IInterfaceType* serviceType_ = args[++argIndex].get< co::IInterfaceType* >();
					argIndex = -1;
					p->removeService( serviceType_ );
				}
				break;
			case 9:
				{
					co::IInterfaceType* serviceType_ = args[++argIndex].get< co::IInterfaceType* >();
					co::IInterfaceType* clientType_ = args[++argIndex].get< co::IInterfaceType* >();
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
	co::IServiceManager* checkInstance( const co::Any& any, co::IMemberInfo* member )
	{
		if( !member )
			throw co::IllegalArgumentException( "illegal null member info" );

		// make sure that 'any' is an instance of this type
		co::IInterfaceType* myType = co::typeOf<co::IServiceManager>::get();

		co::IServiceManager* res;
		if( any.getKind() != co::TK_INTERFACE || !( res = dynamic_cast<co::IServiceManager*>( any.getState().data.itf ) ) )
			CORAL_THROW( co::IllegalArgumentException, "expected a valid co::IServiceManager*, but got " << any );

		// make sure that 'member' belongs to this type
		co::ICompoundType* owner = member->getOwner();
		if( owner != myType )
			CORAL_THROW( co::IllegalArgumentException, "member '" << member->getName() << "' belongs to "
				<< owner->getFullName() << ", not to co.IServiceManager" );

		return res;
	}
};

// ------ IReflector Creation Function ------ //

co::IReflector* __createIServiceManagerIReflector()
{
    return new IServiceManager_Reflector;
}

} // namespace co
