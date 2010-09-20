/*
 * Coral - A C++ Component Framework.
 * See Copyright Notice in Coral.h
 */

#include <co/ModuleManager.h>
#include <co/DynamicProxyHandler.h>
#include <co/ModulePartLoader.h>
#include <co/Module.h>
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

class ModuleManager_Proxy : public co::ModuleManager
{
public:
	ModuleManager_Proxy( co::DynamicProxyHandler* handler ) : _handler( handler )
	{
		_cookie = _handler->registerProxyInterface( co::disambiguate<co::Interface, co::ModuleManager>( this ) );
	}

	virtual ~ModuleManager_Proxy()
	{
		// empty
	}

	// co::Interface Methods:

	co::InterfaceType* getInterfaceType() { return co::typeOf<co::ModuleManager>::get(); }
	co::Component* getInterfaceOwner() { return _handler->getInterfaceOwner(); }
	const std::string& getInterfaceName() { return _handler->getProxyInterfaceName( _cookie ); }
	void componentRetain() { _handler->componentRetain(); }
	void componentRelease() { _handler->componentRelease(); }

	// co.ModuleManager Methods:

	co::ArrayRange<co::ModulePartLoader* const> getLoaders()
	{
		co::Any res;
		_handler->handleGetAttribute( _cookie, getAttribInfo<co::ModuleManager>( 0 ), res );
		assert( res.containsObject() == false );
        return res.get< co::ArrayRange<co::ModulePartLoader* const> >();
	}

	co::ArrayRange<co::Module* const> getModules()
	{
		co::Any res;
		_handler->handleGetAttribute( _cookie, getAttribInfo<co::ModuleManager>( 1 ), res );
		assert( res.containsObject() == false );
        return res.get< co::ArrayRange<co::Module* const> >();
	}

	co::Module* findModule( const std::string& __moduleName )
	{
		co::Any res, args[1];
		args[0].set< const std::string& >( __moduleName );
		co::ArrayRange<co::Any const> range( args, 1 );
		_handler->handleMethodInvocation( _cookie, getMethodInfo<co::ModuleManager>( 2 ), range, res );
		assert( res.containsObject() == false );
		return res.get< co::Module* >();
	}

	void installLoader( co::ModulePartLoader* __loader )
	{
		co::Any res, args[1];
		args[0].set< co::ModulePartLoader* >( __loader );
		co::ArrayRange<co::Any const> range( args, 1 );
		_handler->handleMethodInvocation( _cookie, getMethodInfo<co::ModuleManager>( 3 ), range, res );
	}

	bool isLoadable( const std::string& __moduleName )
	{
		co::Any res, args[1];
		args[0].set< const std::string& >( __moduleName );
		co::ArrayRange<co::Any const> range( args, 1 );
		_handler->handleMethodInvocation( _cookie, getMethodInfo<co::ModuleManager>( 4 ), range, res );
		assert( res.containsObject() == false );
		return res.get< bool >();
	}

	co::Module* load( const std::string& __moduleName )
	{
		co::Any res, args[1];
		args[0].set< const std::string& >( __moduleName );
		co::ArrayRange<co::Any const> range( args, 1 );
		_handler->handleMethodInvocation( _cookie, getMethodInfo<co::ModuleManager>( 5 ), range, res );
		assert( res.containsObject() == false );
		return res.get< co::Module* >();
	}

