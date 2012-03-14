/*
 * Coral - Lightweight C++ Component Framework
 * See Copyright Notice in Coral.h
 */

#include <co/IMethod.h>
#include <co/IDynamicServiceProvider.h>
#include <co/IParameter.h>
#include <co/IAnnotation.h>
#include <co/IType.h>
#include <co/IInterface.h>
#include <co/ICompositeType.h>
#include <co/IException.h>
#include <co/IField.h>
#include <co/IllegalCastException.h>
#include <co/MissingInputException.h>
#include <co/IllegalArgumentException.h>
#include <co/reserved/ReflectorBase.h>
#include <sstream>
#include <cassert>

namespace co {

// ------ Dynamic Service Proxy ------ //

class IMethod_Proxy : public co::IMethod
{
public:
	IMethod_Proxy( co::IDynamicServiceProvider* provider ) : _provider( provider )
	{
		_cookie = _provider->dynamicRegisterService( this );
	}

	virtual ~IMethod_Proxy()
	{
		// empty
	}

	// co::IService Methods:

	co::IInterface* getInterface() { return co::typeOf<co::IMethod>::get(); }
	co::IObject* getProvider() { return _provider->getProvider(); }
	co::IPort* getFacet() { return _provider->dynamicGetFacet( _cookie ); }
	void serviceRetain() { _provider->serviceRetain(); }
	void serviceRelease() { _provider->serviceRelease(); }

	// co.IAnnotated Methods:

	co::Range<co::IAnnotation*> getAnnotations()
	{
		co::Any res = _provider->dynamicGetField( _cookie, getField<co::IAnnotated>( 0 ) );
        return res.get< co::Range<co::IAnnotation*> >();
	}

	void setAnnotations( co::Range<co::IAnnotation*> annotations_ )
	{
		co::Any arg;
		arg.set< co::Range<co::IAnnotation*> >( annotations_ );
		_provider->dynamicSetField( _cookie, getField<co::IAnnotated>( 0 ), arg );
	}

	void addAnnotation( co::IAnnotation* annotation_ )
	{
		co::Any args[1];
		args[0].set< co::IAnnotation* >( annotation_ );
		co::Range<co::Any> range( args, 1 );
		_provider->dynamicInvoke( _cookie, getMethod<co::IAnnotated>( 0 ), range );
	}

	co::IAnnotation* getAnnotation( co::IInterface* requestedType_ )
	{
		co::Any args[1];
		args[0].set< co::IInterface* >( requestedType_ );
		co::Range<co::Any> range( args, 1 );
		co::Any res = _provider->dynamicInvoke( _cookie, getMethod<co::IAnnotated>( 1 ), range );
		return res.get< co::IAnnotation* >();
	}

	// co.IMember Methods:

	co::uint16 getIndex()
	{
		co::Any res = _provider->dynamicGetField( _cookie, getField<co::IMember>( 0 ) );
        return res.get< co::uint16 >();
	}

	co::MemberKind getKind()
	{
		co::Any res = _provider->dynamicGetField( _cookie, getField<co::IMember>( 1 ) );
        return res.get< co::MemberKind >();
	}

	const std::string& getName()
	{
		co::Any res = _provider->dynamicGetField( _cookie, getField<co::IMember>( 2 ) );
        return res.get< const std::string& >();
	}

	co::ICompositeType* getOwner()
	{
		co::Any res = _provider->dynamicGetField( _cookie, getField<co::IMember>( 3 ) );
        return res.get< co::ICompositeType* >();
	}

	// co.IMethod Methods:

	co::Range<co::IException*> getExceptions()
	{
		co::Any res = _provider->dynamicGetField( _cookie, getField<co::IMethod>( 0 ) );
        return res.get< co::Range<co::IException*> >();
	}

	co::Range<co::IParameter*> getParameters()
	{
		co::Any res = _provider->dynamicGetField( _cookie, getField<co::IMethod>( 1 ) );
        return res.get< co::Range<co::IParameter*> >();
	}

	co::IType* getReturnType()
	{
		co::Any res = _provider->dynamicGetField( _cookie, getField<co::IMethod>( 2 ) );
        return res.get< co::IType* >();
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

class IMethod_Reflector : public co::ReflectorBase
{
public:
	IMethod_Reflector()
	{
		// empty
	}

	virtual ~IMethod_Reflector()
	{
		// empty
	}

	co::IType* getType()
	{
		return co::typeOf<co::IMethod>::get();
	}

	co::uint32 getSize()
	{
		return sizeof(void*);
	}

	co::IService* newDynamicProxy( co::IDynamicServiceProvider* provider )
	{
		checkValidDynamicProvider( provider );
		return new co::IMethod_Proxy( provider );
	}

	void getField( co::Any instance, co::IField* field, co::AnyValue& value )
	{
		co::IMethod* p = co::checkInstance<co::IMethod>( instance, field );
		switch( field->getIndex() )
		{
		case 0:		value.set< co::Range<co::IException*> >( p->getExceptions() ); break;
		case 1:		value.set< co::Range<co::IParameter*> >( p->getParameters() ); break;
		case 2:		value.set< co::IType* >( p->getReturnType() ); break;
		default:	raiseUnexpectedMemberIndex();
		}
	}

	void setField( co::Any instance, co::IField* field, co::Any value )
	{
		co::IMethod* p = co::checkInstance<co::IMethod>( instance, field );
		switch( field->getIndex() )
		{
		case 0:		raiseFieldIsReadOnly( field ); break;
		case 1:		raiseFieldIsReadOnly( field ); break;
		case 2:		raiseFieldIsReadOnly( field ); break;
		default:	raiseUnexpectedMemberIndex();
		}
		CORAL_UNUSED( p );
		CORAL_UNUSED( value );
	}

	void invoke( co::Any instance, co::IMethod* method, co::Range<co::Any> args, co::AnyValue& res )
	{
		co::checkInstance<co::IMethod>( instance, method );
		raiseUnexpectedMemberIndex();
		CORAL_UNUSED( args );
		CORAL_UNUSED( res );
	}
};

// ------ Reflector Creation Function ------ //

co::IReflector* __createIMethodReflector()
{
    return new IMethod_Reflector;
}

} // namespace co
