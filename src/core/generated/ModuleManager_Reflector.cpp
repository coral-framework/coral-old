/*******************************************************************************
** Reflection code generated for type 'co.ModuleManager'
**
** Created: Wed Aug 25 16:31:31 2010
**      by: Coral Compiler version 0.1.0
**
** WARNING! All changes made in this file will be lost when recompiling!
********************************************************************************/

#include <co/ModuleManager.h>
#include <co/reserved/ReflectorBase.h>
#include <co/DynamicProxyHandler.h>
#include <co/Module.h>
#include <co/ModulePartLoader.h>
#include <co/MethodInfo.h>
#include <co/AttributeInfo.h>
#include <co/IllegalCastException.h>
#include <co/MissingInputException.h>
#include <co/IllegalArgumentException.h>
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

	// co::ModuleManager Methods:

	co::ArrayRange<co::ModulePartLoader* const> getLoaders()
	{
		co::Any __res;
		_handler->handleGetAttribute( _cookie, getAttribInfo<co::ModuleManager>( 0 ), __res );
        return __res.get< co::ArrayRange<co::ModulePartLoader* const> >();
	}

	co::ArrayRange<co::Module* const> getModules()
	{
		co::Any __res;
		_handler->handleGetAttribute( _cookie, getAttribInfo<co::ModuleManager>( 1 ), __res );
        return __res.get< co::ArrayRange<co::Module* const> >();
	}

	co::Module* findModule( const std::string& moduleName )
	{
		co::Any __res, __arg[1];
		__arg[0].set< const std::string& >( moduleName );
		co::ArrayRange<co::Any const> __ar( __arg, 1 );
		_handler->handleMethodInvocation( _cookie, getMethodInfo<co::ModuleManager>( 2 ), __ar, __res );
		return __res.get< co::Module* >();
	}

	void installLoader( co::ModulePartLoader* loader )
	{
		co::Any __res, __arg[1];
		__arg[0].set< co::ModulePartLoader* >( loader );
		co::ArrayRange<co::Any const> __ar( __arg, 1 );
		_handler->handleMethodInvocation( _cookie, getMethodInfo<co::ModuleManager>( 3 ), __ar, __res );
	}

	bool isLoadable( const std::string& moduleName )
	{
		co::Any __res, __arg[1];
		__arg[0].set< const std::string& >( moduleName );
		co::ArrayRange<co::Any const> __ar( __arg, 1 );
		_handler->handleMethodInvocation( _cookie, getMethodInfo<co::ModuleManager>( 4 ), __ar, __res );
		return __res.get< bool >();
	}

	co::Module* load( const std::string& moduleName )
	{
		co::Any __res, __arg[1];
		__arg[0].set< const std::string& >( moduleName );
		co::ArrayRange<co::Any const> __ar( __arg, 1 );
		_handler->handleMethodInvocation( _cookie, getMethodInfo<co::ModuleManager>( 5 ), __ar, __res );
		return __res.get< co::Module* >();
	}

	void uninstallLoader( co::ModulePartLoader* loader )
	{
		co::Any __res, __arg[1];
		__arg[0].set< co::ModulePartLoader* >( loader );
		co::ArrayRange<co::Any const> __ar( __arg, 1 );
		_handler->handleMethodInvocation( _cookie, getMethodInfo<co::ModuleManager>( 6 ), __ar, __res );
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

	void invokeMethod( const co::Any& instance, co::MethodInfo* mi, co::ArrayRange<co::Any const> args, co::Any& __res )
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
					const std::string& moduleName = args[++argIndex].get< const std::string& >();
					argIndex = -1;
					__res.set< co::Module* >( p->findModule( moduleName ) );
				}
				break;
			case 3:
				{
					co::ModulePartLoader* loader = args[++argIndex].get< co::ModulePartLoader* >();
					argIndex = -1;
					p->installLoader( loader );
				}
				break;
			case 4:
				{
					const std::string& moduleName = args[++argIndex].get< const std::string& >();
					argIndex = -1;
					__res.set< bool >( p->isLoadable( moduleName ) );
				}
				break;
			case 5:
				{
					const std::string& moduleName = args[++argIndex].get< const std::string& >();
					argIndex = -1;
					__res.set< co::Module* >( p->load( moduleName ) );
				}
				break;
			case 6:
				{
					co::ModulePartLoader* loader = args[++argIndex].get< co::ModulePartLoader* >();
					argIndex = -1;
					p->uninstallLoader( loader );
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

		return reinterpret_cast<co::ModuleManager*>( instance.getState().data.ptr );
	}
};

// ------ Reflector Creation Function ------ //

co::Reflector* __createModuleManagerReflector()
{
    return new ModuleManager_Reflector;
}

} // namespace co
