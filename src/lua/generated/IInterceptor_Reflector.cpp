/*
 * Coral - Lightweight C++ Component Framework
 * See Copyright Notice in Coral.h
 */

#include <lua/IInterceptor.h>
#include <co/IDynamicServiceProvider.h>
#include <co/IField.h>
#include <co/IMethod.h>
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

class IInterceptor_Proxy : public lua::IInterceptor
{
public:
	IInterceptor_Proxy( co::IDynamicServiceProvider* provider ) : _provider( provider )
	{
		moduleRetain();
		_cookie = _provider->dynamicRegisterService( this );
	}

	virtual ~IInterceptor_Proxy()
	{
		moduleRelease();
	}

	// co::IService Methods:

	co::IInterface* getInterface() { return co::typeOf<lua::IInterceptor>::get(); }
	co::IObject* getProvider() { return _provider->getProvider(); }
	co::IPort* getFacet() { return _provider->dynamicGetFacet( _cookie ); }
	void serviceRetain() { _provider->serviceRetain(); }
	void serviceRelease() { _provider->serviceRelease(); }

	// lua.IInterceptor Methods:

	void postGetField( co::IService* service_, co::IField* field_, const co::Any& value_ )
	{
		co::Any args[3];
		args[0].set< co::IService* >( service_ );
		args[1].set< co::IField* >( field_ );
		args[2].set< const co::Any& >( value_ );
		co::Range<co::Any const> range( args, 3 );
		_provider->dynamicInvoke( _cookie, getMethod<lua::IInterceptor>( 0 ), range );
	}

	void postInvoke( co::IService* service_, co::IMethod* method_, co::Range<co::Any const> args_, const co::Any& returnValue_ )
	{
		co::Any args[4];
		args[0].set< co::IService* >( service_ );
		args[1].set< co::IMethod* >( method_ );
		args[2].set< co::Range<co::Any const> >( args_ );
		args[3].set< const co::Any& >( returnValue_ );
		co::Range<co::Any const> range( args, 4 );
		_provider->dynamicInvoke( _cookie, getMethod<lua::IInterceptor>( 1 ), range );
	}

	void postSetField( co::IService* service_, co::IField* field_, const co::Any& value_ )
	{
		co::Any args[3];
		args[0].set< co::IService* >( service_ );
		args[1].set< co::IField* >( field_ );
		args[2].set< const co::Any& >( value_ );
		co::Range<co::Any const> range( args, 3 );
		_provider->dynamicInvoke( _cookie, getMethod<lua::IInterceptor>( 2 ), range );
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

class IInterceptor_Reflector : public co::ReflectorBase
{
public:
	IInterceptor_Reflector()
	{
		moduleRetain();
	}

	virtual ~IInterceptor_Reflector()
	{
		moduleRelease();
	}

	co::IType* getType()
	{
		return co::typeOf<lua::IInterceptor>::get();
	}

	co::uint32 getSize()
	{
		return sizeof(void*);
	}

	co::IService* newDynamicProxy( co::IDynamicServiceProvider* provider )
	{
		checkValidDynamicProvider( provider );
		return new lua::IInterceptor_Proxy( provider );
	}

	void getField( const co::Any& instance, co::IField* field, co::Any& value )
	{
		co::checkInstance<lua::IInterceptor>( instance, field );
		raiseUnexpectedMemberIndex();
		CORAL_UNUSED( value );
	}

	void setField( const co::Any& instance, co::IField* field, const co::Any& value )
	{
		co::checkInstance<lua::IInterceptor>( instance, field );
		raiseUnexpectedMemberIndex();
		CORAL_UNUSED( value );
	}

	void invoke( const co::Any& instance, co::IMethod* method, co::Range<co::Any const> args, co::Any& res )
	{
		lua::IInterceptor* p = co::checkInstance<lua::IInterceptor>( instance, method );
		checkNumArguments( method, args.getSize() );
		int argIndex = -1;
		try
		{
			switch( method->getIndex() )
			{
			case 0:
				{
					co::IService* service_ = args[++argIndex].get< co::IService* >();
					co::IField* field_ = args[++argIndex].get< co::IField* >();
					const co::Any& value_ = args[++argIndex].get< const co::Any& >();
					argIndex = -1;
					p->postGetField( service_, field_, value_ );
				}
				break;
			case 1:
				{
					co::IService* service_ = args[++argIndex].get< co::IService* >();
					co::IMethod* method_ = args[++argIndex].get< co::IMethod* >();
					co::Range<co::Any const> args_ = args[++argIndex].get< co::Range<co::Any const> >();
					const co::Any& returnValue_ = args[++argIndex].get< const co::Any& >();
					argIndex = -1;
					p->postInvoke( service_, method_, args_, returnValue_ );
				}
				break;
			case 2:
				{
					co::IService* service_ = args[++argIndex].get< co::IService* >();
					co::IField* field_ = args[++argIndex].get< co::IField* >();
					const co::Any& value_ = args[++argIndex].get< const co::Any& >();
					argIndex = -1;
					p->postSetField( service_, field_, value_ );
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

co::IReflector* __createIInterceptorReflector()
{
    return new IInterceptor_Reflector;
}

} // namespace lua
