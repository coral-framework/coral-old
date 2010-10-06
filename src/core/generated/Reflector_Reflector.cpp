/*
 * Coral - A C++ Component Framework.
 * See Copyright Notice in Coral.h
 */

#include <co/Reflector.h>
#include <co/AttributeInfo.h>
#include <co/MethodInfo.h>
#include <co/Component.h>
#include <co/Type.h>
#include <co/DynamicProxyHandler.h>
#include <co/IllegalCastException.h>
#include <co/MissingInputException.h>
#include <co/IllegalArgumentException.h>
#include <co/UnsupportedOperationException.h>
#include <co/reserved/ReflectorBase.h>
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

	// co.Reflector Methods:

	co::int32 getSize()
	{
		const co::Any& res = _handler->handleGetAttribute( _cookie, getAttribInfo<co::Reflector>( 0 ) );
        return res.get< co::int32 >();
	}

	co::Type* getType()
	{
		const co::Any& res = _handler->handleGetAttribute( _cookie, getAttribInfo<co::Reflector>( 1 ) );
        return res.get< co::Type* >();
	}

	void getAttribute( const co::Any& instance_, co::AttributeInfo* ai_, co::Any& value_ )
	{
		co::Any args[3];
		args[0].set< const co::Any& >( instance_ );
		args[1].set< co::AttributeInfo* >( ai_ );
		args[2].set< co::Any& >( value_ );
		co::ArrayRange<co::Any const> range( args, 3 );
		_handler->handleMethodInvocation( _cookie, getMethodInfo<co::Reflector>( 2 ), range );
	}

	void invokeMethod( const co::Any& instance_, co::MethodInfo* mi_, co::ArrayRange<co::Any const> args_, co::Any& returnValue_ )
	{
		co::Any args[4];
		args[0].set< const co::Any& >( instance_ );
		args[1].set< co::MethodInfo* >( mi_ );
		args[2].set< co::ArrayRange<co::Any const> >( args_ );
		args[3].set< co::Any& >( returnValue_ );
		co::ArrayRange<co::Any const> range( args, 4 );
		_handler->handleMethodInvocation( _cookie, getMethodInfo<co::Reflector>( 3 ), range );
	}

	co::Component* newInstance()
	{
		co::ArrayRange<co::Any const> range;
		const co::Any& res = _handler->handleMethodInvocation( _cookie, getMethodInfo<co::Reflector>( 4 ), range );
		return res.get< co::Component* >();
	}

	co::Interface* newProxy( co::DynamicProxyHandler* handler_ )
	{
		co::Any args[1];
		args[0].set< co::DynamicProxyHandler* >( handler_ );
		co::ArrayRange<co::Any const> range( args, 1 );
		const co::Any& res = _handler->handleMethodInvocation( _cookie, getMethodInfo<co::Reflector>( 5 ), range );
		return res.get< co::Interface* >();
	}

	void setAttribute( const co::Any& instance_, co::AttributeInfo* ai_, const co::Any& value_ )
	{
		co::Any args[3];
		args[0].set< const co::Any& >( instance_ );
		args[1].set< co::AttributeInfo* >( ai_ );
		args[2].set< const co::Any& >( value_ );
		co::ArrayRange<co::Any const> range( args, 3 );
		_handler->handleMethodInvocation( _cookie, getMethodInfo<co::Reflector>( 6 ), range );
	}

	// These co::Reflector methods are not part of the reflection system:

	void createValue( void*, size_t )
	{
		throw co::UnsupportedOperationException( "co::Reflector::createValue() cannot be called through a proxy interface." );
	}

	void copyValue( const void*, void* )
	{
		throw co::UnsupportedOperationException( "co::Reflector::copyValue() cannot be called through a proxy interface." );
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

	void invokeMethod( const co::Any& instance, co::MethodInfo* mi, co::ArrayRange<co::Any const> args, co::Any& res )
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
					const co::Any& instance_ = args[++argIndex].get< const co::Any& >();
					co::AttributeInfo* ai_ = args[++argIndex].get< co::AttributeInfo* >();
					co::Any& value_ = args[++argIndex].get< co::Any& >();
					argIndex = -1;
					p->getAttribute( instance_, ai_, value_ );
				}
				break;
			case 3:
				{
					const co::Any& instance_ = args[++argIndex].get< const co::Any& >();
					co::MethodInfo* mi_ = args[++argIndex].get< co::MethodInfo* >();
					co::ArrayRange<co::Any const> args_ = args[++argIndex].get< co::ArrayRange<co::Any const> >();
					co::Any& returnValue_ = args[++argIndex].get< co::Any& >();
					argIndex = -1;
					p->invokeMethod( instance_, mi_, args_, returnValue_ );
				}
				break;
			case 4:
				{
					res.set< co::Component* >( p->newInstance() );
				}
				break;
			case 5:
				{
					co::DynamicProxyHandler* handler_ = args[++argIndex].get< co::DynamicProxyHandler* >();
					argIndex = -1;
					res.set< co::Interface* >( p->newProxy( handler_ ) );
				}
				break;
			case 6:
				{
					const co::Any& instance_ = args[++argIndex].get< const co::Any& >();
					co::AttributeInfo* ai_ = args[++argIndex].get< co::AttributeInfo* >();
					const co::Any& value_ = args[++argIndex].get< const co::Any& >();
					argIndex = -1;
					p->setAttribute( instance_, ai_, value_ );
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
	co::Reflector* checkInstance( const co::Any& any, co::MemberInfo* member )
	{
		if( !member )
			throw co::IllegalArgumentException( "illegal null member info" );

		// make sure that 'any' is an instance of this type
		co::InterfaceType* myType = co::typeOf<co::Reflector>::get();

		co::Reflector* res;
		if( any.getKind() != co::TK_INTERFACE || !( res = dynamic_cast<co::Reflector*>( any.getState().data.itf ) ) )
			CORAL_THROW( co::IllegalArgumentException, "expected a valid co::Reflector*, but got " << any );

		// make sure that 'member' belongs to this type
		co::CompoundType* owner = member->getOwner();
		if( owner != myType )
			CORAL_THROW( co::IllegalArgumentException, "member '" << member->getName() << "' belongs to "
				<< owner->getFullName() << ", not to co.Reflector" );

		return res;
	}
};

// ------ Reflector Creation Function ------ //

co::Reflector* __createReflectorReflector()
{
    return new Reflector_Reflector;
}

} // namespace co
