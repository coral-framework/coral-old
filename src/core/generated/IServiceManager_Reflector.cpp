/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#include <co/IServiceManager.h>
#include <co/IDynamicServiceProvider.h>
#include <co/IInterface.h>
#include <co/IMethod.h>
#include <co/IField.h>
#include <co/IllegalCastException.h>
#include <co/MissingInputException.h>
#include <co/IllegalArgumentException.h>
#include <co/reserved/ReflectorBase.h>
#include <sstream>
#include <cassert>

namespace co {

// ------ Dynamic Service Proxy ------ //

class IServiceManager_Proxy : public co::IServiceManager
{
public:
	IServiceManager_Proxy( co::IDynamicServiceProvider* provider ) : _provider( provider )
	{
		_cookie = _provider->registerProxyInterface( co::disambiguate<co::IService, co::IServiceManager>( this ) );
	}

	virtual ~IServiceManager_Proxy()
	{
		// empty
	}

	// co::IService Methods:

	co::IInterface* getInterfaceType() { return co::typeOf<co::IServiceManager>::get(); }
	co::IObject* getInterfaceOwner() { return _provider->getInterfaceOwner(); }
	const std::string& getInterfaceName() { return _provider->getProxyInterfaceName( _cookie ); }
	void componentRetain() { _provider->componentRetain(); }
	void componentRelease() { _provider->componentRelease(); }

	// co.IServiceManager Methods:

	bool getIsLazy()
	{
		const co::Any& res = _provider->handleGetAttribute( _cookie, getAttribInfo<co::IServiceManager>( 0 ) );
        return res.get< bool >();
	}

	void setIsLazy( bool isLazy_ )
	{
		co::Any arg;
		arg.set< bool >( isLazy_ );
		_provider->handleSetAttribute( _cookie, getAttribInfo<co::IServiceManager>( 0 ), arg );
	}

	void addService( co::IInterface* serviceType_, co::IService* service_ )
	{
		co::Any args[2];
		args[0].set< co::IInterface* >( serviceType_ );
		args[1].set< co::IService* >( service_ );
		co::Range<co::Any const> range( args, 2 );
		_provider->handleMethodInvocation( _cookie, getMethodInfo<co::IServiceManager>( 0 ), range );
	}

	void addServiceForType( co::IInterface* serviceType_, co::IInterface* clientType_, co::IService* service_ )
	{
		co::Any args[3];
		args[0].set< co::IInterface* >( serviceType_ );
		args[1].set< co::IInterface* >( clientType_ );
		args[2].set< co::IService* >( service_ );
		co::Range<co::Any const> range( args, 3 );
		_provider->handleMethodInvocation( _cookie, getMethodInfo<co::IServiceManager>( 1 ), range );
	}

	void addServiceProvider( co::IInterface* serviceType_, const std::string& componentName_ )
	{
		co::Any args[2];
		args[0].set< co::IInterface* >( serviceType_ );
		args[1].set< const std::string& >( componentName_ );
		co::Range<co::Any const> range( args, 2 );
		_provider->handleMethodInvocation( _cookie, getMethodInfo<co::IServiceManager>( 2 ), range );
	}

	void addServiceProviderForType( co::IInterface* serviceType_, co::IInterface* clientType_, const std::string& componentName_ )
	{
		co::Any args[3];
		args[0].set< co::IInterface* >( serviceType_ );
		args[1].set< co::IInterface* >( clientType_ );
		args[2].set< const std::string& >( componentName_ );
		co::Range<co::Any const> range( args, 3 );
		_provider->handleMethodInvocation( _cookie, getMethodInfo<co::IServiceManager>( 3 ), range );
	}

	co::IService* getService( co::IInterface* serviceType_ )
	{
		co::Any args[1];
		args[0].set< co::IInterface* >( serviceType_ );
		co::Range<co::Any const> range( args, 1 );
		const co::Any& res = _provider->handleMethodInvocation( _cookie, getMethodInfo<co::IServiceManager>( 4 ), range );
		return res.get< co::IService* >();
	}

	co::IService* getServiceForInstance( co::IInterface* serviceType_, co::IService* clientInstance_ )
	{
		co::Any args[2];
		args[0].set< co::IInterface* >( serviceType_ );
		args[1].set< co::IService* >( clientInstance_ );
		co::Range<co::Any const> range( args, 2 );
		const co::Any& res = _provider->handleMethodInvocation( _cookie, getMethodInfo<co::IServiceManager>( 5 ), range );
		return res.get< co::IService* >();
	}

	co::IService* getServiceForType( co::IInterface* serviceType_, co::IInterface* clientType_ )
	{
		co::Any args[2];
		args[0].set< co::IInterface* >( serviceType_ );
		args[1].set< co::IInterface* >( clientType_ );
		co::Range<co::Any const> range( args, 2 );
		const co::Any& res = _provider->handleMethodInvocation( _cookie, getMethodInfo<co::IServiceManager>( 6 ), range );
		return res.get< co::IService* >();
	}

	void removeService( co::IInterface* serviceType_ )
	{
		co::Any args[1];
		args[0].set< co::IInterface* >( serviceType_ );
		co::Range<co::Any const> range( args, 1 );
		_provider->handleMethodInvocation( _cookie, getMethodInfo<co::IServiceManager>( 7 ), range );
	}

