/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#include <co/TypeCreationTransaction.h>
#include <co/DynamicProxyHandler.h>
#include <co/TypeBuilder.h>
#include <co/MethodInfo.h>
#include <co/AttributeInfo.h>
#include <co/IllegalCastException.h>
#include <co/MissingInputException.h>
#include <co/IllegalArgumentException.h>
#include <co/reserved/ReflectorBase.h>
#include <sstream>
#include <cassert>

namespace co {

// ------ Proxy Interface ------ //

class TypeCreationTransaction_Proxy : public co::TypeCreationTransaction
{
public:
	TypeCreationTransaction_Proxy( co::DynamicProxyHandler* handler ) : _handler( handler )
	{
		_cookie = _handler->registerProxyInterface( co::disambiguate<co::Interface, co::TypeCreationTransaction>( this ) );
	}

	virtual ~TypeCreationTransaction_Proxy()
	{
		// empty
	}

	// co::Interface Methods:

	co::InterfaceType* getInterfaceType() { return co::typeOf<co::TypeCreationTransaction>::get(); }
	co::Component* getInterfaceOwner() { return _handler->getInterfaceOwner(); }
	const std::string& getInterfaceName() { return _handler->getProxyInterfaceName( _cookie ); }
	void componentRetain() { _handler->componentRetain(); }
	void componentRelease() { _handler->componentRelease(); }

	// co.TypeCreationTransaction Methods:

	co::ArrayRange<co::TypeBuilder* const> getTypeBuilders()
	{
		const co::Any& res = _handler->handleGetAttribute( _cookie, getAttribInfo<co::TypeCreationTransaction>( 0 ) );
        return res.get< co::ArrayRange<co::TypeBuilder* const> >();
	}

	void commit()
	{
		co::ArrayRange<co::Any const> range;
		_handler->handleMethodInvocation( _cookie, getMethodInfo<co::TypeCreationTransaction>( 0 ), range );
	}

	void rollback()
	{
		co::ArrayRange<co::Any const> range;
		_handler->handleMethodInvocation( _cookie, getMethodInfo<co::TypeCreationTransaction>( 1 ), range );
	}

protected:
	template<typename T>
	co::AttributeInfo* getAttribInfo( co::uint32 index )
	{
		return co::typeOf<T>::get()->getMemberAttributes()[index];
	}

	template<typename T>
	co::MethodInfo* getMethodInfo( co::uint32 index )
	{
		return co::typeOf<T>::get()->getMemberMethods()[index];
	}

private:
	co::DynamicProxyHandler* _handler;
	co::uint32 _cookie;
};

// ------ Reflector ------ //

class TypeCreationTransaction_Reflector : public co::ReflectorBase
{
public:
	TypeCreationTransaction_Reflector()
	{
		// empty
	}

	virtual ~TypeCreationTransaction_Reflector()
	{
		// empty
	}

	co::Type* getType()
	{
		return co::typeOf<co::TypeCreationTransaction>::get();
	}

	co::int32 getSize()
	{
		return sizeof(co::TypeCreationTransaction);
	}

	co::Interface* newProxy( co::DynamicProxyHandler* handler )
	{
		checValidProxyHandler( handler );
		return co::disambiguate<co::Interface, co::TypeCreationTransaction>( new co::TypeCreationTransaction_Proxy( handler ) );
	}

	void getAttribute( const co::Any& instance, co::AttributeInfo* ai, co::Any& value )
	{
		co::TypeCreationTransaction* p = checkInstance( instance, ai );
		switch( ai->getIndex() )
		{
		case 0:		value.set< co::ArrayRange<co::TypeBuilder* const> >( p->getTypeBuilders() ); break;
		default:	raiseUnexpectedMemberIndex();
		}
	}

	void setAttribute( const co::Any& instance, co::AttributeInfo* ai, const co::Any& value )
	{
		co::TypeCreationTransaction* p = checkInstance( instance, ai );
		switch( ai->getIndex() )
		{
		case 0:		raiseAttributeIsReadOnly( ai ); break;
		default:	raiseUnexpectedMemberIndex();
		}
		CORAL_UNUSED( p );
		CORAL_UNUSED( value );
	}

	void invokeMethod( const co::Any& instance, co::MethodInfo* mi, co::ArrayRange<co::Any const> args, co::Any& res )
	{
		co::TypeCreationTransaction* p = checkInstance( instance, mi );
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
	co::TypeCreationTransaction* checkInstance( const co::Any& any, co::MemberInfo* member )
	{
		if( !member )
			throw co::IllegalArgumentException( "illegal null member info" );

		// make sure that 'any' is an instance of this type
		co::InterfaceType* myType = co::typeOf<co::TypeCreationTransaction>::get();

		co::TypeCreationTransaction* res;
		if( any.getKind() != co::TK_INTERFACE || !( res = dynamic_cast<co::TypeCreationTransaction*>( any.getState().data.itf ) ) )
			CORAL_THROW( co::IllegalArgumentException, "expected a valid co::TypeCreationTransaction*, but got " << any );

		// make sure that 'member' belongs to this type
		co::CompoundType* owner = member->getOwner();
		if( owner != myType )
			CORAL_THROW( co::IllegalArgumentException, "member '" << member->getName() << "' belongs to "
				<< owner->getFullName() << ", not to co.TypeCreationTransaction" );

		return res;
	}
};

// ------ Reflector Creation Function ------ //

co::Reflector* __createTypeCreationTransactionReflector()
{
    return new TypeCreationTransaction_Reflector;
}

} // namespace co
