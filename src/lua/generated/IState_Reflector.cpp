/*******************************************************************************
** Reflection code generated for type 'lua.IState'
**
** Created: Mon Sep 20 12:13:52 2010
**      by: Coral Compiler version 0.3.0
**
** WARNING! All changes made in this file will be lost when recompiling!
********************************************************************************/

#include <lua/IState.h>
#include <co/DynamicProxyHandler.h>
#include <co/MethodInfo.h>
#include <co/AttributeInfo.h>
#include <co/IllegalCastException.h>
#include <co/MissingInputException.h>
#include <co/IllegalArgumentException.h>
#include <co/reserved/ReflectorBase.h>
#include <sstream>
#include <cassert>

#if !defined( CORAL_COMPILER_OUTPUT_REVISION )
#error "The header file <co/Config.h> was not included."
#elif CORAL_COMPILER_OUTPUT_REVISION != 1
#error "This file was generated using the Coral Compiler v0.3.0. It"
#error "cannot be used with the include files from this version of Coral."
#error "(The code generation rules have changed too much.)"
#endif

namespace lua {

void moduleRetain();
void moduleRelease();

// ------ Proxy Interface ------ //

class IState_Proxy : public lua::IState
{
public:
	IState_Proxy( co::DynamicProxyHandler* handler ) : _handler( handler )
	{
		moduleRetain();
		_cookie = _handler->registerProxyInterface( co::disambiguate<co::Interface, lua::IState>( this ) );
	}

	virtual ~IState_Proxy()
	{
		moduleRelease();
	}

	// co::Interface Methods:

	co::InterfaceType* getInterfaceType() { return co::typeOf<lua::IState>::get(); }
	co::Component* getInterfaceOwner() { return _handler->getInterfaceOwner(); }
	const std::string& getInterfaceName() { return _handler->getProxyInterfaceName( _cookie ); }
	void componentRetain() { _handler->componentRetain(); }
	void componentRelease() { _handler->componentRelease(); }

	// lua.IState Methods:

	void call( co::int32 __numArgs, co::int32 __numResults )
	{
		co::Any res, args[2];
		args[0].set< co::int32 >( __numArgs );
		args[1].set< co::int32 >( __numResults );
		co::ArrayRange<co::Any const> range( args, 2 );
		_handler->handleMethodInvocation( _cookie, getMethodInfo<lua::IState>( 0 ), range, res );
	}

	void loadFile( const std::string& __filename )
	{
		co::Any res, args[1];
		args[0].set< const std::string& >( __filename );
		co::ArrayRange<co::Any const> range( args, 1 );
		_handler->handleMethodInvocation( _cookie, getMethodInfo<lua::IState>( 1 ), range, res );
	}

	void push( const co::Any& __value )
	{
		co::Any res, args[1];
		args[0].set< const co::Any& >( __value );
		co::ArrayRange<co::Any const> range( args, 1 );
		_handler->handleMethodInvocation( _cookie, getMethodInfo<lua::IState>( 2 ), range, res );
	}

	bool searchScript( const std::string& __name, std::string& __filename )
	{
		co::Any res, args[2];
		args[0].set< const std::string& >( __name );
		args[1].set< std::string& >( __filename );
		co::ArrayRange<co::Any const> range( args, 2 );
		_handler->handleMethodInvocation( _cookie, getMethodInfo<lua::IState>( 3 ), range, res );
		assert( res.containsObject() == false );
		return res.get< bool >();
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

class IState_Reflector : public co::ReflectorBase
{
public:
	IState_Reflector()
	{
		moduleRetain();
	}

	virtual ~IState_Reflector()
	{
		moduleRelease();
	}

	co::Type* getType()
	{
		return co::typeOf<lua::IState>::get();
	}

	co::int32 getSize()
	{
		return sizeof(lua::IState);
	}

	co::Interface* newProxy( co::DynamicProxyHandler* handler )
	{
		checValidProxyHandler( handler );
		return co::disambiguate<co::Interface, lua::IState>( new lua::IState_Proxy( handler ) );
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
		lua::IState* p = checkInstance( instance, mi );
		checkNumArguments( mi, args.getSize() );
		int argIndex = -1;
		try
		{
			switch( mi->getIndex() )
			{
			case 0:
				{
					co::int32 __numArgs = args[++argIndex].get< co::int32 >();
					co::int32 __numResults = args[++argIndex].get< co::int32 >();
					argIndex = -1;
					p->call( __numArgs, __numResults );
				}
				break;
			case 1:
				{
					const std::string& __filename = args[++argIndex].get< const std::string& >();
					argIndex = -1;
					p->loadFile( __filename );
				}
				break;
			case 2:
				{
					const co::Any& __value = args[++argIndex].get< const co::Any& >();
					argIndex = -1;
					p->push( __value );
				}
				break;
			case 3:
				{
					const std::string& __name = args[++argIndex].get< const std::string& >();
					std::string& __filename = args[++argIndex].get< std::string& >();
					argIndex = -1;
					res.set< bool >( p->searchScript( __name, __filename ) );
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
	lua::IState* checkInstance( const co::Any& instance, co::MemberInfo* member )
	{
		if( !member )
			throw co::IllegalArgumentException( "illegal null member info" );

		co::InterfaceType* myType = co::typeOf<lua::IState>::get();

		// make sure that 'instance' is an instance of this type
		if( instance.getKind() != co::TK_INTERFACE ||
			!instance.getInterfaceType()->isSubTypeOf( myType ) ||
			instance.getState().data.ptr == NULL )
			CORAL_THROW( co::IllegalArgumentException, "expected a valid lua::IState*, but got " << instance );

		// make sure that 'member' belongs to this type
		co::CompoundType* owner = member->getOwner();
		if( owner != myType )
			CORAL_THROW( co::IllegalArgumentException, "member '" << member->getName() << "' belongs to "
				<< owner->getFullName() << ", not to " << myType->getFullName() );

		return dynamic_cast<lua::IState*>( instance.getState().data.itf );
	}
};

// ------ Reflector Creation Function ------ //

co::Reflector* __createIStateReflector()
{
    return new IState_Reflector;
}

} // namespace lua
