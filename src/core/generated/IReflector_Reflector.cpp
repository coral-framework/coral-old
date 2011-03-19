/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#include <co/IReflector.h>
#include <co/IType.h>
#include <co/IDynamicProxyHandler.h>
#include <co/IAttributeInfo.h>
#include <co/IMethodInfo.h>
#include <co/IComponent.h>
#include <co/IllegalCastException.h>
#include <co/MissingInputException.h>
#include <co/IllegalArgumentException.h>
#include <co/NotSupportedException.h>
#include <co/reserved/ReflectorBase.h>
#include <sstream>
#include <cassert>

namespace co {

// ------ Proxy Interface ------ //

class IReflector_Proxy : public co::IReflector
{
public:
	IReflector_Proxy( co::IDynamicProxyHandler* handler ) : _handler( handler )
	{
		_cookie = _handler->registerProxyInterface( co::disambiguate<co::Interface, co::IReflector>( this ) );
	}

	virtual ~IReflector_Proxy()
	{
		// empty
	}

	// co::Interface Methods:

	co::IInterfaceType* getInterfaceType() { return co::typeOf<co::IReflector>::get(); }
	co::IComponent* getInterfaceOwner() { return _handler->getInterfaceOwner(); }
	const std::string& getInterfaceName() { return _handler->getProxyInterfaceName( _cookie ); }
	void componentRetain() { _handler->componentRetain(); }
	void componentRelease() { _handler->componentRelease(); }

	// co.IReflector Methods:

	co::int32 getSize()
	{
		const co::Any& res = _handler->handleGetAttribute( _cookie, getAttribInfo<co::IReflector>( 0 ) );
        return res.get< co::int32 >();
	}

	co::IType* getType()
	{
		const co::Any& res = _handler->handleGetAttribute( _cookie, getAttribInfo<co::IReflector>( 1 ) );
        return res.get< co::IType* >();
	}

	void getAttribute( const co::Any& instance_, co::IAttributeInfo* ai_, co::Any& value_ )
	{
		co::Any args[3];
		args[0].set< const co::Any& >( instance_ );
		args[1].set< co::IAttributeInfo* >( ai_ );
		args[2].set< co::Any& >( value_ );
		co::ArrayRange<co::Any const> range( args, 3 );
		_handler->handleMethodInvocation( _cookie, getMethodInfo<co::IReflector>( 0 ), range );
	}

	void invokeMethod( const co::Any& instance_, co::IMethodInfo* mi_, co::ArrayRange<co::Any const> args_, co::Any& returnValue_ )
	{
		co::Any args[4];
		args[0].set< const co::Any& >( instance_ );
		args[1].set< co::IMethodInfo* >( mi_ );
		args[2].set< co::ArrayRange<co::Any const> >( args_ );
		args[3].set< co::Any& >( returnValue_ );
		co::ArrayRange<co::Any const> range( args, 4 );
		_handler->handleMethodInvocation( _cookie, getMethodInfo<co::IReflector>( 1 ), range );
	}

	co::IComponent* newInstance()
	{
		co::ArrayRange<co::Any const> range;
		const co::Any& res = _handler->handleMethodInvocation( _cookie, getMethodInfo<co::IReflector>( 2 ), range );
		return res.get< co::IComponent* >();
	}

	co::Interface* newProxy( co::IDynamicProxyHandler* handler_ )
	{
		co::Any args[1];
		args[0].set< co::IDynamicProxyHandler* >( handler_ );
		co::ArrayRange<co::Any const> range( args, 1 );
		const co::Any& res = _handler->handleMethodInvocation( _cookie, getMethodInfo<co::IReflector>( 3 ), range );
		return res.get< co::Interface* >();
	}

	void raise( const std::string& message_ )
	{
		co::Any args[1];
		args[0].set< const std::string& >( message_ );
		co::ArrayRange<co::Any const> range( args, 1 );
		_handler->handleMethodInvocation( _cookie, getMethodInfo<co::IReflector>( 4 ), range );
	}

	void setAttribute( const co::Any& instance_, co::IAttributeInfo* ai_, const co::Any& value_ )
	{
		co::Any args[3];
		args[0].set< const co::Any& >( instance_ );
		args[1].set< co::IAttributeInfo* >( ai_ );
		args[2].set< const co::Any& >( value_ );
		co::ArrayRange<co::Any const> range( args, 3 );
		_handler->handleMethodInvocation( _cookie, getMethodInfo<co::IReflector>( 5 ), range );
	}

	// These co::IReflector methods are not part of the reflection system:

	void createValue( void*, size_t )
	{
		throw co::NotSupportedException( "co::IReflector::createValue() cannot be called through a proxy interface." );
	}

