/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#include <co/InterfaceType.h>
#include <co/DynamicProxyHandler.h>
#include <co/Reflector.h>
#include <co/Namespace.h>
#include <co/AttributeInfo.h>
#include <co/MethodInfo.h>
#include <co/MemberInfo.h>
#include <co/Uuid.h>
#include <co/IllegalCastException.h>
#include <co/MissingInputException.h>
#include <co/IllegalArgumentException.h>
#include <co/reserved/ReflectorBase.h>
#include <sstream>
#include <cassert>

namespace co {

// ------ Proxy Interface ------ //

class InterfaceType_Proxy : public co::InterfaceType
{
public:
	InterfaceType_Proxy( co::DynamicProxyHandler* handler ) : _handler( handler )
	{
		_cookie = _handler->registerProxyInterface( co::disambiguate<co::Interface, co::InterfaceType>( this ) );
	}

	virtual ~InterfaceType_Proxy()
	{
		// empty
	}

	// co::Interface Methods:

	co::InterfaceType* getInterfaceType() { return co::typeOf<co::InterfaceType>::get(); }
	co::Component* getInterfaceOwner() { return _handler->getInterfaceOwner(); }
	const std::string& getInterfaceName() { return _handler->getProxyInterfaceName( _cookie ); }
	void componentRetain() { _handler->componentRetain(); }
	void componentRelease() { _handler->componentRelease(); }

	// co.MethodContainer Methods:

	co::ArrayRange<co::MethodInfo* const> getMemberMethods()
	{
		const co::Any& res = _handler->handleGetAttribute( _cookie, getAttribInfo<co::MethodContainer>( 0 ) );
        return res.get< co::ArrayRange<co::MethodInfo* const> >();
	}

	// co.CompoundType Methods:

	co::ArrayRange<co::MemberInfo* const> getMembers()
	{
		const co::Any& res = _handler->handleGetAttribute( _cookie, getAttribInfo<co::CompoundType>( 0 ) );
        return res.get< co::ArrayRange<co::MemberInfo* const> >();
	}

	co::MemberInfo* getMember( const std::string& name_ )
	{
		co::Any args[1];
		args[0].set< const std::string& >( name_ );
		co::ArrayRange<co::Any const> range( args, 1 );
		const co::Any& res = _handler->handleMethodInvocation( _cookie, getMethodInfo<co::CompoundType>( 0 ), range );
		return res.get< co::MemberInfo* >();
	}

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

	// co.AttributeContainer Methods:

	co::ArrayRange<co::AttributeInfo* const> getMemberAttributes()
	{
		const co::Any& res = _handler->handleGetAttribute( _cookie, getAttribInfo<co::AttributeContainer>( 0 ) );
        return res.get< co::ArrayRange<co::AttributeInfo* const> >();
	}

	// co.InterfaceType Methods:

	const std::string& getCppBlock()
	{
		const co::Any& res = _handler->handleGetAttribute( _cookie, getAttribInfo<co::InterfaceType>( 0 ) );
        return res.get< const std::string& >();
	}

	co::ArrayRange<co::InterfaceType* const> getInterfaceAncestors()
	{
		const co::Any& res = _handler->handleGetAttribute( _cookie, getAttribInfo<co::InterfaceType>( 1 ) );
        return res.get< co::ArrayRange<co::InterfaceType* const> >();
	}

	co::ArrayRange<co::InterfaceType* const> getSubInterfaces()
	{
		const co::Any& res = _handler->handleGetAttribute( _cookie, getAttribInfo<co::InterfaceType>( 2 ) );
        return res.get< co::ArrayRange<co::InterfaceType* const> >();
	}

	co::ArrayRange<co::InterfaceType* const> getSuperInterfaces()
	{
		const co::Any& res = _handler->handleGetAttribute( _cookie, getAttribInfo<co::InterfaceType>( 3 ) );
        return res.get< co::ArrayRange<co::InterfaceType* const> >();
	}

