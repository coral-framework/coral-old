/*
 * Coral - A C++ Component Framework.
 * See Copyright Notice in Coral.h
 */

#include <co/Module.h>
#include <co/DynamicProxyHandler.h>
#include <co/Namespace.h>
#include <co/ModulePart.h>
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

class Module_Proxy : public co::Module
{
public:
	Module_Proxy( co::DynamicProxyHandler* handler ) : _handler( handler )
	{
		_cookie = _handler->registerProxyInterface( co::disambiguate<co::Interface, co::Module>( this ) );
	}

	virtual ~Module_Proxy()
	{
		// empty
	}

	// co::Interface Methods:

	co::InterfaceType* getInterfaceType() { return co::typeOf<co::Module>::get(); }
	co::Component* getInterfaceOwner() { return _handler->getInterfaceOwner(); }
	const std::string& getInterfaceName() { return _handler->getProxyInterfaceName( _cookie ); }
	void componentRetain() { _handler->componentRetain(); }
	void componentRelease() { _handler->componentRelease(); }

	// co.Module Methods:

	co::Namespace* getNamespace()
	{
		co::Any res;
		_handler->handleGetAttribute( _cookie, getAttribInfo<co::Module>( 0 ), res );
		assert( res.containsObject() == false );
        return res.get< co::Namespace* >();
	}

	co::ArrayRange<co::ModulePart* const> getParts()
	{
		co::Any res;
		_handler->handleGetAttribute( _cookie, getAttribInfo<co::Module>( 1 ), res );
		assert( res.containsObject() == false );
        return res.get< co::ArrayRange<co::ModulePart* const> >();
	}

	co::ModuleState getState()
	{
		co::Any res;
		_handler->handleGetAttribute( _cookie, getAttribInfo<co::Module>( 2 ), res );
		assert( res.containsObject() == false );
        return res.get< co::ModuleState >();
	}

	void abort()
	{
		co::Any res;
		co::ArrayRange<co::Any const> range;
		_handler->handleMethodInvocation( _cookie, getMethodInfo<co::Module>( 3 ), range, res );
	}

	void disintegrate()
	{
		co::Any res;
		co::ArrayRange<co::Any const> range;
		_handler->handleMethodInvocation( _cookie, getMethodInfo<co::Module>( 4 ), range, res );
	}

	void dispose()
	{
		co::Any res;
		co::ArrayRange<co::Any const> range;
		_handler->handleMethodInvocation( _cookie, getMethodInfo<co::Module>( 5 ), range, res );
	}

	void initialize()
	{
		co::Any res;
		co::ArrayRange<co::Any const> range;
		_handler->handleMethodInvocation( _cookie, getMethodInfo<co::Module>( 6 ), range, res );
	}

	void integrate()
	{
		co::Any res;
		co::ArrayRange<co::Any const> range;
		_handler->handleMethodInvocation( _cookie, getMethodInfo<co::Module>( 7 ), range, res );
	}

	void integratePresentation()
	{
		co::Any res;
		co::ArrayRange<co::Any const> range;
		_handler->handleMethodInvocation( _cookie, getMethodInfo<co::Module>( 8 ), range, res );
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

class Module_Reflector : public co::ReflectorBase
{
public:
	Module_Reflector()
	{
		// empty
	}

	virtual ~Module_Reflector()
	{
		// empty
	}

	co::Type* getType()
	{
		return co::typeOf<co::Module>::get();
	}

	co::int32 getSize()
	{
		return sizeof(co::Module);
	}

	co::Interface* newProxy( co::DynamicProxyHandler* handler )
	{
		checValidProxyHandler( handler );
		return co::disambiguate<co::Interface, co::Module>( new co::Module_Proxy( handler ) );
	}

	void getAttribute( const co::Any& instance, co::AttributeInfo* ai, co::Any& value )
	{
		co::Module* p = checkInstance( instance, ai );
		switch( ai->getIndex() )
		{
		case 0:		value.set< co::Namespace* >( p->getNamespace() ); break;
		case 1:		value.set< co::ArrayRange<co::ModulePart* const> >( p->getParts() ); break;
		case 2:		value.set< co::ModuleState >( p->getState() ); break;
		default:	raiseUnexpectedMemberIndex();
		}
	}

	void setAttribute( const co::Any& instance, co::AttributeInfo* ai, const co::Any& value )
	{
		co::Module* p = checkInstance( instance, ai );
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

	void invokeMethod( const co::Any& instance, co::MethodInfo* mi, co::ArrayRange<co::Any const> args, co::Any& res )
	{
		co::Module* p = checkInstance( instance, mi );
		checkNumArguments( mi, args.getSize() );
		int argIndex = -1;
		try
		{
			switch( mi->getIndex() )
			{
			case 3:
				{
					p->abort();
				}
				break;
			case 4:
				{
					p->disintegrate();
				}
				break;
			case 5:
				{
					p->dispose();
				}
				break;
			case 6:
				{
					p->initialize();
				}
				break;
			case 7:
				{
					p->integrate();
				}
				break;
			case 8:
				{
					p->integratePresentation();
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
	co::Module* checkInstance( const co::Any& instance, co::MemberInfo* member )
	{
		if( !member )
			throw co::IllegalArgumentException( "illegal null member info" );

		co::InterfaceType* myType = co::typeOf<co::Module>::get();

		// make sure that 'instance' is an instance of this type
		if( instance.getKind() != co::TK_INTERFACE ||
			!instance.getInterfaceType()->isSubTypeOf( myType ) ||
			instance.getState().data.ptr == NULL )
			CORAL_THROW( co::IllegalArgumentException, "expected a valid co::Module*, but got " << instance );

		// make sure that 'member' belongs to this type
		co::CompoundType* owner = member->getOwner();
		if( owner != myType )
			CORAL_THROW( co::IllegalArgumentException, "member '" << member->getName() << "' belongs to "
				<< owner->getFullName() << ", not to " << myType->getFullName() );

		return dynamic_cast<co::Module*>( instance.getState().data.itf );
	}
};

// ------ Reflector Creation Function ------ //

co::Reflector* __createModuleReflector()
{
    return new Module_Reflector;
}

} // namespace co
