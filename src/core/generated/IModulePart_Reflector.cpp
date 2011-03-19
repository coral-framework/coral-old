/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#include <co/IModulePart.h>
#include <co/IDynamicProxyHandler.h>
#include <co/IModule.h>
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

class IModulePart_Proxy : public co::IModulePart
{
public:
	IModulePart_Proxy( co::IDynamicProxyHandler* handler ) : _handler( handler )
	{
		_cookie = _handler->registerProxyInterface( co::disambiguate<co::Interface, co::IModulePart>( this ) );
	}

	virtual ~IModulePart_Proxy()
	{
		// empty
	}

	// co::Interface Methods:

	co::IInterfaceType* getInterfaceType() { return co::typeOf<co::IModulePart>::get(); }
	co::IComponent* getInterfaceOwner() { return _handler->getInterfaceOwner(); }
	const std::string& getInterfaceName() { return _handler->getProxyInterfaceName( _cookie ); }
	void componentRetain() { _handler->componentRetain(); }
	void componentRelease() { _handler->componentRelease(); }

	// co.IModulePart Methods:

	void disintegrate( co::IModule* module_ )
	{
		co::Any args[1];
		args[0].set< co::IModule* >( module_ );
		co::ArrayRange<co::Any const> range( args, 1 );
		_handler->handleMethodInvocation( _cookie, getMethodInfo<co::IModulePart>( 0 ), range );
	}

	void dispose( co::IModule* module_ )
	{
		co::Any args[1];
		args[0].set< co::IModule* >( module_ );
		co::ArrayRange<co::Any const> range( args, 1 );
		_handler->handleMethodInvocation( _cookie, getMethodInfo<co::IModulePart>( 1 ), range );
	}

	void initialize( co::IModule* module_ )
	{
		co::Any args[1];
		args[0].set< co::IModule* >( module_ );
		co::ArrayRange<co::Any const> range( args, 1 );
		_handler->handleMethodInvocation( _cookie, getMethodInfo<co::IModulePart>( 2 ), range );
	}

	void integrate( co::IModule* module_ )
	{
		co::Any args[1];
		args[0].set< co::IModule* >( module_ );
		co::ArrayRange<co::Any const> range( args, 1 );
		_handler->handleMethodInvocation( _cookie, getMethodInfo<co::IModulePart>( 3 ), range );
	}

	void integratePresentation( co::IModule* module_ )
	{
		co::Any args[1];
		args[0].set< co::IModule* >( module_ );
		co::ArrayRange<co::Any const> range( args, 1 );
		_handler->handleMethodInvocation( _cookie, getMethodInfo<co::IModulePart>( 4 ), range );
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

class IModulePart_Reflector : public co::ReflectorBase
{
public:
	IModulePart_Reflector()
	{
		// empty
	}

	virtual ~IModulePart_Reflector()
	{
		// empty
	}

	co::IType* getType()
	{
		return co::typeOf<co::IModulePart>::get();
	}

	co::int32 getSize()
	{
		return sizeof(co::IModulePart);
	}

	co::Interface* newProxy( co::IDynamicProxyHandler* handler )
	{
		checValidProxyHandler( handler );
		return co::disambiguate<co::Interface, co::IModulePart>( new co::IModulePart_Proxy( handler ) );
	}

	void getAttribute( const co::Any& instance, co::IAttributeInfo* ai, co::Any& value )
	{
		checkInstance( instance, ai );
		raiseUnexpectedMemberIndex();
		CORAL_UNUSED( value );
	}

	void setAttribute( const co::Any& instance, co::IAttributeInfo* ai, const co::Any& value )
	{
		checkInstance( instance, ai );
		raiseUnexpectedMemberIndex();
		CORAL_UNUSED( value );
	}

	void invokeMethod( const co::Any& instance, co::IMethodInfo* mi, co::ArrayRange<co::Any const> args, co::Any& res )
	{
		co::IModulePart* p = checkInstance( instance, mi );
		checkNumArguments( mi, args.getSize() );
		int argIndex = -1;
		try
		{
			switch( mi->getIndex() )
			{
			case 0:
				{
					co::IModule* module_ = args[++argIndex].get< co::IModule* >();
					argIndex = -1;
					p->disintegrate( module_ );
				}
				break;
			case 1:
				{
					co::IModule* module_ = args[++argIndex].get< co::IModule* >();
					argIndex = -1;
					p->dispose( module_ );
				}
				break;
			case 2:
				{
					co::IModule* module_ = args[++argIndex].get< co::IModule* >();
					argIndex = -1;
					p->initialize( module_ );
				}
				break;
			case 3:
				{
					co::IModule* module_ = args[++argIndex].get< co::IModule* >();
					argIndex = -1;
					p->integrate( module_ );
				}
				break;
			case 4:
				{
					co::IModule* module_ = args[++argIndex].get< co::IModule* >();
					argIndex = -1;
					p->integratePresentation( module_ );
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
	co::IModulePart* checkInstance( const co::Any& any, co::IMemberInfo* member )
	{
		if( !member )
			throw co::IllegalArgumentException( "illegal null member info" );

		// make sure that 'any' is an instance of this type
		co::IInterfaceType* myType = co::typeOf<co::IModulePart>::get();

		co::IModulePart* res;
		if( any.getKind() != co::TK_INTERFACE || !( res = dynamic_cast<co::IModulePart*>( any.getState().data.itf ) ) )
			CORAL_THROW( co::IllegalArgumentException, "expected a valid co::IModulePart*, but got " << any );

		// make sure that 'member' belongs to this type
		co::ICompoundType* owner = member->getOwner();
		if( owner != myType )
			CORAL_THROW( co::IllegalArgumentException, "member '" << member->getName() << "' belongs to "
				<< owner->getFullName() << ", not to co.IModulePart" );

		return res;
	}
};

// ------ IReflector Creation Function ------ //

co::IReflector* __createIModulePartIReflector()
{
    return new IModulePart_Reflector;
}

} // namespace co
