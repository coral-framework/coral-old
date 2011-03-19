/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#include <co/IModulePartLoader.h>
#include <co/IDynamicProxyHandler.h>
#include <co/IModulePart.h>
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

class IModulePartLoader_Proxy : public co::IModulePartLoader
{
public:
	IModulePartLoader_Proxy( co::IDynamicProxyHandler* handler ) : _handler( handler )
	{
		_cookie = _handler->registerProxyInterface( co::disambiguate<co::Interface, co::IModulePartLoader>( this ) );
	}

	virtual ~IModulePartLoader_Proxy()
	{
		// empty
	}

	// co::Interface Methods:

	co::IInterfaceType* getInterfaceType() { return co::typeOf<co::IModulePartLoader>::get(); }
	co::IComponent* getInterfaceOwner() { return _handler->getInterfaceOwner(); }
	const std::string& getInterfaceName() { return _handler->getProxyInterfaceName( _cookie ); }
	void componentRetain() { _handler->componentRetain(); }
	void componentRelease() { _handler->componentRelease(); }

	// co.IModulePartLoader Methods:

	bool canLoadModulePart( const std::string& moduleName_ )
	{
		co::Any args[1];
		args[0].set< const std::string& >( moduleName_ );
		co::ArrayRange<co::Any const> range( args, 1 );
		const co::Any& res = _handler->handleMethodInvocation( _cookie, getMethodInfo<co::IModulePartLoader>( 0 ), range );
		return res.get< bool >();
	}

	co::IModulePart* loadModulePart( const std::string& moduleName_ )
	{
		co::Any args[1];
		args[0].set< const std::string& >( moduleName_ );
		co::ArrayRange<co::Any const> range( args, 1 );
		const co::Any& res = _handler->handleMethodInvocation( _cookie, getMethodInfo<co::IModulePartLoader>( 1 ), range );
		return res.get< co::IModulePart* >();
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

class IModulePartLoader_Reflector : public co::ReflectorBase
{
public:
	IModulePartLoader_Reflector()
	{
		// empty
	}

	virtual ~IModulePartLoader_Reflector()
	{
		// empty
	}

	co::IType* getType()
	{
		return co::typeOf<co::IModulePartLoader>::get();
	}

	co::int32 getSize()
	{
		return sizeof(co::IModulePartLoader);
	}

	co::Interface* newProxy( co::IDynamicProxyHandler* handler )
	{
		checValidProxyHandler( handler );
		return co::disambiguate<co::Interface, co::IModulePartLoader>( new co::IModulePartLoader_Proxy( handler ) );
	}

	void getAttribute( const co::Any& instance, co::IAttributeInfo* ai, co::Any& value )
	{
		checkInstance( instance, ai );
		raiseUnexpectedMemberIndex();
		CORAL_UNUSED( value );
	}

	void setAttribute( const co::Any& instance, co::IAttributeInfo* ai, const co::Any& value )
	{
		checkInstance( instance, ai );
		raiseUnexpectedMemberIndex();
		CORAL_UNUSED( value );
	}

	void invokeMethod( const co::Any& instance, co::IMethodInfo* mi, co::ArrayRange<co::Any const> args, co::Any& res )
	{
		co::IModulePartLoader* p = checkInstance( instance, mi );
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
					res.set< co::IModulePart* >( p->loadModulePart( moduleName_ ) );
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
	co::IModulePartLoader* checkInstance( const co::Any& any, co::IMemberInfo* member )
	{
		if( !member )
			throw co::IllegalArgumentException( "illegal null member info" );

		// make sure that 'any' is an instance of this type
		co::IInterfaceType* myType = co::typeOf<co::IModulePartLoader>::get();

		co::IModulePartLoader* res;
		if( any.getKind() != co::TK_INTERFACE || !( res = dynamic_cast<co::IModulePartLoader*>( any.getState().data.itf ) ) )
			CORAL_THROW( co::IllegalArgumentException, "expected a valid co::IModulePartLoader*, but got " << any );

		// make sure that 'member' belongs to this type
		co::ICompoundType* owner = member->getOwner();
		if( owner != myType )
			CORAL_THROW( co::IllegalArgumentException, "member '" << member->getName() << "' belongs to "
				<< owner->getFullName() << ", not to co.IModulePartLoader" );

		return res;
	}
};

// ------ IReflector Creation Function ------ //

co::IReflector* __createIModulePartLoaderIReflector()
{
    return new IModulePartLoader_Reflector;
}

} // namespace co
