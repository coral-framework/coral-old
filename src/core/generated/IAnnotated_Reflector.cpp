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

	co::Range<co::IAnnotation*> getAnnotations()
	{
		co::RefVector<co::IAnnotation> res;
		_provider->dynamicGetField( _cookie, getField<co::IAnnotated>( 0 ), res );
		return res;
	}

	void setAnnotations( co::Range<co::IAnnotation*> annotations_ )
	{
		_provider->dynamicSetField( _cookie, getField<co::IAnnotated>( 0 ), annotations_ );
	}

	void addAnnotation( co::IAnnotation* annotation_ )
	{
		co::Any args[] = { annotation_ };
		_provider->dynamicInvoke( _cookie, getMethod<co::IAnnotated>( 0 ), args, co::Any() );
	}

	co::IAnnotation* getAnnotation( co::IInterface* requestedType_ )
	{
		co::Any args[] = { requestedType_ };
		co::RefPtr<co::IAnnotation> res;
		_provider->dynamicInvoke( _cookie, getMethod<co::IAnnotated>( 1 ), args, res );
		return res.get();
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
		return sizeof(void*);
	}

	co::IService* newDynamicProxy( co::IDynamicServiceProvider* provider )
	{
		checkValidDynamicProvider( provider );
		return new co::IAnnotated_Proxy( provider );
	}

	void getField( co::Any instance, co::IField* field, co::Any value )
	{
		co::IAnnotated* p = co::checkInstance<co::IAnnotated>( instance, field );
		switch( field->getIndex() )
		{
		case 0:		value.put( p->getAnnotations() ); break;
		default:	raiseUnexpectedMemberIndex();
		}
	}

	void setField( co::Any instance, co::IField* field, co::Any value )
	{
		co::IAnnotated* p = co::checkInstance<co::IAnnotated>( instance, field );
		switch( field->getIndex() )
		{
		case 0:		p->setAnnotations( value.get< co::Range<co::IAnnotation*> >() ); break;
		default:	raiseUnexpectedMemberIndex();
		}
		CORAL_UNUSED( p );
		CORAL_UNUSED( value );
	}

	void invoke( co::Any instance, co::IMethod* method, co::Range<co::Any> args, co::Any res )
	{
		co::IAnnotated* p = co::checkInstance<co::IAnnotated>( instance, method );
		checkNumArguments( method, args.getSize() );
		int argIndex = -1;
		try
		{
			switch( method->getIndex() )
			{
			case 1:
				{
					co::IAnnotation* annotation_ = args[++argIndex].get< co::IAnnotation* >();
					argIndex = -1;
					p->addAnnotation( annotation_ );
				}
				break;
			case 2:
				{
					co::IInterface* requestedType_ = args[++argIndex].get< co::IInterface* >();
					argIndex = -1;
					res.put( p->getAnnotation( requestedType_ ) );
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
