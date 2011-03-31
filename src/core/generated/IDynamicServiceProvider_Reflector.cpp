/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#include <co/IDynamicServiceProvider.h>
#include <co/IField.h>
#include <co/IPort.h>
#include <co/IMethod.h>
#include <co/IllegalCastException.h>
#include <co/MissingInputException.h>
#include <co/IllegalArgumentException.h>
#include <co/reserved/ReflectorBase.h>
#include <sstream>
#include <cassert>

namespace co {

// ------ Dynamic Service Proxy ------ //

class IDynamicServiceProvider_Proxy : public co::IDynamicServiceProvider
{
public:
	IDynamicServiceProvider_Proxy( co::IDynamicServiceProvider* provider ) : _provider( provider )
	{
		_cookie = _provider->dynamicRegisterService( co::disambiguate<co::IService, co::IDynamicServiceProvider>( this ) );
	}

	virtual ~IDynamicServiceProvider_Proxy()
	{
		// empty
	}

	// co::IService Methods:

	co::IInterface* getInterface() { return co::typeOf<co::IDynamicServiceProvider>::get(); }
	co::IObject* getProvider() { return _provider->getProvider(); }
	co::IPort* getFacet() { return _provider->dynamicGetFacet( _cookie ); }
	void serviceRetain() { _provider->serviceRetain(); }
	void serviceRelease() { _provider->serviceRelease(); }

	// co.IDynamicServiceProvider Methods:

	co::IPort* dynamicGetFacet( co::int32 dynFacetId_ )
	{
		co::Any args[1];
		args[0].set< co::int32 >( dynFacetId_ );
		co::Range<co::Any const> range( args, 1 );
		const co::Any& res = _provider->dynamicInvoke( _cookie, getMethod<co::IDynamicServiceProvider>( 0 ), range );
		return res.get< co::IPort* >();
	}

	const co::Any& dynamicGetField( co::int32 dynFacetId_, co::IField* field_ )
	{
		co::Any args[2];
		args[0].set< co::int32 >( dynFacetId_ );
		args[1].set< co::IField* >( field_ );
		co::Range<co::Any const> range( args, 2 );
		const co::Any& res = _provider->dynamicInvoke( _cookie, getMethod<co::IDynamicServiceProvider>( 1 ), range );
		return res.get< const co::Any& >();
	}

	const co::Any& dynamicInvoke( co::int32 dynFacetId_, co::IMethod* method_, co::Range<co::Any const> args_ )
	{
		co::Any args[3];
		args[0].set< co::int32 >( dynFacetId_ );
		args[1].set< co::IMethod* >( method_ );
		args[2].set< co::Range<co::Any const> >( args_ );
		co::Range<co::Any const> range( args, 3 );
		const co::Any& res = _provider->dynamicInvoke( _cookie, getMethod<co::IDynamicServiceProvider>( 2 ), range );
		return res.get< const co::Any& >();
	}

	co::int32 dynamicRegisterService( co::IService* dynamicServiceProxy_ )
	{
		co::Any args[1];
		args[0].set< co::IService* >( dynamicServiceProxy_ );
		co::Range<co::Any const> range( args, 1 );
		const co::Any& res = _provider->dynamicInvoke( _cookie, getMethod<co::IDynamicServiceProvider>( 3 ), range );
		return res.get< co::int32 >();
	}

