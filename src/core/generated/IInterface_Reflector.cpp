/*
 * Coral - Lightweight C++ Component Framework
 * See Copyright Notice in Coral.h
 */

#include <co/IInterface.h>
#include <co/IDynamicServiceProvider.h>
#include <co/IInterface.h>
#include <co/IField.h>
#include <co/IReflector.h>
#include <co/INamespace.h>
#include <co/IAnnotation.h>
#include <co/IMember.h>
#include <co/Uuid.h>
#include <co/IMethod.h>
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

	// co.ICompositeType Methods:

	co::Range<co::IMember* const> getMembers()
	{
		co::Any res = _provider->dynamicGetField( _cookie, getField<co::ICompositeType>( 0 ) );
        return res.get< co::Range<co::IMember* const> >();
	}

	co::IMember* getMember( const std::string& name_ )
	{
		co::Any args[1];
		args[0].set< const std::string& >( name_ );
		co::Range<co::Any const> range( args, 1 );
		co::Any res = _provider->dynamicInvoke( _cookie, getMethod<co::ICompositeType>( 0 ), range );
		return res.get< co::IMember* >();
	}

	// co.IRecordType Methods:

	co::Range<co::IField* const> getFields()
	{
		co::Any res = _provider->dynamicGetField( _cookie, getField<co::IRecordType>( 0 ) );
        return res.get< co::Range<co::IField* const> >();
	}

	// co.IClassType Methods:

	co::Range<co::IMethod* const> getMethods()
	{
		co::Any res = _provider->dynamicGetField( _cookie, getField<co::IClassType>( 0 ) );
        return res.get< co::Range<co::IMethod* const> >();
	}

	// co.IInterface Methods:

	co::IInterface* getBaseType()
	{
		co::Any res = _provider->dynamicGetField( _cookie, getField<co::IInterface>( 0 ) );
        return res.get< co::IInterface* >();
	}

	co::Range<co::IInterface* const> getSubTypes()
	{
		co::Any res = _provider->dynamicGetField( _cookie, getField<co::IInterface>( 1 ) );
        return res.get< co::Range<co::IInterface* const> >();
	}

	co::Range<co::IInterface* const> getSuperTypes()
	{
		co::Any res = _provider->dynamicGetField( _cookie, getField<co::IInterface>( 2 ) );
        return res.get< co::Range<co::IInterface* const> >();
	}

	bool isSubTypeOf( co::IInterface* type_ )
	{
		co::Any args[1];
		args[0].set< co::IInterface* >( type_ );
		co::Range<co::Any const> range( args, 1 );
		co::Any res = _provider->dynamicInvoke( _cookie, getMethod<co::IInterface>( 0 ), range );
		return res.get< bool >();
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

	void getField( co::Any instance, co::IField* field, co::AnyValue& value )
	{
		co::IInterface* p = co::checkInstance<co::IInterface>( instance, field );
		switch( field->getIndex() )
		{
		case 0:		value.set< co::IInterface* >( p->getBaseType() ); break;
		case 1:		value.set< co::Range<co::IInterface* const> >( p->getSubTypes() ); break;
		case 2:		value.set< co::Range<co::IInterface* const> >( p->getSuperTypes() ); break;
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

	void invoke( co::Any instance, co::IMethod* method, co::Range<co::Any const> args, co::AnyValue& res )
	{
		co::IInterface* p = co::checkInstance<co::IInterface>( instance, method );
		checkNumArguments( method, args.getSize() );
		int argIndex = -1;
		try
		{
			switch( method->getIndex() )
			{
			case 3:
				{
					co::IInterface* type_ = args[++argIndex].get< co::IInterface* >();
					argIndex = -1;
					res.set< bool >( p->isSubTypeOf( type_ ) );
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

co::IReflector* __createIInterfaceReflector()
{
    return new IInterface_Reflector;
}

} // namespace co
