/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#include <lua/ILauncher.h>
#include <co/IDynamicProxyHandler.h>
#include <co/IMethodInfo.h>
#include <co/IAttributeInfo.h>
#include <co/IllegalCastException.h>
#include <co/MissingInputException.h>
#include <co/IllegalArgumentException.h>
#include <co/reserved/ReflectorBase.h>
#include <sstream>
#include <cassert>

namespace lua {

void moduleRetain();
void moduleRelease();

// ------ Proxy Interface ------ //

class ILauncher_Proxy : public lua::ILauncher
{
public:
	ILauncher_Proxy( co::IDynamicProxyHandler* handler ) : _handler( handler )
	{
		moduleRetain();
		_cookie = _handler->registerProxyInterface( co::disambiguate<co::Interface, lua::ILauncher>( this ) );
	}

	virtual ~ILauncher_Proxy()
	{
		moduleRelease();
	}

	// co::Interface Methods:

	co::IInterfaceType* getInterfaceType() { return co::typeOf<lua::ILauncher>::get(); }
	co::IComponent* getInterfaceOwner() { return _handler->getInterfaceOwner(); }
	const std::string& getInterfaceName() { return _handler->getProxyInterfaceName( _cookie ); }
	void componentRetain() { _handler->componentRetain(); }
	void componentRelease() { _handler->componentRelease(); }

	// lua.ILauncher Methods:

	co::int32 main( co::ArrayRange<std::string const> args_ )
	{
		co::Any args[1];
		args[0].set< co::ArrayRange<std::string const> >( args_ );
		co::ArrayRange<co::Any const> range( args, 1 );
		const co::Any& res = _handler->handleMethodInvocation( _cookie, getMethodInfo<lua::ILauncher>( 0 ), range );
		return res.get< co::int32 >();
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

class ILauncher_Reflector : public co::ReflectorBase
{
public:
	ILauncher_Reflector()
	{
		moduleRetain();
	}

	virtual ~ILauncher_Reflector()
	{
		moduleRelease();
	}

	co::IType* getType()
	{
		return co::typeOf<lua::ILauncher>::get();
	}

	co::int32 getSize()
	{
		return sizeof(lua::ILauncher);
	}

	co::Interface* newProxy( co::IDynamicProxyHandler* handler )
	{
		checValidProxyHandler( handler );
		return co::disambiguate<co::Interface, lua::ILauncher>( new lua::ILauncher_Proxy( handler ) );
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
		lua::ILauncher* p = checkInstance( instance, mi );
		checkNumArguments( mi, args.getSize() );
		int argIndex = -1;
		try
		{
			switch( mi->getIndex() )
			{
			case 0:
				{
					co::ArrayRange<std::string const> args_ = args[++argIndex].get< co::ArrayRange<std::string const> >();
					argIndex = -1;
					res.set< co::int32 >( p->main( args_ ) );
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
	lua::ILauncher* checkInstance( const co::Any& any, co::IMemberInfo* member )
	{
		if( !member )
			throw co::IllegalArgumentException( "illegal null member info" );

		// make sure that 'any' is an instance of this type
		co::IInterfaceType* myType = co::typeOf<lua::ILauncher>::get();

		lua::ILauncher* res;
		if( any.getKind() != co::TK_INTERFACE || !( res = dynamic_cast<lua::ILauncher*>( any.getState().data.itf ) ) )
			CORAL_THROW( co::IllegalArgumentException, "expected a valid lua::ILauncher*, but got " << any );

		// make sure that 'member' belongs to this type
		co::ICompoundType* owner = member->getOwner();
		if( owner != myType )
			CORAL_THROW( co::IllegalArgumentException, "member '" << member->getName() << "' belongs to "
				<< owner->getFullName() << ", not to lua.ILauncher" );

		return res;
	}
};

// ------ IReflector Creation Function ------ //

co::IReflector* __createILauncherIReflector()
{
    return new ILauncher_Reflector;
}

} // namespace lua
