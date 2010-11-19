/*
 * Coral - A lightweight C++ component framework
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
		const co::Any& res = _handler->handleGetAttribute( _cookie, getAttribInfo<co::Module>( 0 ) );
        return res.get< co::Namespace* >();
	}

	co::ArrayRange<co::ModulePart* const> getParts()
	{
		const co::Any& res = _handler->handleGetAttribute( _cookie, getAttribInfo<co::Module>( 1 ) );
        return res.get< co::ArrayRange<co::ModulePart* const> >();
	}

	co::int32 getRank()
	{
		const co::Any& res = _handler->handleGetAttribute( _cookie, getAttribInfo<co::Module>( 2 ) );
        return res.get< co::int32 >();
	}

	void setRank( co::int32 rank_ )
	{
		co::Any arg;
		arg.set< co::int32 >( rank_ );
		_handler->handleSetAttribute( _cookie, getAttribInfo<co::Module>( 2 ), arg );
	}

	co::ModuleState getState()
	{
		const co::Any& res = _handler->handleGetAttribute( _cookie, getAttribInfo<co::Module>( 3 ) );
        return res.get< co::ModuleState >();
	}

	void abort()
	{
		co::ArrayRange<co::Any const> range;
		_handler->handleMethodInvocation( _cookie, getMethodInfo<co::Module>( 0 ), range );
	}

	void disintegrate()
	{
		co::ArrayRange<co::Any const> range;
		_handler->handleMethodInvocation( _cookie, getMethodInfo<co::Module>( 1 ), range );
	}

	void dispose()
	{
		co::ArrayRange<co::Any const> range;
		_handler->handleMethodInvocation( _cookie, getMethodInfo<co::Module>( 2 ), range );
	}

	void initialize()
	{
		co::ArrayRange<co::Any const> range;
		_handler->handleMethodInvocation( _cookie, getMethodInfo<co::Module>( 3 ), range );
	}

	void integrate()
	{
		co::ArrayRange<co::Any const> range;
		_handler->handleMethodInvocation( _cookie, getMethodInfo<co::Module>( 4 ), range );
	}

	void integratePresentation()
	{
		co::ArrayRange<co::Any const> range;
		_handler->handleMethodInvocation( _cookie, getMethodInfo<co::Module>( 5 ), range );
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
		case 2:		value.set< co::int32 >( p->getRank() ); break;
		case 3:		value.set< co::ModuleState >( p->getState() ); break;
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
		case 2:		p->setRank( value.get< co::int32 >() ); break;
		case 3:		raiseAttributeIsReadOnly( ai ); break;
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
			case 4:
				{
					p->abort();
				}
				break;
			case 5:
				{
					p->disintegrate();
				}
				break;
			case 6:
				{
					p->dispose();
				}
				break;
			case 7:
				{
					p->initialize();
				}
				break;
			case 8:
				{
					p->integrate();
				}
				break;
			case 9:
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
	co::Module* checkInstance( const co::Any& any, co::MemberInfo* member )
	{
		if( !member )
			throw co::IllegalArgumentException( "illegal null member info" );

		// make sure that 'any' is an instance of this type
		co::InterfaceType* myType = co::typeOf<co::Module>::get();

		co::Module* res;
		if( any.getKind() != co::TK_INTERFACE || !( res = dynamic_cast<co::Module*>( any.getState().data.itf ) ) )
			CORAL_THROW( co::IllegalArgumentException, "expected a valid co::Module*, but got " << any );

		// make sure that 'member' belongs to this type
		co::CompoundType* owner = member->getOwner();
		if( owner != myType )
			CORAL_THROW( co::IllegalArgumentException, "member '" << member->getName() << "' belongs to "
				<< owner->getFullName() << ", not to co.Module" );

		return res;
	}
};

// ------ Reflector Creation Function ------ //

co::Reflector* __createModuleReflector()
{
    return new Module_Reflector;
}

} // namespace co
