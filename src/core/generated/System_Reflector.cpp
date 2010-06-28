/*******************************************************************************
** Reflection code generated for type 'co.System'
**
** Created: Mon Jun 28 12:01:36 2010
**      by: Coral Compiler version 0.1.0
**
** WARNING! All changes made in this file will be lost when recompiling!
********************************************************************************/

#include <co/System.h>
#include <co/reserved/ReflectorBase.h>
#include <co/DynamicProxyHandler.h>
#include <co/ModuleManager.h>
#include <co/ServiceManager.h>
#include <co/TypeManager.h>
#include <co/MethodInfo.h>
#include <co/AttributeInfo.h>
#include <co/IllegalCastException.h>
#include <co/MissingInputException.h>
#include <co/IllegalArgumentException.h>
#include <sstream>

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
		// empty destructor
	}

	// co::Interface Methods:

	co::InterfaceType* getInterfaceType() { return co::typeOf<co::System>::get(); }
	co::Component* getInterfaceOwner() { return _handler->getInterfaceOwner(); }
	const std::string& getInterfaceName() { return _handler->getProxyInterfaceName( _cookie ); }
	void componentRetain() { _handler->componentRetain(); }
	void componentRelease() { _handler->componentRelease(); }

	// co::System Methods:

	co::ModuleManager* getModules()
	{
		co::Any __res;
		_handler->handleGetAttribute( _cookie, getAttribInfo<co::System>( 0 ), __res );
        return __res.get< co::ModuleManager* >();
	}

	co::ServiceManager* getServices()
	{
		co::Any __res;
		_handler->handleGetAttribute( _cookie, getAttribInfo<co::System>( 1 ), __res );
        return __res.get< co::ServiceManager* >();
	}

	co::SystemState getState()
	{
		co::Any __res;
		_handler->handleGetAttribute( _cookie, getAttribInfo<co::System>( 2 ), __res );
        return __res.get< co::SystemState >();
	}

	co::TypeManager* getTypes()
	{
		co::Any __res;
		_handler->handleGetAttribute( _cookie, getAttribInfo<co::System>( 3 ), __res );
        return __res.get< co::TypeManager* >();
	}

	void setupBase( co::ArrayRange<std::string const> requiredModules )
	{
		co::Any __res, __arg[1];
		__arg[0].set< co::ArrayRange<std::string const> >( requiredModules );
		co::ArrayRange<co::Any const> __ar( __arg, 1 );
		_handler->handleMethodInvocation( _cookie, getMethodInfo<co::System>( 4 ), __ar, __res );
	}

	void setupPresentation( )
	{
		co::Any __res;
		co::ArrayRange<co::Any const> __ar;
		_handler->handleMethodInvocation( _cookie, getMethodInfo<co::System>( 5 ), __ar, __res );
	}

	void tearDown( )
	{
		co::Any __res;
		co::ArrayRange<co::Any const> __ar;
		_handler->handleMethodInvocation( _cookie, getMethodInfo<co::System>( 6 ), __ar, __res );
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
		return co::disambiguate<co::Interface, co::System>( new System_Proxy( handler ) );
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

	void invokeMethod( const co::Any& instance, co::MethodInfo* mi, co::ArrayRange<co::Any const> args, co::Any& __res )
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
					co::ArrayRange<std::string const> requiredModules = args[++argIndex].get< co::ArrayRange<std::string const> >();
					argIndex = -1;
					p->setupBase( requiredModules );
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
		CORAL_UNUSED( __res );
	}

private:
	co::System* checkInstance( const co::Any& instance, co::MemberInfo* member )
	{
		if( !member )
			throw co::IllegalArgumentException( "illegal null member info" );

		co::InterfaceType* myType = co::typeOf<co::System>::get();

		// make sure that 'instance' is an instance of this type
		if( instance.getKind() != co::TK_INTERFACE ||
			!instance.getInterfaceType()->isSubTypeOf( myType ) ||
			instance.getState().data.ptr == NULL )
			CORAL_THROW( co::IllegalArgumentException, "expected a valid co::System*, but got " << instance );

		// make sure that 'member' belongs to this type
		co::CompoundType* owner = member->getOwner();
		if( owner != myType )
			CORAL_THROW( co::IllegalArgumentException, "member '" << member->getName() << "' belongs to "
				<< owner->getFullName() << ", not to " << myType->getFullName() );

		return reinterpret_cast<co::System*>( instance.getState().data.ptr );
	}
};

// ------ Reflector Creation Function ------ //

namespace __coral {

co::Reflector* createSystemReflector()
{
    return new System_Reflector;
}

} // namespace __coral