	void uninstallLoader( co::ModulePartLoader* __loader )
	{
		co::Any res, args[1];
		args[0].set< co::ModulePartLoader* >( __loader );
		co::ArrayRange<co::Any const> range( args, 1 );
		_handler->handleMethodInvocation( _cookie, getMethodInfo<co::ModuleManager>( 6 ), range, res );
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

class ModuleManager_Reflector : public co::ReflectorBase
{
public:
	ModuleManager_Reflector()
	{
		// empty
	}

	virtual ~ModuleManager_Reflector()
	{
		// empty
	}

	co::Type* getType()
	{
		return co::typeOf<co::ModuleManager>::get();
	}

	co::int32 getSize()
	{
		return sizeof(co::ModuleManager);
	}

	co::Interface* newProxy( co::DynamicProxyHandler* handler )
	{
		checValidProxyHandler( handler );
		return co::disambiguate<co::Interface, co::ModuleManager>( new co::ModuleManager_Proxy( handler ) );
	}

	void getAttribute( const co::Any& instance, co::AttributeInfo* ai, co::Any& value )
	{
		co::ModuleManager* p = checkInstance( instance, ai );
		switch( ai->getIndex() )
		{
		case 0:		value.set< co::ArrayRange<co::ModulePartLoader* const> >( p->getLoaders() ); break;
		case 1:		value.set< co::ArrayRange<co::Module* const> >( p->getModules() ); break;
		default:	raiseUnexpectedMemberIndex();
		}
	}

	void setAttribute( const co::Any& instance, co::AttributeInfo* ai, const co::Any& value )
	{
		co::ModuleManager* p = checkInstance( instance, ai );
		switch( ai->getIndex() )
		{
		case 0:		raiseAttributeIsReadOnly( ai ); break;
		case 1:		raiseAttributeIsReadOnly( ai ); break;
		default:	raiseUnexpectedMemberIndex();
		}
		CORAL_UNUSED( p );
		CORAL_UNUSED( value );
	}

	void invokeMethod( const co::Any& instance, co::MethodInfo* mi, co::ArrayRange<co::Any const> args, co::Any& res )
	{
		co::ModuleManager* p = checkInstance( instance, mi );
		checkNumArguments( mi, args.getSize() );
		int argIndex = -1;
		try
		{
			switch( mi->getIndex() )
			{
			case 2:
				{
					const std::string& __moduleName = args[++argIndex].get< const std::string& >();
					argIndex = -1;
					res.set< co::Module* >( p->findModule( __moduleName ) );
				}
				break;
			case 3:
				{
					co::ModulePartLoader* __loader = args[++argIndex].get< co::ModulePartLoader* >();
					argIndex = -1;
					p->installLoader( __loader );
				}
				break;
			case 4:
				{
					const std::string& __moduleName = args[++argIndex].get< const std::string& >();
					argIndex = -1;
					res.set< bool >( p->isLoadable( __moduleName ) );
				}
				break;
			case 5:
				{
					const std::string& __moduleName = args[++argIndex].get< const std::string& >();
					argIndex = -1;
					res.set< co::Module* >( p->load( __moduleName ) );
				}
				break;
			case 6:
				{
					co::ModulePartLoader* __loader = args[++argIndex].get< co::ModulePartLoader* >();
					argIndex = -1;
					p->uninstallLoader( __loader );
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
	co::ModuleManager* checkInstance( const co::Any& instance, co::MemberInfo* member )
	{
		if( !member )
			throw co::IllegalArgumentException( "illegal null member info" );

		co::InterfaceType* myType = co::typeOf<co::ModuleManager>::get();

		// make sure that 'instance' is an instance of this type
		if( instance.getKind() != co::TK_INTERFACE ||
			!instance.getInterfaceType()->isSubTypeOf( myType ) ||
			instance.getState().data.ptr == NULL )
			CORAL_THROW( co::IllegalArgumentException, "expected a valid co::ModuleManager*, but got " << instance );

		// make sure that 'member' belongs to this type
		co::CompoundType* owner = member->getOwner();
		if( owner != myType )
			CORAL_THROW( co::IllegalArgumentException, "member '" << member->getName() << "' belongs to "
				<< owner->getFullName() << ", not to " << myType->getFullName() );

		return dynamic_cast<co::ModuleManager*>( instance.getState().data.itf );
	}
};

// ------ Reflector Creation Function ------ //

co::Reflector* __createModuleManagerReflector()
{
    return new ModuleManager_Reflector;
}

} // namespace co
