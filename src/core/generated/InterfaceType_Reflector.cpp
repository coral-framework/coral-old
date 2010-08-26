/*******************************************************************************
** Reflection code generated for type 'co.InterfaceType'
**
** Created: Wed Aug 25 16:31:31 2010
**      by: Coral Compiler version 0.1.0
**
** WARNING! All changes made in this file will be lost when recompiling!
********************************************************************************/

#include <co/InterfaceType.h>
#include <co/reserved/ReflectorBase.h>
#include <co/DynamicProxyHandler.h>
#include <co/AttributeInfo.h>
#include <co/MemberInfo.h>
#include <co/MethodInfo.h>
#include <co/Namespace.h>
#include <co/Reflector.h>
#include <co/Uuid.h>
#include <co/MethodInfo.h>
#include <co/AttributeInfo.h>
#include <co/IllegalCastException.h>
#include <co/MissingInputException.h>
#include <co/IllegalArgumentException.h>
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

	// co::CompoundType Methods:

	co::ArrayRange<co::MemberInfo* const> getMembers()
	{
		co::Any __res;
		_handler->handleGetAttribute( _cookie, getAttribInfo<co::CompoundType>( 0 ), __res );
        return __res.get< co::ArrayRange<co::MemberInfo* const> >();
	}

	co::MemberInfo* getMember( const std::string& name )
	{
		co::Any __res, __arg[1];
		__arg[0].set< const std::string& >( name );
		co::ArrayRange<co::Any const> __ar( __arg, 1 );
		_handler->handleMethodInvocation( _cookie, getMethodInfo<co::CompoundType>( 1 ), __ar, __res );
		return __res.get< co::MemberInfo* >();
	}

	// co::Type Methods:

	const co::Uuid& getBinarySignature()
	{
		co::Any __res;
		_handler->handleGetAttribute( _cookie, getAttribInfo<co::Type>( 0 ), __res );
        return __res.get< const co::Uuid& >();
	}

	const std::string& getFullName()
	{
		co::Any __res;
		_handler->handleGetAttribute( _cookie, getAttribInfo<co::Type>( 1 ), __res );
        return __res.get< const std::string& >();
	}

	const co::Uuid& getFullSignature()
	{
		co::Any __res;
		_handler->handleGetAttribute( _cookie, getAttribInfo<co::Type>( 2 ), __res );
        return __res.get< const co::Uuid& >();
	}

	co::TypeKind getKind()
	{
		co::Any __res;
		_handler->handleGetAttribute( _cookie, getAttribInfo<co::Type>( 3 ), __res );
        return __res.get< co::TypeKind >();
	}

	const std::string& getName()
	{
		co::Any __res;
		_handler->handleGetAttribute( _cookie, getAttribInfo<co::Type>( 4 ), __res );
        return __res.get< const std::string& >();
	}

	co::Namespace* getNamespace()
	{
		co::Any __res;
		_handler->handleGetAttribute( _cookie, getAttribInfo<co::Type>( 5 ), __res );
        return __res.get< co::Namespace* >();
	}

	co::Reflector* getReflector()
	{
		co::Any __res;
		_handler->handleGetAttribute( _cookie, getAttribInfo<co::Type>( 6 ), __res );
        return __res.get< co::Reflector* >();
	}

	void setReflector( co::Reflector* reflector )
	{
		co::Any __arg;
		__arg.set< co::Reflector* >( reflector );
		_handler->handleSetAttribute( _cookie, getAttribInfo<co::Type>( 6 ), __arg );
	}

	// co::MethodContainer Methods:

	co::ArrayRange<co::MethodInfo* const> getMemberMethods()
	{
		co::Any __res;
		_handler->handleGetAttribute( _cookie, getAttribInfo<co::MethodContainer>( 0 ), __res );
        return __res.get< co::ArrayRange<co::MethodInfo* const> >();
	}

	// co::AttributeContainer Methods:

	co::ArrayRange<co::AttributeInfo* const> getMemberAttributes()
	{
		co::Any __res;
		_handler->handleGetAttribute( _cookie, getAttribInfo<co::AttributeContainer>( 0 ), __res );
        return __res.get< co::ArrayRange<co::AttributeInfo* const> >();
	}

	// co::InterfaceType Methods:

	co::ArrayRange<co::InterfaceType* const> getInterfaceAncestors()
	{
		co::Any __res;
		_handler->handleGetAttribute( _cookie, getAttribInfo<co::InterfaceType>( 0 ), __res );
        return __res.get< co::ArrayRange<co::InterfaceType* const> >();
	}

	co::ArrayRange<co::InterfaceType* const> getSubInterfaces()
	{
		co::Any __res;
		_handler->handleGetAttribute( _cookie, getAttribInfo<co::InterfaceType>( 1 ), __res );
        return __res.get< co::ArrayRange<co::InterfaceType* const> >();
	}

	co::ArrayRange<co::InterfaceType* const> getSuperInterfaces()
	{
		co::Any __res;
		_handler->handleGetAttribute( _cookie, getAttribInfo<co::InterfaceType>( 2 ), __res );
        return __res.get< co::ArrayRange<co::InterfaceType* const> >();
	}

	bool isSubTypeOf( co::InterfaceType* itf )
	{
		co::Any __res, __arg[1];
		__arg[0].set< co::InterfaceType* >( itf );
		co::ArrayRange<co::Any const> __ar( __arg, 1 );
		_handler->handleMethodInvocation( _cookie, getMethodInfo<co::InterfaceType>( 3 ), __ar, __res );
		return __res.get< bool >();
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
		case 0:		value.set< co::ArrayRange<co::InterfaceType* const> >( p->getInterfaceAncestors() ); break;
		case 1:		value.set< co::ArrayRange<co::InterfaceType* const> >( p->getSubInterfaces() ); break;
		case 2:		value.set< co::ArrayRange<co::InterfaceType* const> >( p->getSuperInterfaces() ); break;
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
		default:	raiseUnexpectedMemberIndex();
		}
		CORAL_UNUSED( p );
		CORAL_UNUSED( value );
	}

	void invokeMethod( const co::Any& instance, co::MethodInfo* mi, co::ArrayRange<co::Any const> args, co::Any& __res )
	{
		co::InterfaceType* p = checkInstance( instance, mi );
		checkNumArguments( mi, args.getSize() );
		int argIndex = -1;
		try
		{
			switch( mi->getIndex() )
			{
			case 3:
				{
					co::InterfaceType* itf = args[++argIndex].get< co::InterfaceType* >();
					argIndex = -1;
					__res.set< bool >( p->isSubTypeOf( itf ) );
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
	co::InterfaceType* checkInstance( const co::Any& instance, co::MemberInfo* member )
	{
		if( !member )
			throw co::IllegalArgumentException( "illegal null member info" );

		co::InterfaceType* myType = co::typeOf<co::InterfaceType>::get();

		// make sure that 'instance' is an instance of this type
		if( instance.getKind() != co::TK_INTERFACE ||
			!instance.getInterfaceType()->isSubTypeOf( myType ) ||
			instance.getState().data.ptr == NULL )
			CORAL_THROW( co::IllegalArgumentException, "expected a valid co::InterfaceType*, but got " << instance );

		// make sure that 'member' belongs to this type
		co::CompoundType* owner = member->getOwner();
		if( owner != myType )
			CORAL_THROW( co::IllegalArgumentException, "member '" << member->getName() << "' belongs to "
				<< owner->getFullName() << ", not to " << myType->getFullName() );

		return reinterpret_cast<co::InterfaceType*>( instance.getState().data.ptr );
	}
};

// ------ Reflector Creation Function ------ //

co::Reflector* __createInterfaceTypeReflector()
{
    return new InterfaceType_Reflector;
}

} // namespace co
