/*******************************************************************************
** Reflection code generated for type 'co.Reflector'
**
** Created: Fri Jul 02 00:14:15 2010
**      by: Coral Compiler version 0.1.0
**
** WARNING! All changes made in this file will be lost when recompiling!
********************************************************************************/

#include <co/Reflector.h>
#include <co/reserved/ReflectorBase.h>
#include <co/DynamicProxyHandler.h>
#include <co/AttributeInfo.h>
#include <co/Component.h>
#include <co/DynamicProxyHandler.h>
#include <co/MethodInfo.h>
#include <co/Type.h>
#include <co/MethodInfo.h>
#include <co/AttributeInfo.h>
#include <co/IllegalCastException.h>
#include <co/MissingInputException.h>
#include <co/IllegalArgumentException.h>
#include <co/UnsupportedOperationException.h>
#include <sstream>
#include <cassert>

namespace co {

// ------ Proxy Interface ------ //

class Reflector_Proxy : public co::Reflector
{
public:
	Reflector_Proxy( co::DynamicProxyHandler* handler ) : _handler( handler )
	{
		_cookie = _handler->registerProxyInterface( co::disambiguate<co::Interface, co::Reflector>( this ) );
	}

	virtual ~Reflector_Proxy()
	{
		// empty
	}

	// co::Interface Methods:

	co::InterfaceType* getInterfaceType() { return co::typeOf<co::Reflector>::get(); }
	co::Component* getInterfaceOwner() { return _handler->getInterfaceOwner(); }
	const std::string& getInterfaceName() { return _handler->getProxyInterfaceName( _cookie ); }
	void componentRetain() { _handler->componentRetain(); }
	void componentRelease() { _handler->componentRelease(); }

	// co::Reflector Methods:

	co::int32 getSize()
	{
		co::Any __res;
		_handler->handleGetAttribute( _cookie, getAttribInfo<co::Reflector>( 0 ), __res );
        return __res.get< co::int32 >();
	}

	co::Type* getType()
	{
		co::Any __res;
		_handler->handleGetAttribute( _cookie, getAttribInfo<co::Reflector>( 1 ), __res );
        return __res.get< co::Type* >();
	}

	void getAttribute( const co::Any& instance, co::AttributeInfo* ai, co::Any& value )
	{
		co::Any __res, __arg[3];
		__arg[0].set< const co::Any& >( instance );
		__arg[1].set< co::AttributeInfo* >( ai );
		__arg[2].set< co::Any& >( value );
		co::ArrayRange<co::Any const> __ar( __arg, 3 );
		_handler->handleMethodInvocation( _cookie, getMethodInfo<co::Reflector>( 2 ), __ar, __res );
	}

	void invokeMethod( const co::Any& instance, co::MethodInfo* mi, co::ArrayRange<co::Any const> args, co::Any& returnValue )
	{
		co::Any __res, __arg[4];
		__arg[0].set< const co::Any& >( instance );
		__arg[1].set< co::MethodInfo* >( mi );
		__arg[2].set< co::ArrayRange<co::Any const> >( args );
		__arg[3].set< co::Any& >( returnValue );
		co::ArrayRange<co::Any const> __ar( __arg, 4 );
		_handler->handleMethodInvocation( _cookie, getMethodInfo<co::Reflector>( 3 ), __ar, __res );
	}

	co::Component* newInstance( )
	{
		co::Any __res;
		co::ArrayRange<co::Any const> __ar;
		_handler->handleMethodInvocation( _cookie, getMethodInfo<co::Reflector>( 4 ), __ar, __res );
		return __res.get< co::Component* >();
	}

	co::Interface* newProxy( co::DynamicProxyHandler* handler )
	{
		co::Any __res, __arg[1];
		__arg[0].set< co::DynamicProxyHandler* >( handler );
		co::ArrayRange<co::Any const> __ar( __arg, 1 );
		_handler->handleMethodInvocation( _cookie, getMethodInfo<co::Reflector>( 5 ), __ar, __res );
		return __res.get< co::Interface* >();
	}

	void setAttribute( const co::Any& instance, co::AttributeInfo* ai, const co::Any& value )
	{
		co::Any __res, __arg[3];
		__arg[0].set< const co::Any& >( instance );
		__arg[1].set< co::AttributeInfo* >( ai );
		__arg[2].set< const co::Any& >( value );
		co::ArrayRange<co::Any const> __ar( __arg, 3 );
		_handler->handleMethodInvocation( _cookie, getMethodInfo<co::Reflector>( 6 ), __ar, __res );
	}

	// These co::Reflector methods are not part of the reflection system:

	void createValue( void*, size_t )
	{
		throw co::UnsupportedOperationException( "co::Reflector::createValue() cannot be called through a proxy interface." );
	}

