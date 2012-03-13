/*
 * Coral - Lightweight C++ Component Framework
 * See Copyright Notice in Coral.h
 */

#include <co/IComponent.h>
#include <co/IDynamicServiceProvider.h>
#include <co/IAnnotation.h>
#include <co/IReflector.h>
#include <co/IPort.h>
#include <co/IInterface.h>
#include <co/IMember.h>
#include <co/INamespace.h>
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

class IComponent_Proxy : public co::IComponent
{
public:
	IComponent_Proxy( co::IDynamicServiceProvider* provider ) : _provider( provider )
	{
		_cookie = _provider->dynamicRegisterService( this );
	}

	virtual ~IComponent_Proxy()
	{
		// empty
	}

	// co::IService Methods:

	co::IInterface* getInterface() { return co::typeOf<co::IComponent>::get(); }
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

	// co.ICompositeType Methods:

	co::Range<co::IMember*> getMembers()
	{
		co::Any res = _provider->dynamicGetField( _cookie, getField<co::ICompositeType>( 0 ) );
        return res.get< co::Range<co::IMember*> >();
	}

	co::IMember* getMember( const std::string& name_ )
	{
		co::Any args[1];
		args[0].set< const std::string& >( name_ );
		co::Range<co::Any> range( args, 1 );
		co::Any res = _provider->dynamicInvoke( _cookie, getMethod<co::ICompositeType>( 0 ), range );
		return res.get< co::IMember* >();
	}

	// co.IComponent Methods:

	co::Range<co::IPort*> getFacets()
	{
		co::Any res = _provider->dynamicGetField( _cookie, getField<co::IComponent>( 0 ) );
        return res.get< co::Range<co::IPort*> >();
	}

	co::Range<co::IPort*> getPorts()
	{
		co::Any res = _provider->dynamicGetField( _cookie, getField<co::IComponent>( 1 ) );
        return res.get< co::Range<co::IPort*> >();
	}

	co::Range<co::IPort*> getReceptacles()
	{
		co::Any res = _provider->dynamicGetField( _cookie, getField<co::IComponent>( 2 ) );
        return res.get< co::Range<co::IPort*> >();
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

class IComponent_Reflector : public co::ReflectorBase
{
public:
	IComponent_Reflector()
	{
		// empty
	}

	virtual ~IComponent_Reflector()
	{
		// empty
	}

	co::IType* getType()
	{
		return co::typeOf<co::IComponent>::get();
	}

	co::uint32 getSize()
	{
		return sizeof(void*);
	}

	co::IService* newDynamicProxy( co::IDynamicServiceProvider* provider )
	{
		checkValidDynamicProvider( provider );
		return new co::IComponent_Proxy( provider );
	}

	void getField( co::Any instance, co::IField* field, co::AnyValue& value )
	{
		co::IComponent* p = co::checkInstance<co::IComponent>( instance, field );
		switch( field->getIndex() )
		{
		case 0:		value.set< co::Range<co::IPort*> >( p->getFacets() ); break;
		case 1:		value.set< co::Range<co::IPort*> >( p->getPorts() ); break;
		case 2:		value.set< co::Range<co::IPort*> >( p->getReceptacles() ); break;
		default:	raiseUnexpectedMemberIndex();
		}
	}

	void setField( co::Any instance, co::IField* field, co::Any value )
	{
		co::IComponent* p = co::checkInstance<co::IComponent>( instance, field );
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
		co::checkInstance<co::IComponent>( instance, method );
		raiseUnexpectedMemberIndex();
		CORAL_UNUSED( args );
		CORAL_UNUSED( res );
	}
};

// ------ Reflector Creation Function ------ //

co::IReflector* __createIComponentReflector()
{
    return new IComponent_Reflector;
}

} // namespace co
