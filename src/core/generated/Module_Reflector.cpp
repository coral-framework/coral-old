/*******************************************************************************
** Reflection code generated for type 'co.Module'
**
** Created: Thu Jun 10 20:53:30 2010
**      by: Coral Compiler version 0.1.0
**
** WARNING! All changes made in this file will be lost when recompiling!
********************************************************************************/

#include <co/Module.h>
#include <co/reserved/ReflectorBase.h>
#include <co/DynamicProxyHandler.h>
#include <co/ModulePart.h>
#include <co/Namespace.h>
#include <co/MethodInfo.h>
#include <co/AttributeInfo.h>
#include <co/IllegalCastException.h>
#include <co/MissingInputException.h>
#include <co/IllegalArgumentException.h>
#include <sstream>

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
		// empty destructor
	}

	// co::Interface Methods:

	co::InterfaceType* getInterfaceType() { return co::typeOf<co::Module>::get(); }
	co::Component* getInterfaceOwner() { return _handler->getInterfaceOwner(); }
	const std::string& getInterfaceName() { return _handler->getProxyInterfaceName( _cookie ); }
	void componentRetain() { _handler->componentRetain(); }
	void componentRelease() { _handler->componentRelease(); }

	// co::Module Methods:

	co::Namespace* getNamespace()
	{
		co::Any __res;
		_handler->handleGetAttribute( _cookie, getAttribInfo<co::Module>( 0 ), __res );
        return __res.get< co::Namespace* >();
	}

	co::ArrayRange<co::ModulePart* const> getParts()
	{
		co::Any __res;
		_handler->handleGetAttribute( _cookie, getAttribInfo<co::Module>( 1 ), __res );
        return __res.get< co::ArrayRange<co::ModulePart* const> >();
	}

	co::ModuleState getState()
	{
		co::Any __res;
		_handler->handleGetAttribute( _cookie, getAttribInfo<co::Module>( 2 ), __res );
        return __res.get< co::ModuleState >();
	}

	void abort( )
	{
		co::Any __res;
		co::ArrayRange<co::Any const> __ar;
		_handler->handleMethodInvocation( _cookie, getMethodInfo<co::Module>( 3 ), __ar, __res );
	}

	void disintegrate( )
	{
		co::Any __res;
		co::ArrayRange<co::Any const> __ar;
		_handler->handleMethodInvocation( _cookie, getMethodInfo<co::Module>( 4 ), __ar, __res );
	}

	void dispose( )
	{
		co::Any __res;
		co::ArrayRange<co::Any const> __ar;
		_handler->handleMethodInvocation( _cookie, getMethodInfo<co::Module>( 5 ), __ar, __res );
	}

	void initialize( )
	{
		co::Any __res;
		co::ArrayRange<co::Any const> __ar;
		_handler->handleMethodInvocation( _cookie, getMethodInfo<co::Module>( 6 ), __ar, __res );
	}

	void integrate( )
	{
		co::Any __res;
		co::ArrayRange<co::Any const> __ar;
		_handler->handleMethodInvocation( _cookie, getMethodInfo<co::Module>( 7 ), __ar, __res );
	}

	void integratePresentation( )
	{
		co::Any __res;
		co::ArrayRange<co::Any const> __ar;
		_handler->handleMethodInvocation( _cookie, getMethodInfo<co::Module>( 8 ), __ar, __res );
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
		return co::disambiguate<co::Interface, co::Module>( new Module_Proxy( handler ) );
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

	void invokeMethod( const co::Any& instance, co::MethodInfo* mi, co::ArrayRange<co::Any const> args, co::Any& __res )
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
		CORAL_UNUSED( __res );
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

		return reinterpret_cast<co::Module*>( instance.getState().data.ptr );
	}
};

// ------ Reflector Creation Function ------ //

namespace __coral {

co::Reflector* createModuleReflector()
{
    return new Module_Reflector;
}

} // namespace __coral
