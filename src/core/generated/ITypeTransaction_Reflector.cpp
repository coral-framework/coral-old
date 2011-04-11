/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#include <co/ITypeTransaction.h>
#include <co/IDynamicServiceProvider.h>
#include <co/ITypeBuilder.h>
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

class ITypeTransaction_Proxy : public co::ITypeTransaction
{
public:
	ITypeTransaction_Proxy( co::IDynamicServiceProvider* provider ) : _provider( provider )
	{
		_cookie = _provider->dynamicRegisterService( this );
	}

	virtual ~ITypeTransaction_Proxy()
	{
		// empty
	}

	// co::IService Methods:

	co::IInterface* getInterface() { return co::typeOf<co::ITypeTransaction>::get(); }
	co::IObject* getProvider() { return _provider->getProvider(); }
	co::IPort* getFacet() { return _provider->dynamicGetFacet( _cookie ); }
	void serviceRetain() { _provider->serviceRetain(); }
	void serviceRelease() { _provider->serviceRelease(); }

	// co.ITypeTransaction Methods:

	co::Range<co::ITypeBuilder* const> getTypeBuilders()
	{
		const co::Any& res = _provider->dynamicGetField( _cookie, getField<co::ITypeTransaction>( 0 ) );
        return res.get< co::Range<co::ITypeBuilder* const> >();
	}

	void commit()
	{
		co::Range<co::Any const> range;
		_provider->dynamicInvoke( _cookie, getMethod<co::ITypeTransaction>( 0 ), range );
	}

	void rollback()
	{
		co::Range<co::Any const> range;
		_provider->dynamicInvoke( _cookie, getMethod<co::ITypeTransaction>( 1 ), range );
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

class ITypeTransaction_Reflector : public co::ReflectorBase
{
public:
	ITypeTransaction_Reflector()
	{
		// empty
	}

	virtual ~ITypeTransaction_Reflector()
	{
		// empty
	}

	co::IType* getType()
	{
		return co::typeOf<co::ITypeTransaction>::get();
	}

	co::int32 getSize()
	{
		return sizeof(co::ITypeTransaction);
	}

	co::IService* newDynamicProxy( co::IDynamicServiceProvider* provider )
	{
		checkValidDynamicProvider( provider );
		return new co::ITypeTransaction_Proxy( provider );
	}

	void getField( const co::Any& instance, co::IField* field, co::Any& value )
	{
		co::ITypeTransaction* p = co::checkInstance<co::ITypeTransaction>( instance, field );
		switch( field->getIndex() )
		{
		case 0:		value.set< co::Range<co::ITypeBuilder* const> >( p->getTypeBuilders() ); break;
		default:	raiseUnexpectedMemberIndex();
		}
	}

	void setField( const co::Any& instance, co::IField* field, const co::Any& value )
	{
		co::ITypeTransaction* p = co::checkInstance<co::ITypeTransaction>( instance, field );
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
		co::ITypeTransaction* p = co::checkInstance<co::ITypeTransaction>( instance, method );
		checkNumArguments( method, args.getSize() );
		int argIndex = -1;
		try
		{
			switch( method->getIndex() )
			{
			case 1:
				{
					p->commit();
				}
				break;
			case 2:
				{
					p->rollback();
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

co::IReflector* __createITypeTransactionReflector()
{
    return new ITypeTransaction_Reflector;
}

} // namespace co
