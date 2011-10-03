/*
 * Coral - Lightweight C++ Component Framework
 * See Copyright Notice in Coral.h
 */

#include <co/IAnnotated.h>
#include <co/IDynamicServiceProvider.h>
#include <co/IAnnotation.h>
#include <co/IInterface.h>
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

class IAnnotated_Proxy : public co::IAnnotated
{
public:
	IAnnotated_Proxy( co::IDynamicServiceProvider* provider ) : _provider( provider )
	{
		_cookie = _provider->dynamicRegisterService( this );
	}

	virtual ~IAnnotated_Proxy()
	{
		// empty
	}

	// co::IService Methods:

	co::IInterface* getInterface() { return co::typeOf<co::IAnnotated>::get(); }
	co::IObject* getProvider() { return _provider->getProvider(); }
	co::IPort* getFacet() { return _provider->dynamicGetFacet( _cookie ); }
	void serviceRetain() { _provider->serviceRetain(); }
	void serviceRelease() { _provider->serviceRelease(); }

	// co.IAnnotated Methods:

	void addAnnotation( co::IAnnotation* annotation_ )
	{
		co::Any args[1];
		args[0].set< co::IAnnotation* >( annotation_ );
		co::Range<co::Any const> range( args, 1 );
		_provider->dynamicInvoke( _cookie, getMethod<co::IAnnotated>( 0 ), range );
	}

	co::IAnnotation* getAnnotation( co::IInterface* annotationType_ )
	{
		co::Any args[1];
		args[0].set< co::IInterface* >( annotationType_ );
		co::Range<co::Any const> range( args, 1 );
		const co::Any& res = _provider->dynamicInvoke( _cookie, getMethod<co::IAnnotated>( 1 ), range );
		return res.get< co::IAnnotation* >();
	}

	co::Range<co::IAnnotation* const> getAnnotations()
	{
		co::Range<co::Any const> range;
		const co::Any& res = _provider->dynamicInvoke( _cookie, getMethod<co::IAnnotated>( 2 ), range );
		return res.get< co::Range<co::IAnnotation* const> >();
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

class IAnnotated_Reflector : public co::ReflectorBase
{
public:
	IAnnotated_Reflector()
	{
		// empty
	}

	virtual ~IAnnotated_Reflector()
	{
		// empty
	}

	co::IType* getType()
	{
		return co::typeOf<co::IAnnotated>::get();
	}

	co::uint32 getSize()
	{
		return sizeof(co::IAnnotated);
	}

	co::IService* newDynamicProxy( co::IDynamicServiceProvider* provider )
	{
		checkValidDynamicProvider( provider );
		return new co::IAnnotated_Proxy( provider );
	}

	void getField( const co::Any& instance, co::IField* field, co::Any& value )
	{
		co::checkInstance<co::IAnnotated>( instance, field );
		raiseUnexpectedMemberIndex();
		CORAL_UNUSED( value );
	}

	void setField( const co::Any& instance, co::IField* field, const co::Any& value )
	{
		co::checkInstance<co::IAnnotated>( instance, field );
		raiseUnexpectedMemberIndex();
		CORAL_UNUSED( value );
	}

	void invoke( const co::Any& instance, co::IMethod* method, co::Range<co::Any const> args, co::Any& res )
	{
		co::IAnnotated* p = co::checkInstance<co::IAnnotated>( instance, method );
		checkNumArguments( method, args.getSize() );
		int argIndex = -1;
		try
		{
			switch( method->getIndex() )
			{
			case 0:
				{
					co::IAnnotation* annotation_ = args[++argIndex].get< co::IAnnotation* >();
					argIndex = -1;
					p->addAnnotation( annotation_ );
				}
				break;
			case 1:
				{
					co::IInterface* annotationType_ = args[++argIndex].get< co::IInterface* >();
					argIndex = -1;
					res.set< co::IAnnotation* >( p->getAnnotation( annotationType_ ) );
				}
				break;
			case 2:
				{
					res.set< co::Range<co::IAnnotation* const> >( p->getAnnotations() );
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

co::IReflector* __createIAnnotatedReflector()
{
    return new IAnnotated_Reflector;
}

} // namespace co
