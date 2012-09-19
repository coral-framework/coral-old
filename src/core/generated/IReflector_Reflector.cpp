/*
 * Coral - Lightweight C++ Component Framework
 * See Copyright Notice in Coral.h
 */

#include <co/IReflector.h>
#include <co/IType.h>
#include <co/IField.h>
#include <co/IObject.h>
#include <co/IDynamicServiceProvider.h>
#include <co/IMethod.h>
#include <co/IllegalCastException.h>
#include <co/MissingInputException.h>
#include <co/IllegalArgumentException.h>
#include <co/NotSupportedException.h>
#include <co/reserved/ReflectorBase.h>
#include <sstream>
#include <cassert>

namespace co {

// ------ Dynamic Service Proxy ------ //

class IReflector_Proxy : public co::IReflector
{
public:
	IReflector_Proxy( co::IDynamicServiceProvider* provider ) : _provider( provider )
	{
		_cookie = _provider->dynamicRegisterService( this );
	}

	virtual ~IReflector_Proxy()
	{
		// empty
	}

	// co::IService Methods:

	co::IInterface* getInterface() { return co::typeOf<co::IReflector>::get(); }
	co::IObject* getProvider() { return _provider->getProvider(); }
	co::IPort* getFacet() { return _provider->dynamicGetFacet( _cookie ); }
	void serviceRetain() { _provider->serviceRetain(); }
	void serviceRelease() { _provider->serviceRelease(); }

	// co.IReflector Methods:

	co::uint32 getSize()
	{
		co::AnyValue res = _provider->dynamicGetField( _cookie, getField<co::IReflector>( 0 ) );
        return res.get< co::uint32 >();
	}

	co::IType* getType()
	{
		co::AnyValue res = _provider->dynamicGetField( _cookie, getField<co::IReflector>( 1 ) );
        return res.get< co::IType* >();
	}

	void getField( co::Any instance_, co::IField* field_, co::AnyValue& value_ )
	{
		co::Any args[] = {
			instance_,
			field_,
			value_
		};
		co::Range<co::Any> range( args, 3 );
		_provider->dynamicInvoke( _cookie, getMethod<co::IReflector>( 0 ), range );
	}

	void invoke( co::Any instance_, co::IMethod* method_, co::Range<co::Any> args_, co::AnyValue& returnValue_ )
	{
		co::Any args[] = {
			instance_,
			method_,
			args_,
			returnValue_
		};
		co::Range<co::Any> range( args, 4 );
		_provider->dynamicInvoke( _cookie, getMethod<co::IReflector>( 1 ), range );
	}

	co::IService* newDynamicProxy( co::IDynamicServiceProvider* dynamicProvider_ )
	{
		co::Any args[] = {
			dynamicProvider_
		};
		co::Range<co::Any> range( args, 1 );
		co::AnyValue res = _provider->dynamicInvoke( _cookie, getMethod<co::IReflector>( 2 ), range );
		return res.get< co::IService* >();
	}

	co::IObject* newInstance()
	{
		co::Range<co::Any> range;
		co::AnyValue res = _provider->dynamicInvoke( _cookie, getMethod<co::IReflector>( 3 ), range );
		return res.get< co::IObject* >();
	}

	void raise( const std::string& message_ )
	{
		co::Any args[] = {
			message_
		};
		co::Range<co::Any> range( args, 1 );
		_provider->dynamicInvoke( _cookie, getMethod<co::IReflector>( 4 ), range );
	}

	void setField( co::Any instance_, co::IField* field_, co::Any value_ )
	{
		co::Any args[] = {
			instance_,
			field_,
			value_
		};
		co::Range<co::Any> range( args, 3 );
		_provider->dynamicInvoke( _cookie, getMethod<co::IReflector>( 5 ), range );
	}

	// These co::IReflector methods are not part of the reflection system:

	void createValues( void*, size_t )
	{
		throw co::NotSupportedException( "co::IReflector::createValues() cannot be called through a proxy" );
	}

	void copyValues( const void*, void*, size_t )
	{
		throw co::NotSupportedException( "co::IReflector::copyValues() cannot be called through a proxy" );
	}

	void destroyValues( void*, size_t )
	{
		throw co::NotSupportedException( "co::IReflector::destroyValues() cannot be called through a proxy" );
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

class IReflector_Reflector : public co::ReflectorBase
{
public:
	IReflector_Reflector()
	{
		// empty
	}

	virtual ~IReflector_Reflector()
	{
		// empty
	}

	co::IType* getType()
	{
		return co::typeOf<co::IReflector>::get();
	}

	co::uint32 getSize()
	{
		return sizeof(void*);
	}

	co::IService* newDynamicProxy( co::IDynamicServiceProvider* provider )
	{
		checkValidDynamicProvider( provider );
		return new co::IReflector_Proxy( provider );
	}

	void getField( co::Any instance, co::IField* field, co::AnyValue& value )
	{
		co::IReflector* p = co::checkInstance<co::IReflector>( instance, field );
		switch( field->getIndex() )
		{
		case 0:		value = p->getSize(); break;
		case 1:		value = p->getType(); break;
		default:	raiseUnexpectedMemberIndex();
		}
	}

	void setField( co::Any instance, co::IField* field, co::Any value )
	{
		co::IReflector* p = co::checkInstance<co::IReflector>( instance, field );
		switch( field->getIndex() )
		{
		case 0:		raiseFieldIsReadOnly( field ); break;
		case 1:		raiseFieldIsReadOnly( field ); break;
		default:	raiseUnexpectedMemberIndex();
		}
		CORAL_UNUSED( p );
		CORAL_UNUSED( value );
	}

	void invoke( co::Any instance, co::IMethod* method, co::Range<co::Any> args, co::AnyValue& res )
	{
		co::IReflector* p = co::checkInstance<co::IReflector>( instance, method );
		checkNumArguments( method, args.getSize() );
		int argIndex = -1;
		try
		{
			switch( method->getIndex() )
			{
			case 2:
				{
					co::Any instance_ = args[++argIndex];
					co::IField* field_ = args[++argIndex].get< co::IField* >();
					co::AnyValue& value_ = args[++argIndex].get< co::AnyValue& >();
					argIndex = -1;
					p->getField( instance_, field_, value_ );
				}
				break;
			case 3:
				{
					co::Any instance_ = args[++argIndex];
					co::IMethod* method_ = args[++argIndex].get< co::IMethod* >();
					co::Range<co::Any> args_ = args[++argIndex].get< co::Range<co::Any> >();
					co::AnyValue& returnValue_ = args[++argIndex].get< co::AnyValue& >();
					argIndex = -1;
					p->invoke( instance_, method_, args_, returnValue_ );
				}
				break;
			case 4:
				{
					co::IDynamicServiceProvider* dynamicProvider_ = args[++argIndex].get< co::IDynamicServiceProvider* >();
					argIndex = -1;
					res = p->newDynamicProxy( dynamicProvider_ );
				}
				break;
			case 5:
				{
					res = p->newInstance();
				}
				break;
			case 6:
				{
					const std::string& message_ = args[++argIndex].get< const std::string& >();
					argIndex = -1;
					p->raise( message_ );
				}
				break;
			case 7:
				{
					co::Any instance_ = args[++argIndex];
					co::IField* field_ = args[++argIndex].get< co::IField* >();
					co::Any value_ = args[++argIndex];
					argIndex = -1;
					p->setField( instance_, field_, value_ );
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

co::IReflector* __createIReflectorReflector()
{
    return new IReflector_Reflector;
}

} // namespace co
