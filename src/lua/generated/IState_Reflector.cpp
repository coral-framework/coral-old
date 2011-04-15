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
		_cookie = _provider->dynamicRegisterService( this );
	}

	virtual ~IState_Proxy()
	{
		moduleRelease();
	}

	// co::IService Methods:

	co::IInterface* getInterface() { return co::typeOf<lua::IState>::get(); }
	co::IObject* getProvider() { return _provider->getProvider(); }
	co::IPort* getFacet() { return _provider->dynamicGetFacet( _cookie ); }
	void serviceRetain() { _provider->serviceRetain(); }
	void serviceRelease() { _provider->serviceRelease(); }

	// lua.IState Methods:

	co::int32 callFunction( const std::string& moduleName_, const std::string& functionName_, co::Range<co::Any const> args_, co::Range<co::Any const> results_ )
	{
		co::Any args[4];
		args[0].set< const std::string& >( moduleName_ );
		args[1].set< const std::string& >( functionName_ );
		args[2].set< co::Range<co::Any const> >( args_ );
		args[3].set< co::Range<co::Any const> >( results_ );
		co::Range<co::Any const> range( args, 4 );
		const co::Any& res = _provider->dynamicInvoke( _cookie, getMethod<lua::IState>( 0 ), range );
		return res.get< co::int32 >();
	}

	bool findScript( const std::string& name_, std::string& filename_ )
	{
		co::Any args[2];
		args[0].set< const std::string& >( name_ );
		args[1].set< std::string& >( filename_ );
		co::Range<co::Any const> range( args, 2 );
		const co::Any& res = _provider->dynamicInvoke( _cookie, getMethod<lua::IState>( 1 ), range );
		return res.get< bool >();
	}

protected:
	template<typename T>
	co::IField* getField( co::uint32 index )
	{
		return co::typeOf<T>::get()->getFields()[index];
	}

	template<typename T>
	co::IMethod* getMethod( co::uint32 index )
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

	co::uint32 getSize()
	{
		return sizeof(lua::IState);
	}

	co::IService* newDynamicProxy( co::IDynamicServiceProvider* provider )
	{
		checkValidDynamicProvider( provider );
		return new lua::IState_Proxy( provider );
	}

	void getField( const co::Any& instance, co::IField* field, co::Any& value )
	{
		co::checkInstance<lua::IState>( instance, field );
		raiseUnexpectedMemberIndex();
		CORAL_UNUSED( value );
	}

	void setField( const co::Any& instance, co::IField* field, const co::Any& value )
	{
		co::checkInstance<lua::IState>( instance, field );
		raiseUnexpectedMemberIndex();
		CORAL_UNUSED( value );
	}

	void invoke( const co::Any& instance, co::IMethod* method, co::Range<co::Any const> args, co::Any& res )
	{
		lua::IState* p = co::checkInstance<lua::IState>( instance, method );
		checkNumArguments( method, args.getSize() );
		int argIndex = -1;
		try
		{
			switch( method->getIndex() )
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
			raiseArgumentTypeException( method, argIndex, e );
		}
		catch( ... )
		{
			throw;
		}
		CORAL_UNUSED( res );
	}
};

// ------ Reflector Creation Function ------ //

co::IReflector* __createIStateReflector()
{
    return new IState_Reflector;
}

} // namespace lua
