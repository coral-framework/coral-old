/*
 * Coral - Lightweight C++ Component Framework
 * See Copyright Notice in Coral.h
 */

#include <co/IReflector.h>
#include <co/IField.h>
#include <co/IDynamicServiceProvider.h>
#include <co/IType.h>
#include <co/IObject.h>
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
		const co::Any& res = _provider->dynamicGetField( _cookie, getField<co::IReflector>( 0 ) );
        return res.get< co::uint32 >();
	}

	co::IType* getType()
	{
		const co::Any& res = _provider->dynamicGetField( _cookie, getField<co::IReflector>( 1 ) );
        return res.get< co::IType* >();
	}

	void getField( const co::Any& instance_, co::IField* field_, co::Any& value_ )
	{
		co::Any args[3];
		args[0].set< const co::Any& >( instance_ );
		args[1].set< co::IField* >( field_ );
		args[2].set< co::Any& >( value_ );
		co::Range<co::Any const> range( args, 3 );
		_provider->dynamicInvoke( _cookie, getMethod<co::IReflector>( 0 ), range );
	}

	void invoke( const co::Any& instance_, co::IMethod* method_, co::Range<co::Any const> args_, co::Any& returnValue_ )
	{
		co::Any args[4];
		args[0].set< const co::Any& >( instance_ );
		args[1].set< co::IMethod* >( method_ );
		args[2].set< co::Range<co::Any const> >( args_ );
		args[3].set< co::Any& >( returnValue_ );
		co::Range<co::Any const> range( args, 4 );
		_provider->dynamicInvoke( _cookie, getMethod<co::IReflector>( 1 ), range );
	}

	co::IService* newDynamicProxy( co::IDynamicServiceProvider* dynamicProvider_ )
	{
		co::Any args[1];
		args[0].set< co::IDynamicServiceProvider* >( dynamicProvider_ );
		co::Range<co::Any const> range( args, 1 );
		const co::Any& res = _provider->dynamicInvoke( _cookie, getMethod<co::IReflector>( 2 ), range );
		return res.get< co::IService* >();
	}

	co::IObject* newInstance()
	{
		co::Range<co::Any const> range;
		const co::Any& res = _provider->dynamicInvoke( _cookie, getMethod<co::IReflector>( 3 ), range );
		return res.get< co::IObject* >();
	}

	void raise( const std::string& message_ )
	{
		co::Any args[1];
		args[0].set< const std::string& >( message_ );
		co::Range<co::Any const> range( args, 1 );
		_provider->dynamicInvoke( _cookie, getMethod<co::IReflector>( 4 ), range );
	}

	void setField( const co::Any& instance_, co::IField* field_, const co::Any& value_ )
	{
		co::Any args[3];
		args[0].set< const co::Any& >( instance_ );
		args[1].set< co::IField* >( field_ );
		args[2].set< const co::Any& >( value_ );
		co::Range<co::Any const> range( args, 3 );
		_provider->dynamicInvoke( _cookie, getMethod<co::IReflector>( 5 ), range );
	}

	// These co::IReflector methods are not part of the reflection system:

	void createValue( void* )
	{
		throw co::NotSupportedException( "co::IReflector::createValue() cannot be called through a proxy interface." );
	}

	void copyValue( const void*, void* )
	{
		throw co::NotSupportedException( "co::IReflector::copyValue() cannot be called through a proxy interface." );
	}

	void destroyValue( void* )
	{
		throw co::NotSupportedException( "co::IReflector::destroyValue() cannot be called through a proxy interface." );
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
		return sizeof(co::IReflector);
	}

	co::IService* newDynamicProxy( co::IDynamicServiceProvider* provider )
	{
		checkValidDynamicProvider( provider );
		return new co::IReflector_Proxy( provider );
	}

	void getField( const co::Any& instance, co::IField* field, co::Any& value )
	{
		co::IReflector* p = co::checkInstance<co::IReflector>( instance, field );
		switch( field->getIndex() )
		{
		case 0:		value.set< co::uint32 >( p->getSize() ); break;
		case 1:		value.set< co::IType* >( p->getType() ); break;
		default:	raiseUnexpectedMemberIndex();
		}
	}

	void setField( const co::Any& instance, co::IField* field, const co::Any& value )
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

	void invoke( const co::Any& instance, co::IMethod* method, co::Range<co::Any const> args, co::Any& res )
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
					const co::Any& instance_ = args[++argIndex].get< const co::Any& >();
					co::IField* field_ = args[++argIndex].get< co::IField* >();
					co::Any& value_ = args[++argIndex].get< co::Any& >();
					argIndex = -1;
					p->getField( instance_, field_, value_ );
				}
				break;
			case 3:
				{
					const co::Any& instance_ = args[++argIndex].get< const co::Any& >();
					co::IMethod* method_ = args[++argIndex].get< co::IMethod* >();
					co::Range<co::Any const> args_ = args[++argIndex].get< co::Range<co::Any const> >();
					co::Any& returnValue_ = args[++argIndex].get< co::Any& >();
					argIndex = -1;
					p->invoke( instance_, method_, args_, returnValue_ );
				}
				break;
			case 4:
				{
					co::IDynamicServiceProvider* dynamicProvider_ = args[++argIndex].get< co::IDynamicServiceProvider* >();
					argIndex = -1;
					res.set< co::IService* >( p->newDynamicProxy( dynamicProvider_ ) );
				}
				break;
			case 5:
				{
					res.set< co::IObject* >( p->newInstance() );
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
					const co::Any& instance_ = args[++argIndex].get< const co::Any& >();
					co::IField* field_ = args[++argIndex].get< co::IField* >();
					const co::Any& value_ = args[++argIndex].get< const co::Any& >();
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
