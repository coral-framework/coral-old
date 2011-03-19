/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#include <co/IModule.h>
#include <co/IDynamicProxyHandler.h>
#include <co/INamespace.h>
#include <co/IModulePart.h>
#include <co/IMethodInfo.h>
#include <co/IAttributeInfo.h>
#include <co/IllegalCastException.h>
#include <co/MissingInputException.h>
#include <co/IllegalArgumentException.h>
#include <co/reserved/ReflectorBase.h>
#include <sstream>
#include <cassert>

namespace co {

// ------ Proxy Interface ------ //

class IModule_Proxy : public co::IModule
{
public:
	IModule_Proxy( co::IDynamicProxyHandler* handler ) : _handler( handler )
	{
		_cookie = _handler->registerProxyInterface( co::disambiguate<co::Interface, co::IModule>( this ) );
	}

	virtual ~IModule_Proxy()
	{
		// empty
	}

	// co::Interface Methods:

	co::IInterfaceType* getInterfaceType() { return co::typeOf<co::IModule>::get(); }
	co::IComponent* getInterfaceOwner() { return _handler->getInterfaceOwner(); }
	const std::string& getInterfaceName() { return _handler->getProxyInterfaceName( _cookie ); }
	void componentRetain() { _handler->componentRetain(); }
	void componentRelease() { _handler->componentRelease(); }

	// co.IModule Methods:

	co::INamespace* getNamespace()
	{
		const co::Any& res = _handler->handleGetAttribute( _cookie, getAttribInfo<co::IModule>( 0 ) );
        return res.get< co::INamespace* >();
	}

	co::ArrayRange<co::IModulePart* const> getParts()
	{
		const co::Any& res = _handler->handleGetAttribute( _cookie, getAttribInfo<co::IModule>( 1 ) );
        return res.get< co::ArrayRange<co::IModulePart* const> >();
	}

	co::int32 getRank()
	{
		const co::Any& res = _handler->handleGetAttribute( _cookie, getAttribInfo<co::IModule>( 2 ) );
        return res.get< co::int32 >();
	}

	void setRank( co::int32 rank_ )
	{
		co::Any arg;
		arg.set< co::int32 >( rank_ );
		_handler->handleSetAttribute( _cookie, getAttribInfo<co::IModule>( 2 ), arg );
	}

	co::ModuleState getState()
	{
		const co::Any& res = _handler->handleGetAttribute( _cookie, getAttribInfo<co::IModule>( 3 ) );
        return res.get< co::ModuleState >();
	}

	void abort()
	{
		co::ArrayRange<co::Any const> range;
		_handler->handleMethodInvocation( _cookie, getMethodInfo<co::IModule>( 0 ), range );
	}

	void disintegrate()
	{
		co::ArrayRange<co::Any const> range;
		_handler->handleMethodInvocation( _cookie, getMethodInfo<co::IModule>( 1 ), range );
	}

	void dispose()
	{
		co::ArrayRange<co::Any const> range;
		_handler->handleMethodInvocation( _cookie, getMethodInfo<co::IModule>( 2 ), range );
	}

	void initialize()
	{
		co::ArrayRange<co::Any const> range;
		_handler->handleMethodInvocation( _cookie, getMethodInfo<co::IModule>( 3 ), range );
	}

	void integrate()
	{
		co::ArrayRange<co::Any const> range;
		_handler->handleMethodInvocation( _cookie, getMethodInfo<co::IModule>( 4 ), range );
	}

	void integratePresentation()
	{
		co::ArrayRange<co::Any const> range;
		_handler->handleMethodInvocation( _cookie, getMethodInfo<co::IModule>( 5 ), range );
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

class IModule_Reflector : public co::ReflectorBase
{
public:
	IModule_Reflector()
	{
		// empty
	}

	virtual ~IModule_Reflector()
	{
		// empty
	}

	co::IType* getType()
	{
		return co::typeOf<co::IModule>::get();
	}

	co::int32 getSize()
	{
		return sizeof(co::IModule);
	}

	co::Interface* newProxy( co::IDynamicProxyHandler* handler )
	{
		checValidProxyHandler( handler );
		return co::disambiguate<co::Interface, co::IModule>( new co::IModule_Proxy( handler ) );
	}

	void getAttribute( const co::Any& instance, co::IAttributeInfo* ai, co::Any& value )
	{
		co::IModule* p = checkInstance( instance, ai );
		switch( ai->getIndex() )
		{
		case 0:		value.set< co::INamespace* >( p->getNamespace() ); break;
		case 1:		value.set< co::ArrayRange<co::IModulePart* const> >( p->getParts() ); break;
		case 2:		value.set< co::int32 >( p->getRank() ); break;
		case 3:		value.set< co::ModuleState >( p->getState() ); break;
		default:	raiseUnexpectedMemberIndex();
		}
	}

	void setAttribute( const co::Any& instance, co::IAttributeInfo* ai, const co::Any& value )
	{
		co::IModule* p = checkInstance( instance, ai );
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

	void invokeMethod( const co::Any& instance, co::IMethodInfo* mi, co::ArrayRange<co::Any const> args, co::Any& res )
	{
		co::IModule* p = checkInstance( instance, mi );
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
	co::IModule* checkInstance( const co::Any& any, co::IMemberInfo* member )
	{
		if( !member )
			throw co::IllegalArgumentException( "illegal null member info" );

		// make sure that 'any' is an instance of this type
		co::IInterfaceType* myType = co::typeOf<co::IModule>::get();

		co::IModule* res;
		if( any.getKind() != co::TK_INTERFACE || !( res = dynamic_cast<co::IModule*>( any.getState().data.itf ) ) )
			CORAL_THROW( co::IllegalArgumentException, "expected a valid co::IModule*, but got " << any );

		// make sure that 'member' belongs to this type
		co::ICompoundType* owner = member->getOwner();
		if( owner != myType )
			CORAL_THROW( co::IllegalArgumentException, "member '" << member->getName() << "' belongs to "
				<< owner->getFullName() << ", not to co.IModule" );

		return res;
	}
};

// ------ IReflector Creation Function ------ //

co::IReflector* __createIModuleIReflector()
{
    return new IModule_Reflector;
}

} // namespace co
