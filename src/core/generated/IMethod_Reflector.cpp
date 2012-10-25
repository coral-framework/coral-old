/*
 * Coral - Lightweight C++ Component Framework
 * See Copyright Notice in Coral.h
 */

#include <co/IMethod.h>
#include <co/IDynamicServiceProvider.h>
#include <co/IException.h>
#include <co/ICompositeType.h>
#include <co/IAnnotation.h>
#include <co/IInterface.h>
#include <co/IParameter.h>
#include <co/IType.h>
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

	co::TSlice<co::IAnnotation*> getAnnotations()
	{
		typedef co::Temporary<std::vector<co::IAnnotationRef> > Temporary;
		std::unique_ptr<Temporary> temp( new Temporary );
		auto& res = temp->value;
		_provider->dynamicGetField( _cookie, getField<co::IAnnotated>( 0 ), res );
		return co::TSlice<co::IAnnotation*>( res, temp.release() );
	}

	void setAnnotations( co::Slice<co::IAnnotation*> annotations_ )
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
		co::IAnnotationRef res;
		_provider->dynamicInvoke( _cookie, getMethod<co::IAnnotated>( 1 ), args, res );
		return res.get();
	}

	// co.IMember Methods:

	co::uint16 getIndex()
	{
		co::uint16 res;
		_provider->dynamicGetField( _cookie, getField<co::IMember>( 0 ), res );
		return res;
	}

	co::MemberKind getKind()
	{
		co::MemberKind res;
		_provider->dynamicGetField( _cookie, getField<co::IMember>( 1 ), res );
		return res;
	}

	std::string getName()
	{
		std::string res;
		_provider->dynamicGetField( _cookie, getField<co::IMember>( 2 ), res );
		return res;
	}

	co::ICompositeType* getOwner()
	{
		co::ICompositeTypeRef res;
		_provider->dynamicGetField( _cookie, getField<co::IMember>( 3 ), res );
		return res.get();
	}

	// co.IMethod Methods:

	co::TSlice<co::IException*> getExceptions()
	{
		typedef co::Temporary<std::vector<co::IExceptionRef> > Temporary;
		std::unique_ptr<Temporary> temp( new Temporary );
		auto& res = temp->value;
		_provider->dynamicGetField( _cookie, getField<co::IMethod>( 0 ), res );
		return co::TSlice<co::IException*>( res, temp.release() );
	}

	co::TSlice<co::IParameter*> getParameters()
	{
		typedef co::Temporary<std::vector<co::IParameterRef> > Temporary;
		std::unique_ptr<Temporary> temp( new Temporary );
		auto& res = temp->value;
		_provider->dynamicGetField( _cookie, getField<co::IMethod>( 1 ), res );
		return co::TSlice<co::IParameter*>( res, temp.release() );
	}

	co::IType* getReturnType()
	{
		co::ITypeRef res;
		_provider->dynamicGetField( _cookie, getField<co::IMethod>( 2 ), res );
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

	void getField( const co::Any& instance, co::IField* field, const co::Any& value )
	{
		co::IMethod* p = co::checkInstance<co::IMethod>( instance, field );
		switch( field->getIndex() )
		{
		case 0:		value.put( p->getExceptions() ); break;
		case 1:		value.put( p->getParameters() ); break;
		case 2:		value.put( p->getReturnType() ); break;
		default:	raiseUnexpectedMemberIndex();
		}
	}

	void setField( const co::Any& instance, co::IField* field, const co::Any& value )
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

	void invoke( const co::Any& instance, co::IMethod* method, co::Slice<co::Any> args, const co::Any& res )
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
