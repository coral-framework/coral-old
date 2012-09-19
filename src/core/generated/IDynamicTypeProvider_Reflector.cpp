/*
 * Coral - Lightweight C++ Component Framework
 * See Copyright Notice in Coral.h
 */

#include <co/IDynamicTypeProvider.h>
#include <co/IDynamicServiceProvider.h>
#include <co/IType.h>
#include <co/IMethod.h>
#include <co/IField.h>
#include <co/IllegalCastException.h>
#include <co/MissingInputException.h>
#include <co/IllegalArgumentException.h>
#include <co/reserved/ReflectorBase.h>
#include <sstream>
#include <cassert>

namespace co {

// ------ Dynamic Service Proxy ------ //

class IDynamicTypeProvider_Proxy : public co::IDynamicTypeProvider
{
public:
	IDynamicTypeProvider_Proxy( co::IDynamicServiceProvider* provider ) : _provider( provider )
	{
		_cookie = _provider->dynamicRegisterService( this );
	}

	virtual ~IDynamicTypeProvider_Proxy()
	{
		// empty
	}

	// co::IService Methods:

	co::IInterface* getInterface() { return co::typeOf<co::IDynamicTypeProvider>::get(); }
	co::IObject* getProvider() { return _provider->getProvider(); }
	co::IPort* getFacet() { return _provider->dynamicGetFacet( _cookie ); }
	void serviceRetain() { _provider->serviceRetain(); }
	void serviceRelease() { _provider->serviceRelease(); }

	// co.IAnnotation Methods:

	// co.IDynamicTypeProvider Methods:

	void provideReflectorFor( co::IType* type_ )
	{
		co::Any args[] = {
			type_
		};
		co::Range<co::Any> range( args, 1 );
		_provider->dynamicInvoke( _cookie, getMethod<co::IDynamicTypeProvider>( 0 ), range );
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

class IDynamicTypeProvider_Reflector : public co::ReflectorBase
{
public:
	IDynamicTypeProvider_Reflector()
	{
		// empty
	}

	virtual ~IDynamicTypeProvider_Reflector()
	{
		// empty
	}

	co::IType* getType()
	{
		return co::typeOf<co::IDynamicTypeProvider>::get();
	}

	co::uint32 getSize()
	{
		return sizeof(void*);
	}

	co::IService* newDynamicProxy( co::IDynamicServiceProvider* provider )
	{
		checkValidDynamicProvider( provider );
		return new co::IDynamicTypeProvider_Proxy( provider );
	}

	void getField( co::Any instance, co::IField* field, co::AnyValue& value )
	{
		co::checkInstance<co::IDynamicTypeProvider>( instance, field );
		raiseUnexpectedMemberIndex();
		CORAL_UNUSED( value );
	}

	void setField( co::Any instance, co::IField* field, co::Any value )
	{
		co::checkInstance<co::IDynamicTypeProvider>( instance, field );
		raiseUnexpectedMemberIndex();
		CORAL_UNUSED( value );
	}

	void invoke( co::Any instance, co::IMethod* method, co::Range<co::Any> args, co::AnyValue& res )
	{
		co::IDynamicTypeProvider* p = co::checkInstance<co::IDynamicTypeProvider>( instance, method );
		checkNumArguments( method, args.getSize() );
		int argIndex = -1;
		try
		{
			switch( method->getIndex() )
			{
			case 0:
				{
					co::IType* type_ = args[++argIndex].get< co::IType* >();
					argIndex = -1;
					p->provideReflectorFor( type_ );
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

co::IReflector* __createIDynamicTypeProviderReflector()
{
    return new IDynamicTypeProvider_Reflector;
}

} // namespace co
