/*
 * Coral - Lightweight C++ Component Framework.
 * See Copyright Notice in Coral.h
 */

#include <co/ModulePartLoader.h>
#include <co/DynamicProxyHandler.h>
#include <co/ModulePart.h>
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

class ModulePartLoader_Proxy : public co::ModulePartLoader
{
public:
	ModulePartLoader_Proxy( co::DynamicProxyHandler* handler ) : _handler( handler )
	{
		_cookie = _handler->registerProxyInterface( co::disambiguate<co::Interface, co::ModulePartLoader>( this ) );
	}

	virtual ~ModulePartLoader_Proxy()
	{
		// empty
	}

	// co::Interface Methods:

	co::InterfaceType* getInterfaceType() { return co::typeOf<co::ModulePartLoader>::get(); }
	co::Component* getInterfaceOwner() { return _handler->getInterfaceOwner(); }
	const std::string& getInterfaceName() { return _handler->getProxyInterfaceName( _cookie ); }
	void componentRetain() { _handler->componentRetain(); }
	void componentRelease() { _handler->componentRelease(); }

	// co.ModulePartLoader Methods:

	bool canLoadModulePart( const std::string& moduleName_ )
	{
		co::Any args[1];
		args[0].set< const std::string& >( moduleName_ );
		co::ArrayRange<co::Any const> range( args, 1 );
		const co::Any& res = _handler->handleMethodInvocation( _cookie, getMethodInfo<co::ModulePartLoader>( 0 ), range );
		return res.get< bool >();
	}

	co::ModulePart* loadModulePart( const std::string& moduleName_ )
	{
		co::Any args[1];
		args[0].set< const std::string& >( moduleName_ );
		co::ArrayRange<co::Any const> range( args, 1 );
		const co::Any& res = _handler->handleMethodInvocation( _cookie, getMethodInfo<co::ModulePartLoader>( 1 ), range );
		return res.get< co::ModulePart* >();
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

class ModulePartLoader_Reflector : public co::ReflectorBase
{
public:
	ModulePartLoader_Reflector()
	{
		// empty
	}

	virtual ~ModulePartLoader_Reflector()
	{
		// empty
	}

	co::Type* getType()
	{
		return co::typeOf<co::ModulePartLoader>::get();
	}

	co::int32 getSize()
	{
		return sizeof(co::ModulePartLoader);
	}

	co::Interface* newProxy( co::DynamicProxyHandler* handler )
	{
		checValidProxyHandler( handler );
		return co::disambiguate<co::Interface, co::ModulePartLoader>( new co::ModulePartLoader_Proxy( handler ) );
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
		co::ModulePartLoader* p = checkInstance( instance, mi );
		checkNumArguments( mi, args.getSize() );
		int argIndex = -1;
		try
		{
			switch( mi->getIndex() )
			{
			case 0:
				{
					const std::string& moduleName_ = args[++argIndex].get< const std::string& >();
					argIndex = -1;
					res.set< bool >( p->canLoadModulePart( moduleName_ ) );
				}
				break;
			case 1:
				{
					const std::string& moduleName_ = args[++argIndex].get< const std::string& >();
					argIndex = -1;
					res.set< co::ModulePart* >( p->loadModulePart( moduleName_ ) );
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
	co::ModulePartLoader* checkInstance( const co::Any& any, co::MemberInfo* member )
	{
		if( !member )
			throw co::IllegalArgumentException( "illegal null member info" );

		// make sure that 'any' is an instance of this type
		co::InterfaceType* myType = co::typeOf<co::ModulePartLoader>::get();

		co::ModulePartLoader* res;
		if( any.getKind() != co::TK_INTERFACE || !( res = dynamic_cast<co::ModulePartLoader*>( any.getState().data.itf ) ) )
			CORAL_THROW( co::IllegalArgumentException, "expected a valid co::ModulePartLoader*, but got " << any );

		// make sure that 'member' belongs to this type
		co::CompoundType* owner = member->getOwner();
		if( owner != myType )
			CORAL_THROW( co::IllegalArgumentException, "member '" << member->getName() << "' belongs to "
				<< owner->getFullName() << ", not to co.ModulePartLoader" );

		return res;
	}
};

// ------ Reflector Creation Function ------ //

co::Reflector* __createModulePartLoaderReflector()
{
    return new ModulePartLoader_Reflector;
}

} // namespace co
