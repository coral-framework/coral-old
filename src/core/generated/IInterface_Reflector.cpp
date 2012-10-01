/*
 * Coral - Lightweight C++ Component Framework
 * See Copyright Notice in Coral.h
 */

#include <co/IInterface.h>
#include <co/IDynamicServiceProvider.h>
#include <co/IAnnotation.h>
#include <co/INamespace.h>
#include <co/IReflector.h>
#include <co/IInterface.h>
#include <co/IMethod.h>
#include <co/IField.h>
#include <co/IType.h>
#include <co/IMember.h>
#include <co/Uuid.h>
#include <co/IllegalCastException.h>
#include <co/MissingInputException.h>
#include <co/IllegalArgumentException.h>
#include <co/reserved/ReflectorBase.h>
#include <sstream>
#include <cassert>

namespace co {

// ------ Dynamic Service Proxy ------ //

class IInterface_Proxy : public co::IInterface
{
public:
	IInterface_Proxy( co::IDynamicServiceProvider* provider ) : _provider( provider )
	{
		_cookie = _provider->dynamicRegisterService( this );
	}

	virtual ~IInterface_Proxy()
	{
		// empty
	}

	// co::IService Methods:

	co::IInterface* getInterface() { return co::typeOf<co::IInterface>::get(); }
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

	// co.IType Methods:

	co::Uuid getBinarySignature()
	{
		co::Uuid res;
		_provider->dynamicGetField( _cookie, getField<co::IType>( 0 ), res );
		return res;
	}

	co::IReflector* getCurrentReflector()
	{
		co::RefPtr<co::IReflector> res;
		_provider->dynamicGetField( _cookie, getField<co::IType>( 1 ), res );
		return res.get();
	}

	std::string getFullName()
	{
		std::string res;
		_provider->dynamicGetField( _cookie, getField<co::IType>( 2 ), res );
		return res;
	}

	co::Uuid getFullSignature()
	{
		co::Uuid res;
		_provider->dynamicGetField( _cookie, getField<co::IType>( 3 ), res );
		return res;
	}

	co::TypeKind getKind()
	{
		co::TypeKind res;
		_provider->dynamicGetField( _cookie, getField<co::IType>( 4 ), res );
		return res;
	}

	std::string getName()
	{
		std::string res;
		_provider->dynamicGetField( _cookie, getField<co::IType>( 5 ), res );
		return res;
	}

	co::INamespace* getNamespace()
	{
		co::RefPtr<co::INamespace> res;
		_provider->dynamicGetField( _cookie, getField<co::IType>( 6 ), res );
		return res.get();
	}

	co::IReflector* getReflector()
	{
		co::RefPtr<co::IReflector> res;
		_provider->dynamicGetField( _cookie, getField<co::IType>( 7 ), res );
		return res.get();
	}

	void setReflector( co::IReflector* reflector_ )
	{
		_provider->dynamicSetField( _cookie, getField<co::IType>( 7 ), reflector_ );
	}

	bool isA( co::IType* type_ )
	{
		co::Any args[] = {
			type_
		};
		co::Range<co::Any> range( args, 1 );
		bool res;
		_provider->dynamicInvoke( _cookie, getMethod<co::IType>( 0 ), range, res );
		return res;
	}

	// co.ICompositeType Methods:

	co::Range<co::IMember*> getMembers()
	{
		co::RefVector<co::IMember> res;
		_provider->dynamicGetField( _cookie, getField<co::ICompositeType>( 0 ), res );
		return res;
	}

	co::IMember* getMember( const std::string& name_ )
	{
		co::Any args[] = {
			name_
		};
		co::Range<co::Any> range( args, 1 );
		co::RefPtr<co::IMember> res;
		_provider->dynamicInvoke( _cookie, getMethod<co::ICompositeType>( 0 ), range, res );
		return res.get();
	}

	// co.IRecordType Methods:

	co::Range<co::IField*> getFields()
	{
		co::RefVector<co::IField> res;
		_provider->dynamicGetField( _cookie, getField<co::IRecordType>( 0 ), res );
		return res;
	}

	// co.IClassType Methods:

	co::Range<co::IMethod*> getMethods()
	{
		co::RefVector<co::IMethod> res;
		_provider->dynamicGetField( _cookie, getField<co::IClassType>( 0 ), res );
		return res;
	}

	// co.IInterface Methods:

	co::IInterface* getBaseType()
	{
		co::RefPtr<co::IInterface> res;
		_provider->dynamicGetField( _cookie, getField<co::IInterface>( 0 ), res );
		return res.get();
	}

	co::Range<co::IInterface*> getSubTypes()
	{
		co::RefVector<co::IInterface> res;
		_provider->dynamicGetField( _cookie, getField<co::IInterface>( 1 ), res );
		return res;
	}

	co::Range<co::IInterface*> getSuperTypes()
	{
		co::RefVector<co::IInterface> res;
		_provider->dynamicGetField( _cookie, getField<co::IInterface>( 2 ), res );
		return res;
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

class IInterface_Reflector : public co::ReflectorBase
{
public:
	IInterface_Reflector()
	{
		// empty
	}

	virtual ~IInterface_Reflector()
	{
		// empty
	}

	co::IType* getType()
	{
		return co::typeOf<co::IInterface>::get();
	}

	co::uint32 getSize()
	{
		return sizeof(void*);
	}

	co::IService* newDynamicProxy( co::IDynamicServiceProvider* provider )
	{
		checkValidDynamicProvider( provider );
		return new co::IInterface_Proxy( provider );
	}

	void getField( co::Any instance, co::IField* field, co::Any value )
	{
		co::IInterface* p = co::checkInstance<co::IInterface>( instance, field );
		switch( field->getIndex() )
		{
		case 0:		value.put( p->getBaseType() ); break;
		case 1:		value.put( p->getSubTypes() ); break;
		case 2:		value.put( p->getSuperTypes() ); break;
		default:	raiseUnexpectedMemberIndex();
		}
	}

	void setField( co::Any instance, co::IField* field, co::Any value )
	{
		co::IInterface* p = co::checkInstance<co::IInterface>( instance, field );
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

	void invoke( co::Any instance, co::IMethod* method, co::Range<co::Any> args, co::Any res )
	{
		co::checkInstance<co::IInterface>( instance, method );
		raiseUnexpectedMemberIndex();
		CORAL_UNUSED( args );
		CORAL_UNUSED( res );
	}
};

// ------ Reflector Creation Function ------ //

co::IReflector* __createIInterfaceReflector()
{
    return new IInterface_Reflector;
}

} // namespace co
