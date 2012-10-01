/*
 * Coral - Lightweight C++ Component Framework
 * See Copyright Notice in Coral.h
 */

#include <co/IMember.h>
#include <co/IDynamicServiceProvider.h>
#include <co/IAnnotation.h>
#include <co/IInterface.h>
#include <co/ICompositeType.h>
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

class IMember_Proxy : public co::IMember
{
public:
	IMember_Proxy( co::IDynamicServiceProvider* provider ) : _provider( provider )
	{
		_cookie = _provider->dynamicRegisterService( this );
	}

	virtual ~IMember_Proxy()
	{
		// empty
	}

	// co::IService Methods:

	co::IInterface* getInterface() { return co::typeOf<co::IMember>::get(); }
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
		co::Any args[] = {
			annotation_
		};
		co::Range<co::Any> range( args, 1 );
		_provider->dynamicInvoke( _cookie, getMethod<co::IAnnotated>( 0 ), range, co::Any() );
	}

	co::IAnnotation* getAnnotation( co::IInterface* requestedType_ )
	{
		co::Any args[] = {
			requestedType_
		};
		co::Range<co::Any> range( args, 1 );
		co::RefPtr<co::IAnnotation> res;
		_provider->dynamicInvoke( _cookie, getMethod<co::IAnnotated>( 1 ), range, res );
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

class IMember_Reflector : public co::ReflectorBase
{
public:
	IMember_Reflector()
	{
		// empty
	}

	virtual ~IMember_Reflector()
	{
		// empty
	}

	co::IType* getType()
	{
		return co::typeOf<co::IMember>::get();
	}

	co::uint32 getSize()
	{
		return sizeof(void*);
	}

	co::IService* newDynamicProxy( co::IDynamicServiceProvider* provider )
	{
		checkValidDynamicProvider( provider );
		return new co::IMember_Proxy( provider );
	}

	void getField( co::Any instance, co::IField* field, co::Any value )
	{
		co::IMember* p = co::checkInstance<co::IMember>( instance, field );
		switch( field->getIndex() )
		{
		case 0:		value.put( p->getIndex() ); break;
		case 1:		value.put( p->getKind() ); break;
		case 2:		value.put( p->getName() ); break;
		case 3:		value.put( p->getOwner() ); break;
		default:	raiseUnexpectedMemberIndex();
		}
	}

	void setField( co::Any instance, co::IField* field, co::Any value )
	{
		co::IMember* p = co::checkInstance<co::IMember>( instance, field );
		switch( field->getIndex() )
		{
		case 0:		raiseFieldIsReadOnly( field ); break;
		case 1:		raiseFieldIsReadOnly( field ); break;
		case 2:		raiseFieldIsReadOnly( field ); break;
		case 3:		raiseFieldIsReadOnly( field ); break;
		default:	raiseUnexpectedMemberIndex();
		}
		CORAL_UNUSED( p );
		CORAL_UNUSED( value );
	}

	void invoke( co::Any instance, co::IMethod* method, co::Range<co::Any> args, co::Any res )
	{
		co::checkInstance<co::IMember>( instance, method );
		raiseUnexpectedMemberIndex();
		CORAL_UNUSED( args );
		CORAL_UNUSED( res );
	}
};

// ------ Reflector Creation Function ------ //

co::IReflector* __createIMemberReflector()
{
    return new IMember_Reflector;
}

} // namespace co
