/*
 * Coral - Lightweight C++ Component Framework
 * See Copyright Notice in Coral.h
 */

#include <co/IField.h>
#include <co/IDynamicServiceProvider.h>
#include <co/IInterface.h>
#include <co/IType.h>
#include <co/IAnnotation.h>
#include <co/ICompositeType.h>
#include <co/IMethod.h>
#include <co/IllegalCastException.h>
#include <co/MissingInputException.h>
#include <co/IllegalArgumentException.h>
#include <co/reserved/ReflectorBase.h>
#include <sstream>
#include <cassert>

namespace co {

// ------ Dynamic Service Proxy ------ //

class IField_Proxy : public co::IField
{
public:
	IField_Proxy( co::IDynamicServiceProvider* provider ) : _provider( provider )
	{
		_cookie = _provider->dynamicRegisterService( this );
	}

	virtual ~IField_Proxy()
	{
		// empty
	}

	// co::IService Methods:

	co::IInterface* getInterface() { return co::typeOf<co::IField>::get(); }
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
		co::RefPtr<co::ICompositeType> res;
		_provider->dynamicGetField( _cookie, getField<co::IMember>( 3 ), res );
		return res.get();
	}

	// co.IField Methods:

	bool getIsReadOnly()
	{
		bool res;
		_provider->dynamicGetField( _cookie, getField<co::IField>( 0 ), res );
		return res;
	}

	co::IType* getType()
	{
		co::RefPtr<co::IType> res;
		_provider->dynamicGetField( _cookie, getField<co::IField>( 1 ), res );
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

class IField_Reflector : public co::ReflectorBase
{
public:
	IField_Reflector()
	{
		// empty
	}

	virtual ~IField_Reflector()
	{
		// empty
	}

	co::IType* getType()
	{
		return co::typeOf<co::IField>::get();
	}

	co::uint32 getSize()
	{
		return sizeof(void*);
	}

	co::IService* newDynamicProxy( co::IDynamicServiceProvider* provider )
	{
		checkValidDynamicProvider( provider );
		return new co::IField_Proxy( provider );
	}

	void getField( const co::Any& instance, co::IField* field, const co::Any& value )
	{
		co::IField* p = co::checkInstance<co::IField>( instance, field );
		switch( field->getIndex() )
		{
		case 0:		value.put( p->getIsReadOnly() ); break;
		case 1:		value.put( p->getType() ); break;
		default:	raiseUnexpectedMemberIndex();
		}
	}

	void setField( const co::Any& instance, co::IField* field, const co::Any& value )
	{
		co::IField* p = co::checkInstance<co::IField>( instance, field );
		switch( field->getIndex() )
		{
		case 0:		raiseFieldIsReadOnly( field ); break;
		case 1:		raiseFieldIsReadOnly( field ); break;
		default:	raiseUnexpectedMemberIndex();
		}
		CORAL_UNUSED( p );
		CORAL_UNUSED( value );
	}

	void invoke( const co::Any& instance, co::IMethod* method, co::Range<co::Any> args, const co::Any& res )
	{
		co::checkInstance<co::IField>( instance, method );
		raiseUnexpectedMemberIndex();
		CORAL_UNUSED( args );
		CORAL_UNUSED( res );
	}
};

// ------ Reflector Creation Function ------ //

co::IReflector* __createIFieldReflector()
{
    return new IField_Reflector;
}

} // namespace co
