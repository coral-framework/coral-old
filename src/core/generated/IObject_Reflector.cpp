/*
 * Coral - Lightweight C++ Component Framework
 * See Copyright Notice in Coral.h
 */

#include <co/IObject.h>
#include <co/IDynamicServiceProvider.h>
#include <co/IPort.h>
#include <co/IComponent.h>
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

class IObject_Proxy : public co::IObject
{
public:
	IObject_Proxy( co::IDynamicServiceProvider* provider ) : _provider( provider )
	{
		_cookie = _provider->dynamicRegisterService( this );
	}

	virtual ~IObject_Proxy()
	{
		// empty
	}

	// co::IService Methods:

	co::IInterface* getInterface() { return co::typeOf<co::IObject>::get(); }
	co::IObject* getProvider() { return _provider->getProvider(); }
	co::IPort* getFacet() { return _provider->dynamicGetFacet( _cookie ); }
	void serviceRetain() { _provider->serviceRetain(); }
	void serviceRelease() { _provider->serviceRelease(); }

	// co.IObject Methods:

	co::IComponent* getComponent()
	{
		co::AnyValue res = _provider->dynamicGetField( _cookie, getField<co::IObject>( 0 ) );
        return res.get< co::IComponent* >();
	}

	co::IService* getServiceAt( co::IPort* port_ )
	{
		co::Any args[1];
		args[0].set< co::IPort* >( port_ );
		co::Range<co::Any> range( args, 1 );
		co::AnyValue res = _provider->dynamicInvoke( _cookie, getMethod<co::IObject>( 0 ), range );
		return res.get< co::IService* >();
	}

	void setServiceAt( co::IPort* receptacle_, co::IService* service_ )
	{
		co::Any args[2];
		args[0].set< co::IPort* >( receptacle_ );
		args[1].set< co::IService* >( service_ );
		co::Range<co::Any> range( args, 2 );
		_provider->dynamicInvoke( _cookie, getMethod<co::IObject>( 1 ), range );
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

class IObject_Reflector : public co::ReflectorBase
{
public:
	IObject_Reflector()
	{
		// empty
	}

	virtual ~IObject_Reflector()
	{
		// empty
	}

	co::IType* getType()
	{
		return co::typeOf<co::IObject>::get();
	}

	co::uint32 getSize()
	{
		return sizeof(void*);
	}

	co::IService* newDynamicProxy( co::IDynamicServiceProvider* provider )
	{
		checkValidDynamicProvider( provider );
		return new co::IObject_Proxy( provider );
	}

	void getField( co::Any instance, co::IField* field, co::AnyValue& value )
	{
		co::IObject* p = co::checkInstance<co::IObject>( instance, field );
		switch( field->getIndex() )
		{
		case 0:		value = p->getComponent(); break;
		default:	raiseUnexpectedMemberIndex();
		}
	}

	void setField( co::Any instance, co::IField* field, co::Any value )
	{
		co::IObject* p = co::checkInstance<co::IObject>( instance, field );
		switch( field->getIndex() )
		{
		case 0:		raiseFieldIsReadOnly( field ); break;
		default:	raiseUnexpectedMemberIndex();
		}
		CORAL_UNUSED( p );
		CORAL_UNUSED( value );
	}

	void invoke( co::Any instance, co::IMethod* method, co::Range<co::Any> args, co::AnyValue& res )
	{
		co::IObject* p = co::checkInstance<co::IObject>( instance, method );
		checkNumArguments( method, args.getSize() );
		int argIndex = -1;
		try
		{
			switch( method->getIndex() )
			{
			case 1:
				{
					co::IPort* port_ = args[++argIndex].get< co::IPort* >();
					argIndex = -1;
					res = p->getServiceAt( port_ );
				}
				break;
			case 2:
				{
					co::IPort* receptacle_ = args[++argIndex].get< co::IPort* >();
					co::IService* service_ = args[++argIndex].get< co::IService* >();
					argIndex = -1;
					p->setServiceAt( receptacle_, service_ );
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

co::IReflector* __createIObjectReflector()
{
    return new IObject_Reflector;
}

} // namespace co
