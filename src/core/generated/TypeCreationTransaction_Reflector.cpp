/*******************************************************************************
** Reflection code generated for type 'co.TypeCreationTransaction'
**
** Created: Thu Jun 10 20:53:30 2010
**      by: Coral Compiler version 0.1.0
**
** WARNING! All changes made in this file will be lost when recompiling!
********************************************************************************/

#include <co/TypeCreationTransaction.h>
#include <co/reserved/ReflectorBase.h>
#include <co/DynamicProxyHandler.h>
#include <co/TypeBuilder.h>
#include <co/MethodInfo.h>
#include <co/AttributeInfo.h>
#include <co/IllegalCastException.h>
#include <co/MissingInputException.h>
#include <co/IllegalArgumentException.h>
#include <sstream>

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
		// empty destructor
	}

	// co::Interface Methods:

	co::InterfaceType* getInterfaceType() { return co::typeOf<co::TypeCreationTransaction>::get(); }
	co::Component* getInterfaceOwner() { return _handler->getInterfaceOwner(); }
	const std::string& getInterfaceName() { return _handler->getProxyInterfaceName( _cookie ); }
	void componentRetain() { _handler->componentRetain(); }
	void componentRelease() { _handler->componentRelease(); }

	// co::TypeCreationTransaction Methods:

	co::ArrayRange<co::TypeBuilder* const> getTypeBuilders()
	{
		co::Any __res;
		_handler->handleGetAttribute( _cookie, getAttribInfo<co::TypeCreationTransaction>( 0 ), __res );
        return __res.get< co::ArrayRange<co::TypeBuilder* const> >();
	}

	void commit( )
	{
		co::Any __res;
		co::ArrayRange<co::Any const> __ar;
		_handler->handleMethodInvocation( _cookie, getMethodInfo<co::TypeCreationTransaction>( 1 ), __ar, __res );
	}

	void rollback( )
	{
		co::Any __res;
		co::ArrayRange<co::Any const> __ar;
		_handler->handleMethodInvocation( _cookie, getMethodInfo<co::TypeCreationTransaction>( 2 ), __ar, __res );
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
		return co::disambiguate<co::Interface, co::TypeCreationTransaction>( new TypeCreationTransaction_Proxy( handler ) );
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

	void invokeMethod( const co::Any& instance, co::MethodInfo* mi, co::ArrayRange<co::Any const> args, co::Any& __res )
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
		CORAL_UNUSED( __res );
	}

private:
	co::TypeCreationTransaction* checkInstance( const co::Any& instance, co::MemberInfo* member )
	{
		if( !member )
			throw co::IllegalArgumentException( "illegal null member info" );

		co::InterfaceType* myType = co::typeOf<co::TypeCreationTransaction>::get();

		// make sure that 'instance' is an instance of this type
		if( instance.getKind() != co::TK_INTERFACE ||
			!instance.getInterfaceType()->isSubTypeOf( myType ) ||
			instance.getState().data.ptr == NULL )
			CORAL_THROW( co::IllegalArgumentException, "expected a valid co::TypeCreationTransaction*, but got " << instance );

		// make sure that 'member' belongs to this type
		co::CompoundType* owner = member->getOwner();
		if( owner != myType )
			CORAL_THROW( co::IllegalArgumentException, "member '" << member->getName() << "' belongs to "
				<< owner->getFullName() << ", not to " << myType->getFullName() );

		return reinterpret_cast<co::TypeCreationTransaction*>( instance.getState().data.ptr );
	}
};

// ------ Reflector Creation Function ------ //

namespace __coral {

co::Reflector* createTypeCreationTransactionReflector()
{
    return new TypeCreationTransaction_Reflector;
}

} // namespace __coral
