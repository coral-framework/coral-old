/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#include <co/IModuleManager.h>
#include <co/IDynamicProxyHandler.h>
#include <co/IModule.h>
#include <co/IModulePartLoader.h>
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

class IModuleManager_Proxy : public co::IModuleManager
{
public:
	IModuleManager_Proxy( co::IDynamicProxyHandler* handler ) : _handler( handler )
	{
		_cookie = _handler->registerProxyInterface( co::disambiguate<co::Interface, co::IModuleManager>( this ) );
	}

	virtual ~IModuleManager_Proxy()
	{
		// empty
	}

	// co::Interface Methods:

	co::IInterfaceType* getInterfaceType() { return co::typeOf<co::IModuleManager>::get(); }
	co::IComponent* getInterfaceOwner() { return _handler->getInterfaceOwner(); }
	const std::string& getInterfaceName() { return _handler->getProxyInterfaceName( _cookie ); }
	void componentRetain() { _handler->componentRetain(); }
	void componentRelease() { _handler->componentRelease(); }

	// co.IModuleManager Methods:

	bool getBinaryCompatibilityChecking()
	{
		const co::Any& res = _handler->handleGetAttribute( _cookie, getAttribInfo<co::IModuleManager>( 0 ) );
        return res.get< bool >();
	}

	void setBinaryCompatibilityChecking( bool binaryCompatibilityChecking_ )
	{
		co::Any arg;
		arg.set< bool >( binaryCompatibilityChecking_ );
		_handler->handleSetAttribute( _cookie, getAttribInfo<co::IModuleManager>( 0 ), arg );
	}

	co::ArrayRange<co::IModulePartLoader* const> getLoaders()
	{
		const co::Any& res = _handler->handleGetAttribute( _cookie, getAttribInfo<co::IModuleManager>( 1 ) );
        return res.get< co::ArrayRange<co::IModulePartLoader* const> >();
	}

	co::ArrayRange<co::IModule* const> getModules()
	{
		const co::Any& res = _handler->handleGetAttribute( _cookie, getAttribInfo<co::IModuleManager>( 2 ) );
        return res.get< co::ArrayRange<co::IModule* const> >();
	}

	co::IModule* findModule( const std::string& moduleName_ )
	{
		co::Any args[1];
		args[0].set< const std::string& >( moduleName_ );
		co::ArrayRange<co::Any const> range( args, 1 );
		const co::Any& res = _handler->handleMethodInvocation( _cookie, getMethodInfo<co::IModuleManager>( 0 ), range );
		return res.get< co::IModule* >();
	}

	void installLoader( co::IModulePartLoader* loader_ )
	{
		co::Any args[1];
		args[0].set< co::IModulePartLoader* >( loader_ );
		co::ArrayRange<co::Any const> range( args, 1 );
		_handler->handleMethodInvocation( _cookie, getMethodInfo<co::IModuleManager>( 1 ), range );
	}

	bool isLoadable( const std::string& moduleName_ )
	{
		co::Any args[1];
		args[0].set< const std::string& >( moduleName_ );
		co::ArrayRange<co::Any const> range( args, 1 );
		const co::Any& res = _handler->handleMethodInvocation( _cookie, getMethodInfo<co::IModuleManager>( 2 ), range );
		return res.get< bool >();
	}

	co::IModule* load( const std::string& moduleName_ )
	{
		co::Any args[1];
		args[0].set< const std::string& >( moduleName_ );
		co::ArrayRange<co::Any const> range( args, 1 );
		const co::Any& res = _handler->handleMethodInvocation( _cookie, getMethodInfo<co::IModuleManager>( 3 ), range );
		return res.get< co::IModule* >();
	}