	void copyValue( const void*, void* )
	{
		throw co::NotSupportedException( "co::IReflector::copyValue() cannot be called through a proxy interface." );
	}

	void destroyValue( void* )
	{
		throw co::NotSupportedException( "co::IReflector::destroyValue() cannot be called through a proxy interface." );
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

class IReflector_Reflector : public co::ReflectorBase
{
public:
	IReflector_Reflector()
	{
		// empty
	}

	virtual ~IReflector_Reflector()
	{
		// empty
	}

	co::IType* getType()
	{
		return co::typeOf<co::IReflector>::get();
	}

	co::int32 getSize()
	{
		return sizeof(co::IReflector);
	}

	co::Interface* newProxy( co::IDynamicProxyHandler* handler )
	{
		checValidProxyHandler( handler );
		return co::disambiguate<co::Interface, co::IReflector>( new co::IReflector_Proxy( handler ) );
	}

	void getAttribute( const co::Any& instance, co::IAttributeInfo* ai, co::Any& value )
	{
		co::IReflector* p = checkInstance( instance, ai );
		switch( ai->getIndex() )
		{
		case 0:		value.set< co::int32 >( p->getSize() ); break;
		case 1:		value.set< co::IType* >( p->getType() ); break;
		default:	raiseUnexpectedMemberIndex();
		}
	}

	void setAttribute( const co::Any& instance, co::IAttributeInfo* ai, const co::Any& value )
	{
		co::IReflector* p = checkInstance( instance, ai );
		switch( ai->getIndex() )
		{
		case 0:		raiseAttributeIsReadOnly( ai ); break;
		case 1:		raiseAttributeIsReadOnly( ai ); break;
		default:	raiseUnexpectedMemberIndex();
		}
		CORAL_UNUSED( p );
		CORAL_UNUSED( value );
	}

	void invokeMethod( const co::Any& instance, co::IMethodInfo* mi, co::ArrayRange<co::Any const> args, co::Any& res )
	{
		co::IReflector* p = checkInstance( instance, mi );
		checkNumArguments( mi, args.getSize() );
		int argIndex = -1;
		try
		{
			switch( mi->getIndex() )
			{
			case 2:
				{
					const co::Any& instance_ = args[++argIndex].get< const co::Any& >();
					co::IAttributeInfo* ai_ = args[++argIndex].get< co::IAttributeInfo* >();
					co::Any& value_ = args[++argIndex].get< co::Any& >();
					argIndex = -1;
					p->getAttribute( instance_, ai_, value_ );
				}
				break;
			case 3:
				{
					const co::Any& instance_ = args[++argIndex].get< const co::Any& >();
					co::IMethodInfo* mi_ = args[++argIndex].get< co::IMethodInfo* >();
					co::ArrayRange<co::Any const> args_ = args[++argIndex].get< co::ArrayRange<co::Any const> >();
					co::Any& returnValue_ = args[++argIndex].get< co::Any& >();
					argIndex = -1;
					p->invokeMethod( instance_, mi_, args_, returnValue_ );
				}
				break;
			case 4:
				{
					res.set< co::IComponent* >( p->newInstance() );
				}
				break;
			case 5:
				{
					co::IDynamicProxyHandler* handler_ = args[++argIndex].get< co::IDynamicProxyHandler* >();
					argIndex = -1;
					res.set< co::Interface* >( p->newProxy( handler_ ) );
				}
				break;
			case 6:
				{
					const std::string& message_ = args[++argIndex].get< const std::string& >();
					argIndex = -1;
					p->raise( message_ );
				}
				break;
			case 7:
				{
					const co::Any& instance_ = args[++argIndex].get< const co::Any& >();
					co::IAttributeInfo* ai_ = args[++argIndex].get< co::IAttributeInfo* >();
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
	co::IReflector* checkInstance( const co::Any& any, co::IMemberInfo* member )
	{
		if( !member )
			throw co::IllegalArgumentException( "illegal null member info" );

		// make sure that 'any' is an instance of this type
		co::IInterfaceType* myType = co::typeOf<co::IReflector>::get();

		co::IReflector* res;
		if( any.getKind() != co::TK_INTERFACE || !( res = dynamic_cast<co::IReflector*>( any.getState().data.itf ) ) )
			CORAL_THROW( co::IllegalArgumentException, "expected a valid co::IReflector*, but got " << any );

		// make sure that 'member' belongs to this type
		co::ICompoundType* owner = member->getOwner();
		if( owner != myType )
			CORAL_THROW( co::IllegalArgumentException, "member '" << member->getName() << "' belongs to "
				<< owner->getFullName() << ", not to co.IReflector" );

		return res;
	}
};

// ------ IReflector Creation Function ------ //

co::IReflector* __createIReflectorIReflector()
{
    return new IReflector_Reflector;
}

} // namespace co