	void dynamicSetField( co::int32 dynFacetId_, co::IField* field_, const co::Any& value_ )
	{
		co::Any args[3];
		args[0].set< co::int32 >( dynFacetId_ );
		args[1].set< co::IField* >( field_ );
		args[2].set< const co::Any& >( value_ );
		co::Range<co::Any const> range( args, 3 );
		_provider->dynamicInvoke( _cookie, getMethod<co::IDynamicServiceProvider>( 4 ), range );
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

class IDynamicServiceProvider_Reflector : public co::ReflectorBase
{
public:
	IDynamicServiceProvider_Reflector()
	{
		// empty
	}

	virtual ~IDynamicServiceProvider_Reflector()
	{
		// empty
	}

	co::IType* getType()
	{
		return co::typeOf<co::IDynamicServiceProvider>::get();
	}

	co::int32 getSize()
	{
		return sizeof(co::IDynamicServiceProvider);
	}

	co::IService* newDynamicProxy( co::IDynamicServiceProvider* provider )
	{
		checkValidDynamicProvider( provider );
		return co::disambiguate<co::IService, co::IDynamicServiceProvider>( new co::IDynamicServiceProvider_Proxy( provider ) );
	}

	void getField( const co::Any& instance, co::IField* field, co::Any& value )
	{
		checkInstance( instance, field );
		raiseUnexpectedMemberIndex();
		CORAL_UNUSED( value );
	}

	void setField( const co::Any& instance, co::IField* field, const co::Any& value )
	{
		checkInstance( instance, field );
		raiseUnexpectedMemberIndex();
		CORAL_UNUSED( value );
	}

	void invoke( const co::Any& instance, co::IMethod* method, co::Range<co::Any const> args, co::Any& res )
	{
		co::IDynamicServiceProvider* p = checkInstance( instance, method );
		checkNumArguments( method, args.getSize() );
		int argIndex = -1;
		try
		{
			switch( method->getIndex() )
			{
			case 0:
				{
					co::int32 dynFacetId_ = args[++argIndex].get< co::int32 >();
					argIndex = -1;
					res.set< co::IPort* >( p->dynamicGetFacet( dynFacetId_ ) );
				}
				break;
			case 1:
				{
					co::int32 dynFacetId_ = args[++argIndex].get< co::int32 >();
					co::IField* field_ = args[++argIndex].get< co::IField* >();
					argIndex = -1;
					res.set< const co::Any& >( p->dynamicGetField( dynFacetId_, field_ ) );
				}
				break;
			case 2:
				{
					co::int32 dynFacetId_ = args[++argIndex].get< co::int32 >();
					co::IMethod* method_ = args[++argIndex].get< co::IMethod* >();
					co::Range<co::Any const> args_ = args[++argIndex].get< co::Range<co::Any const> >();
					argIndex = -1;
					res.set< const co::Any& >( p->dynamicInvoke( dynFacetId_, method_, args_ ) );
				}
				break;
			case 3:
				{
					co::IService* dynamicServiceProxy_ = args[++argIndex].get< co::IService* >();
					argIndex = -1;
					res.set< co::int32 >( p->dynamicRegisterService( dynamicServiceProxy_ ) );
				}
				break;
			case 4:
				{
					co::int32 dynFacetId_ = args[++argIndex].get< co::int32 >();
					co::IField* field_ = args[++argIndex].get< co::IField* >();
					const co::Any& value_ = args[++argIndex].get< const co::Any& >();
					argIndex = -1;
					p->dynamicSetField( dynFacetId_, field_, value_ );
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

private:
	co::IDynamicServiceProvider* checkInstance( const co::Any& any, co::IMember* member )
	{
		if( !member )
			throw co::IllegalArgumentException( "illegal null member info" );

		// make sure that 'any' is an instance of this type
		co::IInterface* myType = co::typeOf<co::IDynamicServiceProvider>::get();

		co::IDynamicServiceProvider* res;
		if( any.getKind() != co::TK_INTERFACE || !( res = dynamic_cast<co::IDynamicServiceProvider*>( any.getState().data.itf ) ) )
			CORAL_THROW( co::IllegalArgumentException, "expected a valid co::IDynamicServiceProvider*, but got " << any );

		// make sure that 'member' belongs to this type
		co::ICompositeType* owner = member->getOwner();
		if( owner != myType )
			CORAL_THROW( co::IllegalArgumentException, "member '" << member->getName() << "' belongs to "
				<< owner->getFullName() << ", not to co.IDynamicServiceProvider" );

		return res;
	}
};

// ------ Reflector Creation Function ------ //

co::IReflector* __createIDynamicServiceProviderReflector()
{
    return new IDynamicServiceProvider_Reflector;
}

} // namespace co
