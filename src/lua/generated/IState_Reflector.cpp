/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#include <lua/IState.h>
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

class IState_Proxy : public lua::IState
{
public:
	IState_Proxy( co::IDynamicProxyHandler* handler ) : _handler( handler )
	{
		moduleRetain();
		_cookie = _handler->registerProxyInterface( co::disambiguate<co::Interface, lua::IState>( this ) );
	}

	virtual ~IState_Proxy()
	{
		moduleRelease();
	}

	// co::Interface Methods:

	co::IInterfaceType* getInterfaceType() { return co::typeOf<lua::IState>::get(); }
	co::IComponent* getInterfaceOwner() { return _handler->getInterfaceOwner(); }
	const std::string& getInterfaceName() { return _handler->getProxyInterfaceName( _cookie ); }
	void componentRetain() { _handler->componentRetain(); }
	void componentRelease() { _handler->componentRelease(); }

	// lua.IState Methods:

	co::int32 callFunction( const std::string& moduleName_, const std::string& functionName_, co::ArrayRange<co::Any const> args_, co::ArrayRange<co::Any const> results_ )
	{
		co::Any args[4];
		args[0].set< const std::string& >( moduleName_ );
		args[1].set< const std::string& >( functionName_ );
		args[2].set< co::ArrayRange<co::Any const> >( args_ );
		args[3].set< co::ArrayRange<co::Any const> >( results_ );
		co::ArrayRange<co::Any const> range( args, 4 );
		const co::Any& res = _handler->handleMethodInvocation( _cookie, getMethodInfo<lua::IState>( 0 ), range );
		return res.get< co::int32 >();
	}

	bool findScript( const std::string& name_, std::string& filename_ )
	{
		co::Any args[2];
		args[0].set< const std::string& >( name_ );
		args[1].set< std::string& >( filename_ );
		co::ArrayRange<co::Any const> range( args, 2 );
		const co::Any& res = _handler->handleMethodInvocation( _cookie, getMethodInfo<lua::IState>( 1 ), range );
		return res.get< bool >();
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

class IState_Reflector : public co::ReflectorBase
{
public:
	IState_Reflector()
	{
		moduleRetain();
	}

	virtual ~IState_Reflector()
	{
		moduleRelease();
	}

	co::IType* getType()
	{
		return co::typeOf<lua::IState>::get();
	}

	co::int32 getSize()
	{
		return sizeof(lua::IState);
	}

	co::Interface* newProxy( co::IDynamicProxyHandler* handler )
	{
		checValidProxyHandler( handler );
		return co::disambiguate<co::Interface, lua::IState>( new lua::IState_Proxy( handler ) );
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
		lua::IState* p = checkInstance( instance, mi );
		checkNumArguments( mi, args.getSize() );
		int argIndex = -1;
		try
		{
			switch( mi->getIndex() )
			{
			case 0:
				{
					const std::string& moduleName_ = args[++argIndex].get< const std::string& >();
					const std::string& functionName_ = args[++argIndex].get< const std::string& >();
					co::ArrayRange<co::Any const> args_ = args[++argIndex].get< co::ArrayRange<co::Any const> >();
					co::ArrayRange<co::Any const> results_ = args[++argIndex].get< co::ArrayRange<co::Any const> >();
					argIndex = -1;
					res.set< co::int32 >( p->callFunction( moduleName_, functionName_, args_, results_ ) );
				}
				break;
			case 1:
				{
					const std::string& name_ = args[++argIndex].get< const std::string& >();
					std::string& filename_ = args[++argIndex].get< std::string& >();
					argIndex = -1;
					res.set< bool >( p->findScript( name_, filename_ ) );
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
	lua::IState* checkInstance( const co::Any& any, co::IMemberInfo* member )
	{
		if( !member )
			throw co::IllegalArgumentException( "illegal null member info" );

		// make sure that 'any' is an instance of this type
		co::IInterfaceType* myType = co::typeOf<lua::IState>::get();

		lua::IState* res;
		if( any.getKind() != co::TK_INTERFACE || !( res = dynamic_cast<lua::IState*>( any.getState().data.itf ) ) )
			CORAL_THROW( co::IllegalArgumentException, "expected a valid lua::IState*, but got " << any );

		// make sure that 'member' belongs to this type
		co::ICompoundType* owner = member->getOwner();
		if( owner != myType )
			CORAL_THROW( co::IllegalArgumentException, "member '" << member->getName() << "' belongs to "
				<< owner->getFullName() << ", not to lua.IState" );

		return res;
	}
};

// ------ IReflector Creation Function ------ //

co::IReflector* __createIStateIReflector()
{
    return new IState_Reflector;
}

} // namespace lua
