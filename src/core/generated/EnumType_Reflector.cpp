/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#include <co/EnumType.h>
#include <co/DynamicProxyHandler.h>
#include <co/Reflector.h>
#include <co/Namespace.h>
#include <co/Uuid.h>
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

class EnumType_Proxy : public co::EnumType
{
public:
	EnumType_Proxy( co::DynamicProxyHandler* handler ) : _handler( handler )
	{
		_cookie = _handler->registerProxyInterface( co::disambiguate<co::Interface, co::EnumType>( this ) );
	}

	virtual ~EnumType_Proxy()
	{
		// empty
	}

	// co::Interface Methods:

	co::InterfaceType* getInterfaceType() { return co::typeOf<co::EnumType>::get(); }
	co::Component* getInterfaceOwner() { return _handler->getInterfaceOwner(); }
	const std::string& getInterfaceName() { return _handler->getProxyInterfaceName( _cookie ); }
	void componentRetain() { _handler->componentRetain(); }
	void componentRelease() { _handler->componentRelease(); }

	// co.Type Methods:

	const co::Uuid& getBinarySignature()
	{
		const co::Any& res = _handler->handleGetAttribute( _cookie, getAttribInfo<co::Type>( 0 ) );
        return res.get< const co::Uuid& >();
	}

	const std::string& getFullName()
	{
		const co::Any& res = _handler->handleGetAttribute( _cookie, getAttribInfo<co::Type>( 1 ) );
        return res.get< const std::string& >();
	}

	const co::Uuid& getFullSignature()
	{
		const co::Any& res = _handler->handleGetAttribute( _cookie, getAttribInfo<co::Type>( 2 ) );
        return res.get< const co::Uuid& >();
	}

	co::TypeKind getKind()
	{
		const co::Any& res = _handler->handleGetAttribute( _cookie, getAttribInfo<co::Type>( 3 ) );
        return res.get< co::TypeKind >();
	}

	const std::string& getName()
	{
		const co::Any& res = _handler->handleGetAttribute( _cookie, getAttribInfo<co::Type>( 4 ) );
        return res.get< const std::string& >();
	}

	co::Namespace* getNamespace()
	{
		const co::Any& res = _handler->handleGetAttribute( _cookie, getAttribInfo<co::Type>( 5 ) );
        return res.get< co::Namespace* >();
	}

	co::Reflector* getReflector()
	{
		const co::Any& res = _handler->handleGetAttribute( _cookie, getAttribInfo<co::Type>( 6 ) );
        return res.get< co::Reflector* >();
	}

	void setReflector( co::Reflector* reflector_ )
	{
		co::Any arg;
		arg.set< co::Reflector* >( reflector_ );
		_handler->handleSetAttribute( _cookie, getAttribInfo<co::Type>( 6 ), arg );
	}

	// co.EnumType Methods:

	co::ArrayRange<std::string const> getIdentifiers()
	{
		const co::Any& res = _handler->handleGetAttribute( _cookie, getAttribInfo<co::EnumType>( 0 ) );
        return res.get< co::ArrayRange<std::string const> >();
	}

	co::int32 getValueOf( const std::string& identifier_ )
	{
		co::Any args[1];
		args[0].set< const std::string& >( identifier_ );
		co::ArrayRange<co::Any const> range( args, 1 );
		const co::Any& res = _handler->handleMethodInvocation( _cookie, getMethodInfo<co::EnumType>( 0 ), range );
		return res.get< co::int32 >();
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

class EnumType_Reflector : public co::ReflectorBase
{
public:
	EnumType_Reflector()
	{
		// empty
	}

	virtual ~EnumType_Reflector()
	{
		// empty
	}

	co::Type* getType()
	{
		return co::typeOf<co::EnumType>::get();
	}

	co::int32 getSize()
	{
		return sizeof(co::EnumType);
	}

	co::Interface* newProxy( co::DynamicProxyHandler* handler )
	{
		checValidProxyHandler( handler );
		return co::disambiguate<co::Interface, co::EnumType>( new co::EnumType_Proxy( handler ) );
	}

	void getAttribute( const co::Any& instance, co::AttributeInfo* ai, co::Any& value )
	{
		co::EnumType* p = checkInstance( instance, ai );
		switch( ai->getIndex() )
		{
		case 0:		value.set< co::ArrayRange<std::string const> >( p->getIdentifiers() ); break;
		default:	raiseUnexpectedMemberIndex();
		}
	}

	void setAttribute( const co::Any& instance, co::AttributeInfo* ai, const co::Any& value )
	{
		co::EnumType* p = checkInstance( instance, ai );
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
		co::EnumType* p = checkInstance( instance, mi );
		checkNumArguments( mi, args.getSize() );
		int argIndex = -1;
		try
		{
			switch( mi->getIndex() )
			{
			case 1:
				{
					const std::string& identifier_ = args[++argIndex].get< const std::string& >();
					argIndex = -1;
					res.set< co::int32 >( p->getValueOf( identifier_ ) );
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
	co::EnumType* checkInstance( const co::Any& any, co::MemberInfo* member )
	{
		if( !member )
			throw co::IllegalArgumentException( "illegal null member info" );

		// make sure that 'any' is an instance of this type
		co::InterfaceType* myType = co::typeOf<co::EnumType>::get();

		co::EnumType* res;
		if( any.getKind() != co::TK_INTERFACE || !( res = dynamic_cast<co::EnumType*>( any.getState().data.itf ) ) )
			CORAL_THROW( co::IllegalArgumentException, "expected a valid co::EnumType*, but got " << any );

		// make sure that 'member' belongs to this type
		co::CompoundType* owner = member->getOwner();
		if( owner != myType )
			CORAL_THROW( co::IllegalArgumentException, "member '" << member->getName() << "' belongs to "
				<< owner->getFullName() << ", not to co.EnumType" );

		return res;
	}
};

// ------ Reflector Creation Function ------ //

co::Reflector* __createEnumTypeReflector()
{
    return new EnumType_Reflector;
}

} // namespace co
