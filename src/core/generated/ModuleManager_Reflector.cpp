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

	bool getBinaryCompatibilityChecks()
	{
		const co::Any& res = _handler->handleGetAttribute( _cookie, getAttribInfo<co::ModuleManager>( 0 ) );
        return res.get< bool >();
	}

	void setBinaryCompatibilityChecks( bool binaryCompatibilityChecks_ )
	{
		co::Any arg;
		arg.set< bool >( binaryCompatibilityChecks_ );
		_handler->handleSetAttribute( _cookie, getAttribInfo<co::ModuleManager>( 0 ), arg );
	}

	co::ArrayRange<co::ModulePartLoader* const> getLoaders()
	{
		const co::Any& res = _handler->handleGetAttribute( _cookie, getAttribInfo<co::ModuleManager>( 1 ) );
        return res.get< co::ArrayRange<co::ModulePartLoader* const> >();
	}

	co::ArrayRange<co::Module* const> getModules()
	{
		const co::Any& res = _handler->handleGetAttribute( _cookie, getAttribInfo<co::ModuleManager>( 2 ) );
        return res.get< co::ArrayRange<co::Module* const> >();
	}

	co::Module* findModule( const std::string& moduleName_ )
	{
		co::Any args[1];
		args[0].set< const std::string& >( moduleName_ );
		co::ArrayRange<co::Any const> range( args, 1 );
		const co::Any& res = _handler->handleMethodInvocation( _cookie, getMethodInfo<co::ModuleManager>( 0 ), range );
		return res.get< co::Module* >();
	}

	void installLoader( co::ModulePartLoader* loader_ )
	{
		co::Any args[1];
		args[0].set< co::ModulePartLoader* >( loader_ );
		co::ArrayRange<co::Any const> range( args, 1 );
		_handler->handleMethodInvocation( _cookie, getMethodInfo<co::ModuleManager>( 1 ), range );
	}

	bool isLoadable( const std::string& moduleName_ )
	{
		co::Any args[1];
		args[0].set< const std::string& >( moduleName_ );
		co::ArrayRange<co::Any const> range( args, 1 );
		const co::Any& res = _handler->handleMethodInvocation( _cookie, getMethodInfo<co::ModuleManager>( 2 ), range );
		return res.get< bool >();
	}

	co::Module* load( const std::string& moduleName_ )
	{
		co::Any args[1];
		args[0].set< const std::string& >( moduleName_ );
		co::ArrayRange<co::Any const> range( args, 1 );
		const co::Any& res = _handler->handleMethodInvocation( _cookie, getMethodInfo<co::ModuleManager>( 3 ), range );
		return res.get< co::Module* >();
	}

	void uninstallLoader( co::ModulePartLoader* loader_ )
	{
		co::Any args[1];
		args[0].set< co::ModulePartLoader* >( loader_ );
		co::ArrayRange<co::Any const> range( args, 1 );
		_handler->handleMethodInvocation( _cookie, getMethodInfo<co::ModuleManager>( 4 ), range );
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
		case 0:		value.set< bool >( p->getBinaryCompatibilityChecks() ); break;
		case 1:		value.set< co::ArrayRange<co::ModulePartLoader* const> >( p->getLoaders() ); break;
		case 2:		value.set< co::ArrayRange<co::Module* const> >( p->getModules() ); break;
		default:	raiseUnexpectedMemberIndex();
		}
	}

	void setAttribute( const co::Any& instance, co::AttributeInfo* ai, const co::Any& value )
	{
		co::ModuleManager* p = checkInstance( instance, ai );
		switch( ai->getIndex() )
		{
		case 0:		p->setBinaryCompatibilityChecks( value.get< bool >() ); break;
		case 1:		raiseAttributeIsReadOnly( ai ); break;
		case 2:		raiseAttributeIsReadOnly( ai ); break;
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
			case 3:
				{
					const std::string& moduleName_ = args[++argIndex].get< const std::string& >();
					argIndex = -1;
					res.set< co::Module* >( p->findModule( moduleName_ ) );
				}
				break;
			case 4:
				{
					co::ModulePartLoader* loader_ = args[++argIndex].get< co::ModulePartLoader* >();
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
					res.set< co::Module* >( p->load( moduleName_ ) );
				}
				break;
			case 7:
				{
					co::ModulePartLoader* loader_ = args[++argIndex].get< co::ModulePartLoader* >();
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
	co::ModuleManager* checkInstance( const co::Any& any, co::MemberInfo* member )
	{
		if( !member )
			throw co::IllegalArgumentException( "illegal null member info" );

		// make sure that 'any' is an instance of this type
		co::InterfaceType* myType = co::typeOf<co::ModuleManager>::get();

		co::ModuleManager* res;
		if( any.getKind() != co::TK_INTERFACE || !( res = dynamic_cast<co::ModuleManager*>( any.getState().data.itf ) ) )
			CORAL_THROW( co::IllegalArgumentException, "expected a valid co::ModuleManager*, but got " << any );

		// make sure that 'member' belongs to this type
		co::CompoundType* owner = member->getOwner();
		if( owner != myType )
			CORAL_THROW( co::IllegalArgumentException, "member '" << member->getName() << "' belongs to "
				<< owner->getFullName() << ", not to co.ModuleManager" );

		return res;
	}
};

// ------ Reflector Creation Function ------ //

co::Reflector* __createModuleManagerReflector()
{
    return new ModuleManager_Reflector;
}

} // namespace co
