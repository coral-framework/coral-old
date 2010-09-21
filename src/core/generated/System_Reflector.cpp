/*
 * Coral - A C++ Component Framework.
 * See Copyright Notice in Coral.h
 */

#include <co/System.h>
#include <co/DynamicProxyHandler.h>
#include <co/ModuleManager.h>
#include <co/ServiceManager.h>
#include <co/TypeManager.h>
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

class System_Proxy : public co::System
{
public:
	System_Proxy( co::DynamicProxyHandler* handler ) : _handler( handler )
	{
		_cookie = _handler->registerProxyInterface( co::disambiguate<co::Interface, co::System>( this ) );
	}

	virtual ~System_Proxy()
	{
		// empty
	}

	// co::Interface Methods:

	co::InterfaceType* getInterfaceType() { return co::typeOf<co::System>::get(); }
	co::Component* getInterfaceOwner() { return _handler->getInterfaceOwner(); }
	const std::string& getInterfaceName() { return _handler->getProxyInterfaceName( _cookie ); }
	void componentRetain() { _handler->componentRetain(); }
	void componentRelease() { _handler->componentRelease(); }

	// co.System Methods:

	co::ModuleManager* getModules()
	{
		co::Any res;
		_handler->handleGetAttribute( _cookie, getAttribInfo<co::System>( 0 ), res );
		assert( res.containsObject() == false );
        return res.get< co::ModuleManager* >();
	}

	co::ServiceManager* getServices()
	{
		co::Any res;
		_handler->handleGetAttribute( _cookie, getAttribInfo<co::System>( 1 ), res );
		assert( res.containsObject() == false );
        return res.get< co::ServiceManager* >();
	}

	co::SystemState getState()
	{
		co::Any res;
		_handler->handleGetAttribute( _cookie, getAttribInfo<co::System>( 2 ), res );
		assert( res.containsObject() == false );
        return res.get< co::SystemState >();
	}

	co::TypeManager* getTypes()
	{
		co::Any res;
		_handler->handleGetAttribute( _cookie, getAttribInfo<co::System>( 3 ), res );
		assert( res.containsObject() == false );
        return res.get< co::TypeManager* >();
	}

	void setupBase( co::ArrayRange<std::string const> requiredModules_ )
	{
		co::Any res, args[1];
		args[0].set< co::ArrayRange<std::string const> >( requiredModules_ );
		co::ArrayRange<co::Any const> range( args, 1 );
		_handler->handleMethodInvocation( _cookie, getMethodInfo<co::System>( 4 ), range, res );
	}

	void setupPresentation()
	{
		co::Any res;
		co::ArrayRange<co::Any const> range;
		_handler->handleMethodInvocation( _cookie, getMethodInfo<co::System>( 5 ), range, res );
	}

	void tearDown()
	{
		co::Any res;
		co::ArrayRange<co::Any const> range;
		_handler->handleMethodInvocation( _cookie, getMethodInfo<co::System>( 6 ), range, res );
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

class System_Reflector : public co::ReflectorBase
{
public:
	System_Reflector()
	{
		// empty
	}

	virtual ~System_Reflector()
	{
		// empty
	}

	co::Type* getType()
	{
		return co::typeOf<co::System>::get();
	}

	co::int32 getSize()
	{
		return sizeof(co::System);
	}

	co::Interface* newProxy( co::DynamicProxyHandler* handler )
	{
		checValidProxyHandler( handler );
		return co::disambiguate<co::Interface, co::System>( new co::System_Proxy( handler ) );
	}

	void getAttribute( const co::Any& instance, co::AttributeInfo* ai, co::Any& value )
	{
		co::System* p = checkInstance( instance, ai );
		switch( ai->getIndex() )
		{
		case 0:		value.set< co::ModuleManager* >( p->getModules() ); break;
		case 1:		value.set< co::ServiceManager* >( p->getServices() ); break;
		case 2:		value.set< co::SystemState >( p->getState() ); break;
		case 3:		value.set< co::TypeManager* >( p->getTypes() ); break;
		default:	raiseUnexpectedMemberIndex();
		}
	}

	void setAttribute( const co::Any& instance, co::AttributeInfo* ai, const co::Any& value )
	{
		co::System* p = checkInstance( instance, ai );
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
		co::System* p = checkInstance( instance, mi );
		checkNumArguments( mi, args.getSize() );
		int argIndex = -1;
		try
		{
			switch( mi->getIndex() )
			{
			case 4:
				{
					co::ArrayRange<std::string const> requiredModules_ = args[++argIndex].get< co::ArrayRange<std::string const> >();
					argIndex = -1;
					p->setupBase( requiredModules_ );
				}
				break;
			case 5:
				{
					p->setupPresentation();
				}
				break;
			case 6:
				{
					p->tearDown();
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
	co::System* checkInstance( const co::Any& any, co::MemberInfo* member )
	{
		if( !member )
			throw co::IllegalArgumentException( "illegal null member info" );

		// make sure that 'any' is an instance of this type
		co::InterfaceType* myType = co::typeOf<co::System>::get();

		co::System* res;
		if( any.getKind() != co::TK_INTERFACE || !( res = dynamic_cast<co::System*>( any.getState().data.itf ) ) )
			CORAL_THROW( co::IllegalArgumentException, "expected a valid co::System*, but got " << any );

		// make sure that 'member' belongs to this type
		co::CompoundType* owner = member->getOwner();
		if( owner != myType )
			CORAL_THROW( co::IllegalArgumentException, "member '" << member->getName() << "' belongs to "
				<< owner->getFullName() << ", not to co.System" );

		return res;
	}
};

// ------ Reflector Creation Function ------ //

co::Reflector* __createSystemReflector()
{
    return new System_Reflector;
}

} // namespace co
