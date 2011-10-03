/*
 * Coral - Lightweight C++ Component Framework
 * See Copyright Notice in Coral.h
 */

#include <co/ICompositeType.h>
#include <co/IDynamicServiceProvider.h>
#include <co/IAnnotation.h>
#include <co/IReflector.h>
#include <co/INamespace.h>
#include <co/IMember.h>
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

class ICompositeType_Proxy : public co::ICompositeType
{
public:
	ICompositeType_Proxy( co::IDynamicServiceProvider* provider ) : _provider( provider )
	{
		_cookie = _provider->dynamicRegisterService( this );
	}

	virtual ~ICompositeType_Proxy()
	{
		// empty
	}

	// co::IService Methods:

	co::IInterface* getInterface() { return co::typeOf<co::ICompositeType>::get(); }
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

	// co.IType Methods:

	const co::Uuid& getBinarySignature()
	{
		const co::Any& res = _provider->dynamicGetField( _cookie, getField<co::IType>( 0 ) );
        return res.get< const co::Uuid& >();
	}

	const std::string& getFullName()
	{
		const co::Any& res = _provider->dynamicGetField( _cookie, getField<co::IType>( 1 ) );
        return res.get< const std::string& >();
	}

	const co::Uuid& getFullSignature()
	{
		const co::Any& res = _provider->dynamicGetField( _cookie, getField<co::IType>( 2 ) );
        return res.get< const co::Uuid& >();
	}

	co::TypeKind getKind()
	{
		const co::Any& res = _provider->dynamicGetField( _cookie, getField<co::IType>( 3 ) );
        return res.get< co::TypeKind >();
	}

	const std::string& getName()
	{
		const co::Any& res = _provider->dynamicGetField( _cookie, getField<co::IType>( 4 ) );
        return res.get< const std::string& >();
	}

	co::INamespace* getNamespace()
	{
		const co::Any& res = _provider->dynamicGetField( _cookie, getField<co::IType>( 5 ) );
        return res.get< co::INamespace* >();
	}

	co::IReflector* getReflector()
	{
		const co::Any& res = _provider->dynamicGetField( _cookie, getField<co::IType>( 6 ) );
        return res.get< co::IReflector* >();
	}

	void setReflector( co::IReflector* reflector_ )
	{
		co::Any arg;
		arg.set< co::IReflector* >( reflector_ );
		_provider->dynamicSetField( _cookie, getField<co::IType>( 6 ), arg );
	}

	// co.ICompositeType Methods:

	co::Range<co::IMember* const> getMembers()
	{
		const co::Any& res = _provider->dynamicGetField( _cookie, getField<co::ICompositeType>( 0 ) );
        return res.get< co::Range<co::IMember* const> >();
	}

	co::IMember* getMember( const std::string& name_ )
	{
		co::Any args[1];
		args[0].set< const std::string& >( name_ );
		co::Range<co::Any const> range( args, 1 );
		const co::Any& res = _provider->dynamicInvoke( _cookie, getMethod<co::ICompositeType>( 0 ), range );
		return res.get< co::IMember* >();
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

class ICompositeType_Reflector : public co::ReflectorBase
{
public:
	ICompositeType_Reflector()
	{
		// empty
	}

	virtual ~ICompositeType_Reflector()
	{
		// empty
	}

	co::IType* getType()
	{
		return co::typeOf<co::ICompositeType>::get();
	}

	co::uint32 getSize()
	{
		return sizeof(co::ICompositeType);
	}

	co::IService* newDynamicProxy( co::IDynamicServiceProvider* provider )
	{
		checkValidDynamicProvider( provider );
		return new co::ICompositeType_Proxy( provider );
	}

	void getField( const co::Any& instance, co::IField* field, co::Any& value )
	{
		co::ICompositeType* p = co::checkInstance<co::ICompositeType>( instance, field );
		switch( field->getIndex() )
		{
		case 0:		value.set< co::Range<co::IMember* const> >( p->getMembers() ); break;
		default:	raiseUnexpectedMemberIndex();
		}
	}

	void setField( const co::Any& instance, co::IField* field, const co::Any& value )
	{
		co::ICompositeType* p = co::checkInstance<co::ICompositeType>( instance, field );
		switch( field->getIndex() )
		{
		case 0:		raiseFieldIsReadOnly( field ); break;
		default:	raiseUnexpectedMemberIndex();
		}
		CORAL_UNUSED( p );
		CORAL_UNUSED( value );
	}

	void invoke( const co::Any& instance, co::IMethod* method, co::Range<co::Any const> args, co::Any& res )
	{
		co::ICompositeType* p = co::checkInstance<co::ICompositeType>( instance, method );
		checkNumArguments( method, args.getSize() );
		int argIndex = -1;
		try
		{
			switch( method->getIndex() )
			{
			case 1:
				{
					const std::string& name_ = args[++argIndex].get< const std::string& >();
					argIndex = -1;
					res.set< co::IMember* >( p->getMember( name_ ) );
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

co::IReflector* __createICompositeTypeReflector()
{
    return new ICompositeType_Reflector;
}

} // namespace co
