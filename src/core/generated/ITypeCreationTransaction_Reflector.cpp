/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#include <co/ITypeCreationTransaction.h>
#include <co/IDynamicProxyHandler.h>
#include <co/ITypeBuilder.h>
#include <co/IMethodInfo.h>
#include <co/IAttributeInfo.h>
#include <co/IllegalCastException.h>
#include <co/MissingInputException.h>
#include <co/IllegalArgumentException.h>
#include <co/reserved/ReflectorBase.h>
#include <sstream>
#include <cassert>

namespace co {

// ------ Proxy Interface ------ //

class ITypeCreationTransaction_Proxy : public co::ITypeCreationTransaction
{
public:
	ITypeCreationTransaction_Proxy( co::IDynamicProxyHandler* handler ) : _handler( handler )
	{
		_cookie = _handler->registerProxyInterface( co::disambiguate<co::Interface, co::ITypeCreationTransaction>( this ) );
	}

	virtual ~ITypeCreationTransaction_Proxy()
	{
		// empty
	}

	// co::Interface Methods:

	co::IInterfaceType* getInterfaceType() { return co::typeOf<co::ITypeCreationTransaction>::get(); }
	co::IComponent* getInterfaceOwner() { return _handler->getInterfaceOwner(); }
	const std::string& getInterfaceName() { return _handler->getProxyInterfaceName( _cookie ); }
	void componentRetain() { _handler->componentRetain(); }
	void componentRelease() { _handler->componentRelease(); }

	// co.ITypeCreationTransaction Methods:

	co::ArrayRange<co::ITypeBuilder* const> getTypeBuilders()
	{
		const co::Any& res = _handler->handleGetAttribute( _cookie, getAttribInfo<co::ITypeCreationTransaction>( 0 ) );
        return res.get< co::ArrayRange<co::ITypeBuilder* const> >();
	}

	void commit()
	{
		co::ArrayRange<co::Any const> range;
		_handler->handleMethodInvocation( _cookie, getMethodInfo<co::ITypeCreationTransaction>( 0 ), range );
	}

	void rollback()
	{
		co::ArrayRange<co::Any const> range;
		_handler->handleMethodInvocation( _cookie, getMethodInfo<co::ITypeCreationTransaction>( 1 ), range );
	}

protected:
	template<typename T>
	co::IAttributeInfo* getAttribInfo( co::uint32 index )
	{
		return co::typeOf<T>::get()->getMemberAttributes()[index];
	}

	template<typename T>
	co::IMethodInfo* getMethodInfo( co::uint32 index )
	{
		return co::typeOf<T>::get()->getMemberMethods()[index];
	}

private:
	co::IDynamicProxyHandler* _handler;
	co::uint32 _cookie;
};

// ------ IReflector ------ //

class ITypeCreationTransaction_Reflector : public co::ReflectorBase
{
public:
	ITypeCreationTransaction_Reflector()
	{
		// empty
	}

	virtual ~ITypeCreationTransaction_Reflector()
	{
		// empty
	}

	co::IType* getType()
	{
		return co::typeOf<co::ITypeCreationTransaction>::get();
	}

	co::int32 getSize()
	{
		return sizeof(co::ITypeCreationTransaction);
	}

	co::Interface* newProxy( co::IDynamicProxyHandler* handler )
	{
		checValidProxyHandler( handler );
		return co::disambiguate<co::Interface, co::ITypeCreationTransaction>( new co::ITypeCreationTransaction_Proxy( handler ) );
	}

	void getAttribute( const co::Any& instance, co::IAttributeInfo* ai, co::Any& value )
	{
		co::ITypeCreationTransaction* p = checkInstance( instance, ai );
		switch( ai->getIndex() )
		{
		case 0:		value.set< co::ArrayRange<co::ITypeBuilder* const> >( p->getTypeBuilders() ); break;
		default:	raiseUnexpectedMemberIndex();
		}
	}

	void setAttribute( const co::Any& instance, co::IAttributeInfo* ai, const co::Any& value )
	{
		co::ITypeCreationTransaction* p = checkInstance( instance, ai );
		switch( ai->getIndex() )
		{
		case 0:		raiseAttributeIsReadOnly( ai ); break;
		default:	raiseUnexpectedMemberIndex();
		}
		CORAL_UNUSED( p );
		CORAL_UNUSED( value );
	}

	void invokeMethod( const co::Any& instance, co::IMethodInfo* mi, co::ArrayRange<co::Any const> args, co::Any& res )
	{
		co::ITypeCreationTransaction* p = checkInstance( instance, mi );
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
	co::ITypeCreationTransaction* checkInstance( const co::Any& any, co::IMemberInfo* member )
	{
		if( !member )
			throw co::IllegalArgumentException( "illegal null member info" );

		// make sure that 'any' is an instance of this type
		co::IInterfaceType* myType = co::typeOf<co::ITypeCreationTransaction>::get();

		co::ITypeCreationTransaction* res;
		if( any.getKind() != co::TK_INTERFACE || !( res = dynamic_cast<co::ITypeCreationTransaction*>( any.getState().data.itf ) ) )
			CORAL_THROW( co::IllegalArgumentException, "expected a valid co::ITypeCreationTransaction*, but got " << any );

		// make sure that 'member' belongs to this type
		co::ICompoundType* owner = member->getOwner();
		if( owner != myType )
			CORAL_THROW( co::IllegalArgumentException, "member '" << member->getName() << "' belongs to "
				<< owner->getFullName() << ", not to co.ITypeCreationTransaction" );

		return res;
	}
};

// ------ IReflector Creation Function ------ //

co::IReflector* __createITypeCreationTransactionIReflector()
{
    return new ITypeCreationTransaction_Reflector;
}

} // namespace co
