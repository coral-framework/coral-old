/*
 * Coral - Lightweight C++ Component Framework.
 * See Copyright Notice in Coral.h
 */

#include <co/ModulePart.h>
#include <co/DynamicProxyHandler.h>
#include <co/Module.h>
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

class ModulePart_Proxy : public co::ModulePart
{
public:
	ModulePart_Proxy( co::DynamicProxyHandler* handler ) : _handler( handler )
	{
		_cookie = _handler->registerProxyInterface( co::disambiguate<co::Interface, co::ModulePart>( this ) );
	}

	virtual ~ModulePart_Proxy()
	{
		// empty
	}

	// co::Interface Methods:

	co::InterfaceType* getInterfaceType() { return co::typeOf<co::ModulePart>::get(); }
	co::Component* getInterfaceOwner() { return _handler->getInterfaceOwner(); }
	const std::string& getInterfaceName() { return _handler->getProxyInterfaceName( _cookie ); }
	void componentRetain() { _handler->componentRetain(); }
	void componentRelease() { _handler->componentRelease(); }

	// co.ModulePart Methods:

	void disintegrate( co::Module* module_ )
	{
		co::Any args[1];
		args[0].set< co::Module* >( module_ );
		co::ArrayRange<co::Any const> range( args, 1 );
		_handler->handleMethodInvocation( _cookie, getMethodInfo<co::ModulePart>( 0 ), range );
	}

	void dispose( co::Module* module_ )
	{
		co::Any args[1];
		args[0].set< co::Module* >( module_ );
		co::ArrayRange<co::Any const> range( args, 1 );
		_handler->handleMethodInvocation( _cookie, getMethodInfo<co::ModulePart>( 1 ), range );
	}

	void initialize( co::Module* module_ )
	{
		co::Any args[1];
		args[0].set< co::Module* >( module_ );
		co::ArrayRange<co::Any const> range( args, 1 );
		_handler->handleMethodInvocation( _cookie, getMethodInfo<co::ModulePart>( 2 ), range );
	}

	void integrate( co::Module* module_ )
	{
		co::Any args[1];
		args[0].set< co::Module* >( module_ );
		co::ArrayRange<co::Any const> range( args, 1 );
		_handler->handleMethodInvocation( _cookie, getMethodInfo<co::ModulePart>( 3 ), range );
	}

	void integratePresentation( co::Module* module_ )
	{
		co::Any args[1];
		args[0].set< co::Module* >( module_ );
		co::ArrayRange<co::Any const> range( args, 1 );
		_handler->handleMethodInvocation( _cookie, getMethodInfo<co::ModulePart>( 4 ), range );
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

class ModulePart_Reflector : public co::ReflectorBase
{
public:
	ModulePart_Reflector()
	{
		// empty
	}

	virtual ~ModulePart_Reflector()
	{
		// empty
	}

	co::Type* getType()
	{
		return co::typeOf<co::ModulePart>::get();
	}

	co::int32 getSize()
	{
		return sizeof(co::ModulePart);
	}

	co::Interface* newProxy( co::DynamicProxyHandler* handler )
	{
		checValidProxyHandler( handler );
		return co::disambiguate<co::Interface, co::ModulePart>( new co::ModulePart_Proxy( handler ) );
	}

	void getAttribute( const co::Any& instance, co::AttributeInfo* ai, co::Any& value )
	{
		checkInstance( instance, ai );
		raiseUnexpectedMemberIndex();
		CORAL_UNUSED( value );
	}

	void setAttribute( const co::Any& instance, co::AttributeInfo* ai, const co::Any& value )
	{
		checkInstance( instance, ai );
		raiseUnexpectedMemberIndex();
		CORAL_UNUSED( value );
	}

	void invokeMethod( const co::Any& instance, co::MethodInfo* mi, co::ArrayRange<co::Any const> args, co::Any& res )
	{
		co::ModulePart* p = checkInstance( instance, mi );
		checkNumArguments( mi, args.getSize() );
		int argIndex = -1;
		try
		{
			switch( mi->getIndex() )
			{
			case 0:
				{
					co::Module* module_ = args[++argIndex].get< co::Module* >();
					argIndex = -1;
					p->disintegrate( module_ );
				}
				break;
			case 1:
				{
					co::Module* module_ = args[++argIndex].get< co::Module* >();
					argIndex = -1;
					p->dispose( module_ );
				}
				break;
			case 2:
				{
					co::Module* module_ = args[++argIndex].get< co::Module* >();
					argIndex = -1;
					p->initialize( module_ );
				}
				break;
			case 3:
				{
					co::Module* module_ = args[++argIndex].get< co::Module* >();
					argIndex = -1;
					p->integrate( module_ );
				}
				break;
			case 4:
				{
					co::Module* module_ = args[++argIndex].get< co::Module* >();
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
	co::ModulePart* checkInstance( const co::Any& any, co::MemberInfo* member )
	{
		if( !member )
			throw co::IllegalArgumentException( "illegal null member info" );

		// make sure that 'any' is an instance of this type
		co::InterfaceType* myType = co::typeOf<co::ModulePart>::get();

		co::ModulePart* res;
		if( any.getKind() != co::TK_INTERFACE || !( res = dynamic_cast<co::ModulePart*>( any.getState().data.itf ) ) )
			CORAL_THROW( co::IllegalArgumentException, "expected a valid co::ModulePart*, but got " << any );

		// make sure that 'member' belongs to this type
		co::CompoundType* owner = member->getOwner();
		if( owner != myType )
			CORAL_THROW( co::IllegalArgumentException, "member '" << member->getName() << "' belongs to "
				<< owner->getFullName() << ", not to co.ModulePart" );

		return res;
	}
};

// ------ Reflector Creation Function ------ //

co::Reflector* __createModulePartReflector()
{
    return new ModulePart_Reflector;
}

} // namespace co
