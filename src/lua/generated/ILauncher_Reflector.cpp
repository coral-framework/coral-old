/*
 * Coral - Lightweight C++ Component Framework
 * See Copyright Notice in Coral.h
 */

#include <lua/ILauncher.h>
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

class ILauncher_Proxy : public lua::ILauncher
{
public:
	ILauncher_Proxy( co::IDynamicServiceProvider* provider ) : _provider( provider )
	{
		moduleRetain();
		_cookie = _provider->dynamicRegisterService( this );
	}

	virtual ~ILauncher_Proxy()
	{
		moduleRelease();
	}

	// co::IService Methods:

	co::IInterface* getInterface() { return co::typeOf<lua::ILauncher>::get(); }
	co::IObject* getProvider() { return _provider->getProvider(); }
	co::IPort* getFacet() { return _provider->dynamicGetFacet( _cookie ); }
	void serviceRetain() { _provider->serviceRetain(); }
	void serviceRelease() { _provider->serviceRelease(); }

	// lua.ILauncher Methods:

	co::int32 main( co::Range<std::string> args_ )
	{
		co::Any args[1];
		args[0].set< co::Range<std::string> >( args_ );
		co::Range<co::Any> range( args, 1 );
		const co::Any& res = _provider->dynamicInvoke( _cookie, getMethod<lua::ILauncher>( 0 ), range );
		return res.get< co::int32 >();
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

	co::uint32 getSize()
	{
		return sizeof(void*);
	}

	co::IService* newDynamicProxy( co::IDynamicServiceProvider* provider )
	{
		checkValidDynamicProvider( provider );
		return new lua::ILauncher_Proxy( provider );
	}

	void getField( co::Any instance, co::IField* field, co::AnyValue& value )
	{
		co::checkInstance<lua::ILauncher>( instance, field );
		raiseUnexpectedMemberIndex();
		CORAL_UNUSED( value );
	}

	void setField( co::Any instance, co::IField* field, co::Any value )
	{
		co::checkInstance<lua::ILauncher>( instance, field );
		raiseUnexpectedMemberIndex();
		CORAL_UNUSED( value );
	}

	void invoke( co::Any instance, co::IMethod* method, co::Range<co::Any> args, co::AnyValue& res )
	{
		lua::ILauncher* p = co::checkInstance<lua::ILauncher>( instance, method );
		checkNumArguments( method, args.getSize() );
		int argIndex = -1;
		try
		{
			switch( method->getIndex() )
			{
			case 0:
				{
					co::Range<std::string> args_ = args[++argIndex].get< co::Range<std::string> >();
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

co::IReflector* __createILauncherReflector()
{
    return new ILauncher_Reflector;
}

} // namespace lua