	void uninstallLoader( co::IModulePartLoader* loader_ )
	{
		co::Any args[1];
		args[0].set< co::IModulePartLoader* >( loader_ );
		co::ArrayRange<co::Any const> range( args, 1 );
		_handler->handleMethodInvocation( _cookie, getMethodInfo<co::IModuleManager>( 4 ), range );
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

class IModuleManager_Reflector : public co::ReflectorBase
{
public:
	IModuleManager_Reflector()
	{
		// empty
	}

	virtual ~IModuleManager_Reflector()
	{
		// empty
	}

	co::IType* getType()
	{
		return co::typeOf<co::IModuleManager>::get();
	}

	co::int32 getSize()
	{
		return sizeof(co::IModuleManager);
	}

	co::Interface* newProxy( co::IDynamicProxyHandler* handler )
	{
		checValidProxyHandler( handler );
		return co::disambiguate<co::Interface, co::IModuleManager>( new co::IModuleManager_Proxy( handler ) );
	}

	void getAttribute( const co::Any& instance, co::IAttributeInfo* ai, co::Any& value )
	{
		co::IModuleManager* p = checkInstance( instance, ai );
		switch( ai->getIndex() )
		{
		case 0:		value.set< bool >( p->getBinaryCompatibilityChecking() ); break;
		case 1:		value.set< co::ArrayRange<co::IModulePartLoader* const> >( p->getLoaders() ); break;
		case 2:		value.set< co::ArrayRange<co::IModule* const> >( p->getModules() ); break;
		default:	raiseUnexpectedMemberIndex();
		}
	}

	void setAttribute( const co::Any& instance, co::IAttributeInfo* ai, const co::Any& value )
	{
		co::IModuleManager* p = checkInstance( instance, ai );
		switch( ai->getIndex() )
		{
		case 0:		p->setBinaryCompatibilityChecking( value.get< bool >() ); break;
		case 1:		raiseAttributeIsReadOnly( ai ); break;
		case 2:		raiseAttributeIsReadOnly( ai ); break;
		default:	raiseUnexpectedMemberIndex();
		}
		CORAL_UNUSED( p );
		CORAL_UNUSED( value );
	}

	void invokeMethod( const co::Any& instance, co::IMethodInfo* mi, co::ArrayRange<co::Any const> args, co::Any& res )
	{
		co::IModuleManager* p = checkInstance( instance, mi );
		checkNumArguments( mi, args.getSize() );
		int argIndex = -1;
		try
		{
			switch( mi->getIndex() )
			{
			case 3:
				{
					const std::string& moduleName_ = args[++argIndex].get< const std::string& >();
					argIndex = -1;
					res.set< co::IModule* >( p->findModule( moduleName_ ) );
				}
				break;
			case 4:
				{
					co::IModulePartLoader* loader_ = args[++argIndex].get< co::IModulePartLoader* >();
					argIndex = -1;
					p->installLoader( loader_ );
				}
				break;
			case 5:
				{
					const std::string& moduleName_ = args[++argIndex].get< const std::string& >();
					argIndex = -1;
					res.set< bool >( p->isLoadable( moduleName_ ) );
				}
				break;
			case 6:
				{
					const std::string& moduleName_ = args[++argIndex].get< const std::string& >();
					argIndex = -1;
					res.set< co::IModule* >( p->load( moduleName_ ) );
				}
				break;
			case 7:
				{
					co::IModulePartLoader* loader_ = args[++argIndex].get< co::IModulePartLoader* >();
					argIndex = -1;
					p->uninstallLoader( loader_ );
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
	co::IModuleManager* checkInstance( const co::Any& any, co::IMemberInfo* member )
	{
		if( !member )
			throw co::IllegalArgumentException( "illegal null member info" );

		// make sure that 'any' is an instance of this type
		co::IInterfaceType* myType = co::typeOf<co::IModuleManager>::get();

		co::IModuleManager* res;
		if( any.getKind() != co::TK_INTERFACE || !( res = dynamic_cast<co::IModuleManager*>( any.getState().data.itf ) ) )
			CORAL_THROW( co::IllegalArgumentException, "expected a valid co::IModuleManager*, but got " << any );

		// make sure that 'member' belongs to this type
		co::ICompoundType* owner = member->getOwner();
		if( owner != myType )
			CORAL_THROW( co::IllegalArgumentException, "member '" << member->getName() << "' belongs to "
				<< owner->getFullName() << ", not to co.IModuleManager" );

		return res;
	}
};

// ------ IReflector Creation Function ------ //

co::IReflector* __createIModuleManagerIReflector()
{
    return new IModuleManager_Reflector;
}

} // namespace co
