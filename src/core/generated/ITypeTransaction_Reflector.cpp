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
		_cookie = _provider->registerProxyInterface( co::disambiguate<co::IService, co::ITypeTransaction>( this ) );
	}

	virtual ~ITypeTransaction_Proxy()
	{
		// empty
	}

	// co::IService Methods:

	co::IInterface* getInterfaceType() { return co::typeOf<co::ITypeTransaction>::get(); }
	co::IObject* getInterfaceOwner() { return _provider->getInterfaceOwner(); }
	const std::string& getInterfaceName() { return _provider->getProxyInterfaceName( _cookie ); }
	void componentRetain() { _provider->componentRetain(); }
	void componentRelease() { _provider->componentRelease(); }

	// co.ITypeTransaction Methods:

	co::Range<co::ITypeBuilder* const> getTypeBuilders()
	{
		const co::Any& res = _provider->handleGetAttribute( _cookie, getAttribInfo<co::ITypeTransaction>( 0 ) );
        return res.get< co::Range<co::ITypeBuilder* const> >();
	}

	void commit()
	{
		co::Range<co::Any const> range;
		_provider->handleMethodInvocation( _cookie, getMethodInfo<co::ITypeTransaction>( 0 ), range );
	}

	void rollback()
	{
		co::Range<co::Any const> range;
		_provider->handleMethodInvocation( _cookie, getMethodInfo<co::ITypeTransaction>( 1 ), range );
	}

protected:
	template<typename T>
	co::IField* getAttribInfo( co::uint32 index )
	{
		return co::typeOf<T>::get()->getFields()[index];
	}

	template<typename T>
	co::IMethod* getMethodInfo( co::uint32 index )
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

	co::IService* newProxy( co::IDynamicServiceProvider* provider )
	{
		checkValidDynamicProvider( provider );
		return co::disambiguate<co::IService, co::ITypeTransaction>( new co::ITypeTransaction_Proxy( provider ) );
	}

	void getAttribute( const co::Any& instance, co::IField* ai, co::Any& value )
	{
		co::ITypeTransaction* p = checkInstance( instance, ai );
		switch( ai->getIndex() )
		{
		case 0:		value.set< co::Range<co::ITypeBuilder* const> >( p->getTypeBuilders() ); break;
		default:	raiseUnexpectedMemberIndex();
		}
	}

	void setAttribute( const co::Any& instance, co::IField* ai, const co::Any& value )
	{
		co::ITypeTransaction* p = checkInstance( instance, ai );
		switch( ai->getIndex() )
		{
		case 0:		raiseAttributeIsReadOnly( ai ); break;
		default:	raiseUnexpectedMemberIndex();
		}
		CORAL_UNUSED( p );
		CORAL_UNUSED( value );
	}

	void invokeMethod( const co::Any& instance, co::IMethod* mi, co::Range<co::Any const> args, co::Any& res )
	{
		co::ITypeTransaction* p = checkInstance( instance, mi );
		checkNumArguments( mi, args.getSize() );
		int argIndex = -1;
		try
		{
			switch( mi->getIndex() )
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
			raiseArgumentTypeException( mi, argIndex, e );
		}
		catch( ... )
		{
			throw;
		}
		CORAL_UNUSED( res );
	}

private:
	co::ITypeTransaction* checkInstance( const co::Any& any, co::IMember* member )
	{
		if( !member )
			throw co::IllegalArgumentException( "illegal null member info" );

		// make sure that 'any' is an instance of this type
		co::IInterface* myType = co::typeOf<co::ITypeTransaction>::get();

		co::ITypeTransaction* res;
		if( any.getKind() != co::TK_INTERFACE || !( res = dynamic_cast<co::ITypeTransaction*>( any.getState().data.itf ) ) )
			CORAL_THROW( co::IllegalArgumentException, "expected a valid co::ITypeTransaction*, but got " << any );

		// make sure that 'member' belongs to this type
		co::ICompositeType* owner = member->getOwner();
		if( owner != myType )
			CORAL_THROW( co::IllegalArgumentException, "member '" << member->getName() << "' belongs to "
				<< owner->getFullName() << ", not to co.ITypeTransaction" );

		return res;
	}
};

// ------ Reflector Creation Function ------ //

co::IReflector* __createITypeTransactionReflector()
{
    return new ITypeTransaction_Reflector;
}

} // namespace co
