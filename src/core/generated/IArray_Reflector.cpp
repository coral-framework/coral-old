/*
 * Coral - Lightweight C++ Component Framework
 * See Copyright Notice in Coral.h
 */

#include <co/IArray.h>
#include <co/IDynamicServiceProvider.h>
#include <co/IAnnotation.h>
#include <co/IReflector.h>
#include <co/INamespace.h>
#include <co/IInterface.h>
#include <co/Uuid.h>
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

class IArray_Proxy : public co::IArray
{
public:
	IArray_Proxy( co::IDynamicServiceProvider* provider ) : _provider( provider )
	{
		_cookie = _provider->dynamicRegisterService( this );
	}

	virtual ~IArray_Proxy()
	{
		// empty
	}

	// co::IService Methods:

	co::IInterface* getInterface() { return co::typeOf<co::IArray>::get(); }
	co::IObject* getProvider() { return _provider->getProvider(); }
	co::IPort* getFacet() { return _provider->dynamicGetFacet( _cookie ); }
	void serviceRetain() { _provider->serviceRetain(); }
	void serviceRelease() { _provider->serviceRelease(); }

	// co.IAnnotated Methods:

	co::Range<co::IAnnotation* const> getAnnotations()
	{
		co::Any res = _provider->dynamicGetField( _cookie, getField<co::IAnnotated>( 0 ) );
        return res.get< co::Range<co::IAnnotation* const> >();
	}

	void setAnnotations( co::Range<co::IAnnotation* const> annotations_ )
	{
		co::Any arg;
		arg.set< co::Range<co::IAnnotation* const> >( annotations_ );
		_provider->dynamicSetField( _cookie, getField<co::IAnnotated>( 0 ), arg );
	}

	void addAnnotation( co::IAnnotation* annotation_ )
	{
		co::Any args[1];
		args[0].set< co::IAnnotation* >( annotation_ );
		co::Range<co::Any const> range( args, 1 );
		_provider->dynamicInvoke( _cookie, getMethod<co::IAnnotated>( 0 ), range );
	}

	co::IAnnotation* getAnnotation( co::IInterface* requestedType_ )
	{
		co::Any args[1];
		args[0].set< co::IInterface* >( requestedType_ );
		co::Range<co::Any const> range( args, 1 );
		co::Any res = _provider->dynamicInvoke( _cookie, getMethod<co::IAnnotated>( 1 ), range );
		return res.get< co::IAnnotation* >();
	}

	// co.IType Methods:

	const co::Uuid& getBinarySignature()
	{
		co::Any res = _provider->dynamicGetField( _cookie, getField<co::IType>( 0 ) );
        return res.get< const co::Uuid& >();
	}

	co::IReflector* getCurrentReflector()
	{
		co::Any res = _provider->dynamicGetField( _cookie, getField<co::IType>( 1 ) );
        return res.get< co::IReflector* >();
	}

	const std::string& getFullName()
	{
		co::Any res = _provider->dynamicGetField( _cookie, getField<co::IType>( 2 ) );
        return res.get< const std::string& >();
	}

	const co::Uuid& getFullSignature()
	{
		co::Any res = _provider->dynamicGetField( _cookie, getField<co::IType>( 3 ) );
        return res.get< const co::Uuid& >();
	}

	co::TypeKind getKind()
	{
		co::Any res = _provider->dynamicGetField( _cookie, getField<co::IType>( 4 ) );
        return res.get< co::TypeKind >();
	}

	const std::string& getName()
	{
		co::Any res = _provider->dynamicGetField( _cookie, getField<co::IType>( 5 ) );
        return res.get< const std::string& >();
	}

	co::INamespace* getNamespace()
	{
		co::Any res = _provider->dynamicGetField( _cookie, getField<co::IType>( 6 ) );
        return res.get< co::INamespace* >();
	}

	co::IReflector* getReflector()
	{
		co::Any res = _provider->dynamicGetField( _cookie, getField<co::IType>( 7 ) );
        return res.get< co::IReflector* >();
	}

	void setReflector( co::IReflector* reflector_ )
	{
		co::Any arg;
		arg.set< co::IReflector* >( reflector_ );
		_provider->dynamicSetField( _cookie, getField<co::IType>( 7 ), arg );
	}

	// co.IArray Methods:

	co::IType* getElementType()
	{
		co::Any res = _provider->dynamicGetField( _cookie, getField<co::IArray>( 0 ) );
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

class IArray_Reflector : public co::ReflectorBase
{
public:
	IArray_Reflector()
	{
		// empty
	}

	virtual ~IArray_Reflector()
	{
		// empty
	}

	co::IType* getType()
	{
		return co::typeOf<co::IArray>::get();
	}

	co::uint32 getSize()
	{
		return sizeof(void*);
	}

	co::IService* newDynamicProxy( co::IDynamicServiceProvider* provider )
	{
		checkValidDynamicProvider( provider );
		return new co::IArray_Proxy( provider );
	}

	void getField( co::Any instance, co::IField* field, co::AnyValue& value )
	{
		co::IArray* p = co::checkInstance<co::IArray>( instance, field );
		switch( field->getIndex() )
		{
		case 0:		value.set< co::IType* >( p->getElementType() ); break;
		default:	raiseUnexpectedMemberIndex();
		}
	}

	void setField( co::Any instance, co::IField* field, co::Any value )
	{
		co::IArray* p = co::checkInstance<co::IArray>( instance, field );
		switch( field->getIndex() )
		{
		case 0:		raiseFieldIsReadOnly( field ); break;
		default:	raiseUnexpectedMemberIndex();
		}
		CORAL_UNUSED( p );
		CORAL_UNUSED( value );
	}

	void invoke( co::Any instance, co::IMethod* method, co::Range<co::Any const> args, co::AnyValue& res )
	{
		co::checkInstance<co::IArray>( instance, method );
		raiseUnexpectedMemberIndex();
		CORAL_UNUSED( args );
		CORAL_UNUSED( res );
	}
};

// ------ Reflector Creation Function ------ //

co::IReflector* __createIArrayReflector()
{
    return new IArray_Reflector;
}

} // namespace co
