/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#include <lua/IState.h>
#include <co/IDynamicServiceProvider.h>
#include <co/IMethod.h>
#include <co/IField.h>
#include <co/IllegalCastException.h>
#include <co/MissingInputException.h>
#include <co/IllegalArgumentException.h>
#include <co/reserved/ReflectorBase.h>
#include <sstream>
#include <cassert>

namespace lua {

void moduleRetain();
void moduleRelease();

// ------ Dynamic Service Proxy ------ //

class IState_Proxy : public lua::IState
{
public:
	IState_Proxy( co::IDynamicServiceProvider* provider ) : _provider( provider )
	{
		moduleRetain();
		_cookie = _provider->registerProxyInterface( co::disambiguate<co::IService, lua::IState>( this ) );
	}

	virtual ~IState_Proxy()
	{
		moduleRelease();
	}

	// co::IService Methods:

	co::IInterface* getInterfaceType() { return co::typeOf<lua::IState>::get(); }
	co::IObject* getInterfaceOwner() { return _provider->getInterfaceOwner(); }
	const std::string& getInterfaceName() { return _provider->getProxyInterfaceName( _cookie ); }
	void componentRetain() { _provider->componentRetain(); }
	void componentRelease() { _provider->componentRelease(); }

	// lua.IState Methods:

	co::int32 callFunction( const std::string& moduleName_, const std::string& functionName_, co::Range<co::Any const> args_, co::Range<co::Any const> results_ )
	{
		co::Any args[4];
		args[0].set< const std::string& >( moduleName_ );
		args[1].set< const std::string& >( functionName_ );
		args[2].set< co::Range<co::Any const> >( args_ );
		args[3].set< co::Range<co::Any const> >( results_ );
		co::Range<co::Any const> range( args, 4 );
		const co::Any& res = _provider->handleMethodInvocation( _cookie, getMethodInfo<lua::IState>( 0 ), range );
		return res.get< co::int32 >();
	}

	bool findScript( const std::string& name_, std::string& filename_ )
	{
		co::Any args[2];
		args[0].set< const std::string& >( name_ );
		args[1].set< std::string& >( filename_ );
		co::Range<co::Any const> range( args, 2 );
		const co::Any& res = _provider->handleMethodInvocation( _cookie, getMethodInfo<lua::IState>( 1 ), range );
		return res.get< bool >();
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

	co::IService* newProxy( co::IDynamicServiceProvider* provider )
	{
		checkValidDynamicProvider( provider );
		return co::disambiguate<co::IService, lua::IState>( new lua::IState_Proxy( provider ) );
	}

	void getAttribute( const co::Any& instance, co::IField* ai, co::Any& value )
	{
		checkInstance( instance, ai );
		raiseUnexpectedMemberIndex();
		CORAL_UNUSED( value );
	}

	void setAttribute( const co::Any& instance, co::IField* ai, const co::Any& value )
	{
		checkInstance( instance, ai );
		raiseUnexpectedMemberIndex();
		CORAL_UNUSED( value );
	}

	void invokeMethod( const co::Any& instance, co::IMethod* mi, co::Range<co::Any const> args, co::Any& res )
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
					co::Range<co::Any const> args_ = args[++argIndex].get< co::Range<co::Any const> >();
					co::Range<co::Any const> results_ = args[++argIndex].get< co::Range<co::Any const> >();
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
	lua::IState* checkInstance( const co::Any& any, co::IMember* member )
	{
		if( !member )
			throw co::IllegalArgumentException( "illegal null member info" );

		// make sure that 'any' is an instance of this type
		co::IInterface* myType = co::typeOf<lua::IState>::get();

		lua::IState* res;
		if( any.getKind() != co::TK_INTERFACE || !( res = dynamic_cast<lua::IState*>( any.getState().data.itf ) ) )
			CORAL_THROW( co::IllegalArgumentException, "expected a valid lua::IState*, but got " << any );

		// make sure that 'member' belongs to this type
		co::ICompositeType* owner = member->getOwner();
		if( owner != myType )
			CORAL_THROW( co::IllegalArgumentException, "member '" << member->getName() << "' belongs to "
				<< owner->getFullName() << ", not to lua.IState" );

		return res;
	}
};

// ------ Reflector Creation Function ------ //

co::IReflector* __createIStateReflector()
{
    return new IState_Reflector;
}

} // namespace lua