	void removeServiceForType( co::IInterface* serviceType_, co::IInterface* clientType_ )
	{
		co::Any args[2];
		args[0].set< co::IInterface* >( serviceType_ );
		args[1].set< co::IInterface* >( clientType_ );
		co::Range<co::Any const> range( args, 2 );
		_provider->handleMethodInvocation( _cookie, getMethodInfo<co::IServiceManager>( 8 ), range );
	}

protected:
	template<typename T>
	co::IField* getAttribInfo( co::uint32 index )
	{
		return co::typeOf<T>::get()->getFields()[index];
	}

	template<typename T>
	co::IMethod* getMethodInfo( co::uint32 index )
	{
		return co::typeOf<T>::get()->getMethods()[index];
	}

private:
	co::IDynamicServiceProvider* _provider;
	co::uint32 _cookie;
};

// ------ Reflector Component ------ //

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

	co::IService* newProxy( co::IDynamicServiceProvider* provider )
	{
		checkValidDynamicProvider( provider );
		return co::disambiguate<co::IService, co::IServiceManager>( new co::IServiceManager_Proxy( provider ) );
	}

	void getAttribute( const co::Any& instance, co::IField* ai, co::Any& value )
	{
		co::IServiceManager* p = checkInstance( instance, ai );
		switch( ai->getIndex() )
		{
		case 0:		value.set< bool >( p->getIsLazy() ); break;
		default:	raiseUnexpectedMemberIndex();
		}
	}

	void setAttribute( const co::Any& instance, co::IField* ai, const co::Any& value )
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

	void invokeMethod( const co::Any& instance, co::IMethod* mi, co::Range<co::Any const> args, co::Any& res )
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
					co::IInterface* serviceType_ = args[++argIndex].get< co::IInterface* >();
					co::IService* service_ = args[++argIndex].get< co::IService* >();
					argIndex = -1;
					p->addService( serviceType_, service_ );
				}
				break;
			case 2:
				{
					co::IInterface* serviceType_ = args[++argIndex].get< co::IInterface* >();
					co::IInterface* clientType_ = args[++argIndex].get< co::IInterface* >();
					co::IService* service_ = args[++argIndex].get< co::IService* >();
					argIndex = -1;
					p->addServiceForType( serviceType_, clientType_, service_ );
				}
				break;
			case 3:
				{
					co::IInterface* serviceType_ = args[++argIndex].get< co::IInterface* >();
					const std::string& componentName_ = args[++argIndex].get< const std::string& >();
					argIndex = -1;
					p->addServiceProvider( serviceType_, componentName_ );
				}
				break;
			case 4:
				{
					co::IInterface* serviceType_ = args[++argIndex].get< co::IInterface* >();
					co::IInterface* clientType_ = args[++argIndex].get< co::IInterface* >();
					const std::string& componentName_ = args[++argIndex].get< const std::string& >();
					argIndex = -1;
					p->addServiceProviderForType( serviceType_, clientType_, componentName_ );
				}
				break;
			case 5:
				{
					co::IInterface* serviceType_ = args[++argIndex].get< co::IInterface* >();
					argIndex = -1;
					res.set< co::IService* >( p->getService( serviceType_ ) );
				}
				break;
			case 6:
				{
					co::IInterface* serviceType_ = args[++argIndex].get< co::IInterface* >();
					co::IService* clientInstance_ = args[++argIndex].get< co::IService* >();
					argIndex = -1;
					res.set< co::IService* >( p->getServiceForInstance( serviceType_, clientInstance_ ) );
				}
				break;
			case 7:
				{
					co::IInterface* serviceType_ = args[++argIndex].get< co::IInterface* >();
					co::IInterface* clientType_ = args[++argIndex].get< co::IInterface* >();
					argIndex = -1;
					res.set< co::IService* >( p->getServiceForType( serviceType_, clientType_ ) );
				}
				break;
			case 8:
				{
					co::IInterface* serviceType_ = args[++argIndex].get< co::IInterface* >();
					argIndex = -1;
					p->removeService( serviceType_ );
				}
				break;
			case 9:
				{
					co::IInterface* serviceType_ = args[++argIndex].get< co::IInterface* >();
					co::IInterface* clientType_ = args[++argIndex].get< co::IInterface* >();
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
	co::IServiceManager* checkInstance( const co::Any& any, co::IMember* member )
	{
		if( !member )
			throw co::IllegalArgumentException( "illegal null member info" );

		// make sure that 'any' is an instance of this type
		co::IInterface* myType = co::typeOf<co::IServiceManager>::get();

		co::IServiceManager* res;
		if( any.getKind() != co::TK_INTERFACE || !( res = dynamic_cast<co::IServiceManager*>( any.getState().data.itf ) ) )
			CORAL_THROW( co::IllegalArgumentException, "expected a valid co::IServiceManager*, but got " << any );

		// make sure that 'member' belongs to this type
		co::ICompositeType* owner = member->getOwner();
		if( owner != myType )
			CORAL_THROW( co::IllegalArgumentException, "member '" << member->getName() << "' belongs to "
				<< owner->getFullName() << ", not to co.IServiceManager" );

		return res;
	}
};

// ------ Reflector Creation Function ------ //

co::IReflector* __createIServiceManagerReflector()
{
    return new IServiceManager_Reflector;
}

} // namespace co