	bool isSubTypeOf( co::InterfaceType* itf_ )
	{
		co::Any args[1];
		args[0].set< co::InterfaceType* >( itf_ );
		co::ArrayRange<co::Any const> range( args, 1 );
		const co::Any& res = _handler->handleMethodInvocation( _cookie, getMethodInfo<co::InterfaceType>( 0 ), range );
		return res.get< bool >();
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

class InterfaceType_Reflector : public co::ReflectorBase
{
public:
	InterfaceType_Reflector()
	{
		// empty
	}

	virtual ~InterfaceType_Reflector()
	{
		// empty
	}

	co::Type* getType()
	{
		return co::typeOf<co::InterfaceType>::get();
	}

	co::int32 getSize()
	{
		return sizeof(co::InterfaceType);
	}

	co::Interface* newProxy( co::DynamicProxyHandler* handler )
	{
		checValidProxyHandler( handler );
		return co::disambiguate<co::Interface, co::InterfaceType>( new co::InterfaceType_Proxy( handler ) );
	}

	void getAttribute( const co::Any& instance, co::AttributeInfo* ai, co::Any& value )
	{
		co::InterfaceType* p = checkInstance( instance, ai );
		switch( ai->getIndex() )
		{
		case 0:		value.set< const std::string& >( p->getCppBlock() ); break;
		case 1:		value.set< co::ArrayRange<co::InterfaceType* const> >( p->getInterfaceAncestors() ); break;
		case 2:		value.set< co::ArrayRange<co::InterfaceType* const> >( p->getSubInterfaces() ); break;
		case 3:		value.set< co::ArrayRange<co::InterfaceType* const> >( p->getSuperInterfaces() ); break;
		default:	raiseUnexpectedMemberIndex();
		}
	}

	void setAttribute( const co::Any& instance, co::AttributeInfo* ai, const co::Any& value )
	{
		co::InterfaceType* p = checkInstance( instance, ai );
		switch( ai->getIndex() )
		{
		case 0:		raiseAttributeIsReadOnly( ai ); break;
		case 1:		raiseAttributeIsReadOnly( ai ); break;
		case 2:		raiseAttributeIsReadOnly( ai ); break;
		case 3:		raiseAttributeIsReadOnly( ai ); break;
		default:	raiseUnexpectedMemberIndex();
		}
		CORAL_UNUSED( p );
		CORAL_UNUSED( value );
	}

	void invokeMethod( const co::Any& instance, co::MethodInfo* mi, co::ArrayRange<co::Any const> args, co::Any& res )
	{
		co::InterfaceType* p = checkInstance( instance, mi );
		checkNumArguments( mi, args.getSize() );
		int argIndex = -1;
		try
		{
			switch( mi->getIndex() )
			{
			case 4:
				{
					co::InterfaceType* itf_ = args[++argIndex].get< co::InterfaceType* >();
					argIndex = -1;
					res.set< bool >( p->isSubTypeOf( itf_ ) );
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
	co::InterfaceType* checkInstance( const co::Any& any, co::MemberInfo* member )
	{
		if( !member )
			throw co::IllegalArgumentException( "illegal null member info" );

		// make sure that 'any' is an instance of this type
		co::InterfaceType* myType = co::typeOf<co::InterfaceType>::get();

		co::InterfaceType* res;
		if( any.getKind() != co::TK_INTERFACE || !( res = dynamic_cast<co::InterfaceType*>( any.getState().data.itf ) ) )
			CORAL_THROW( co::IllegalArgumentException, "expected a valid co::InterfaceType*, but got " << any );

		// make sure that 'member' belongs to this type
		co::CompoundType* owner = member->getOwner();
		if( owner != myType )
			CORAL_THROW( co::IllegalArgumentException, "member '" << member->getName() << "' belongs to "
				<< owner->getFullName() << ", not to co.InterfaceType" );

		return res;
	}
};

// ------ Reflector Creation Function ------ //

co::Reflector* __createInterfaceTypeReflector()
{
    return new InterfaceType_Reflector;
}

} // namespace co