	void destroyValue( void* )
	{
		throw co::UnsupportedOperationException( "co::Reflector::destroyValue() cannot be called through a proxy interface." );
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

class Reflector_Reflector : public co::ReflectorBase
{
public:
	Reflector_Reflector()
	{
		// empty
	}

	virtual ~Reflector_Reflector()
	{
		// empty
	}

	co::Type* getType()
	{
		return co::typeOf<co::Reflector>::get();
	}

	co::int32 getSize()
	{
		return sizeof(co::Reflector);
	}

	co::Interface* newProxy( co::DynamicProxyHandler* handler )
	{
		checValidProxyHandler( handler );
		return co::disambiguate<co::Interface, co::Reflector>( new co::Reflector_Proxy( handler ) );
	}

	void getAttribute( const co::Any& instance, co::AttributeInfo* ai, co::Any& value )
	{
		co::Reflector* p = checkInstance( instance, ai );
		switch( ai->getIndex() )
		{
		case 0:		value.set< co::int32 >( p->getSize() ); break;
		case 1:		value.set< co::Type* >( p->getType() ); break;
		default:	raiseUnexpectedMemberIndex();
		}
	}

	void setAttribute( const co::Any& instance, co::AttributeInfo* ai, const co::Any& value )
	{
		co::Reflector* p = checkInstance( instance, ai );
		switch( ai->getIndex() )
		{
		case 0:		raiseAttributeIsReadOnly( ai ); break;
		case 1:		raiseAttributeIsReadOnly( ai ); break;
		default:	raiseUnexpectedMemberIndex();
		}
		CORAL_UNUSED( p );
		CORAL_UNUSED( value );
	}

	void invokeMethod( const co::Any& instance, co::MethodInfo* mi, co::ArrayRange<co::Any const> args, co::Any& __res )
	{
		co::Reflector* p = checkInstance( instance, mi );
		checkNumArguments( mi, args.getSize() );
		int argIndex = -1;
		try
		{
			switch( mi->getIndex() )
			{
			case 2:
				{
					const co::Any& instance = args[++argIndex].get< const co::Any& >();
					co::AttributeInfo* ai = args[++argIndex].get< co::AttributeInfo* >();
					co::Any& value = args[++argIndex].get< co::Any& >();
					argIndex = -1;
					p->getAttribute( instance, ai, value );
				}
				break;
			case 3:
				{
					const co::Any& instance = args[++argIndex].get< const co::Any& >();
					co::MethodInfo* mi = args[++argIndex].get< co::MethodInfo* >();
					co::ArrayRange<co::Any const> args = args[++argIndex].get< co::ArrayRange<co::Any const> >();
					co::Any& returnValue = args[++argIndex].get< co::Any& >();
					argIndex = -1;
					p->invokeMethod( instance, mi, args, returnValue );
				}
				break;
			case 4:
				{
					__res.set< co::Component* >( p->newInstance() );
				}
				break;
			case 5:
				{
					co::DynamicProxyHandler* handler = args[++argIndex].get< co::DynamicProxyHandler* >();
					argIndex = -1;
					__res.set< co::Interface* >( p->newProxy( handler ) );
				}
				break;
			case 6:
				{
					const co::Any& instance = args[++argIndex].get< const co::Any& >();
					co::AttributeInfo* ai = args[++argIndex].get< co::AttributeInfo* >();
					const co::Any& value = args[++argIndex].get< const co::Any& >();
					argIndex = -1;
					p->setAttribute( instance, ai, value );
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
	co::Reflector* checkInstance( const co::Any& instance, co::MemberInfo* member )
	{
		if( !member )
			throw co::IllegalArgumentException( "illegal null member info" );

		co::InterfaceType* myType = co::typeOf<co::Reflector>::get();

		// make sure that 'instance' is an instance of this type
		if( instance.getKind() != co::TK_INTERFACE ||
			!instance.getInterfaceType()->isSubTypeOf( myType ) ||
			instance.getState().data.ptr == NULL )
			CORAL_THROW( co::IllegalArgumentException, "expected a valid co::Reflector*, but got " << instance );

		// make sure that 'member' belongs to this type
		co::CompoundType* owner = member->getOwner();
		if( owner != myType )
			CORAL_THROW( co::IllegalArgumentException, "member '" << member->getName() << "' belongs to "
				<< owner->getFullName() << ", not to " << myType->getFullName() );

		return reinterpret_cast<co::Reflector*>( instance.getState().data.ptr );
	}
};

// ------ Reflector Creation Function ------ //

co::Reflector* __createReflectorReflector()
{
    return new Reflector_Reflector;
}

} // namespace co
