/*
 * Coral - A C++ Component Framework.
 * See Copyright Notice in Coral.h
 */

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

	void call( co::int32 numArgs_, co::int32 numResults_ )
	{
		co::Any args[2];
		args[0].set< co::int32 >( numArgs_ );
		args[1].set< co::int32 >( numResults_ );
		co::ArrayRange<co::Any const> range( args, 2 );
		_handler->handleMethodInvocation( _cookie, getMethodInfo<lua::IState>( 0 ), range );
	}

	void loadFile( const std::string& filename_ )
	{
		co::Any args[1];
		args[0].set< const std::string& >( filename_ );
		co::ArrayRange<co::Any const> range( args, 1 );
		_handler->handleMethodInvocation( _cookie, getMethodInfo<lua::IState>( 1 ), range );
	}

	void push( const co::Any& value_ )
	{
		co::Any args[1];
		args[0].set< const co::Any& >( value_ );
		co::ArrayRange<co::Any const> range( args, 1 );
		_handler->handleMethodInvocation( _cookie, getMethodInfo<lua::IState>( 2 ), range );
	}

	bool searchScript( const std::string& name_, std::string& filename_ )
	{
		co::Any args[2];
		args[0].set< const std::string& >( name_ );
		args[1].set< std::string& >( filename_ );
		co::ArrayRange<co::Any const> range( args, 2 );
		const co::Any& res = _handler->handleMethodInvocation( _cookie, getMethodInfo<lua::IState>( 3 ), range );
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
					co::int32 numArgs_ = args[++argIndex].get< co::int32 >();
					co::int32 numResults_ = args[++argIndex].get< co::int32 >();
					argIndex = -1;
					p->call( numArgs_, numResults_ );
				}
				break;
			case 1:
				{
					const std::string& filename_ = args[++argIndex].get< const std::string& >();
					argIndex = -1;
					p->loadFile( filename_ );
				}
				break;
			case 2:
				{
					const co::Any& value_ = args[++argIndex].get< const co::Any& >();
					argIndex = -1;
					p->push( value_ );
				}
				break;
			case 3:
				{
					const std::string& name_ = args[++argIndex].get< const std::string& >();
					std::string& filename_ = args[++argIndex].get< std::string& >();
					argIndex = -1;
					res.set< bool >( p->searchScript( name_, filename_ ) );
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
	lua::IState* checkInstance( const co::Any& any, co::MemberInfo* member )
	{
		if( !member )
			throw co::IllegalArgumentException( "illegal null member info" );

		// make sure that 'any' is an instance of this type
		co::InterfaceType* myType = co::typeOf<lua::IState>::get();

		lua::IState* res;
		if( any.getKind() != co::TK_INTERFACE || !( res = dynamic_cast<lua::IState*>( any.getState().data.itf ) ) )
			CORAL_THROW( co::IllegalArgumentException, "expected a valid lua::IState*, but got " << any );

		// make sure that 'member' belongs to this type
		co::CompoundType* owner = member->getOwner();
		if( owner != myType )
			CORAL_THROW( co::IllegalArgumentException, "member '" << member->getName() << "' belongs to "
				<< owner->getFullName() << ", not to lua.IState" );

		return res;
	}
};

// ------ Reflector Creation Function ------ //

co::Reflector* __createIStateReflector()
{
    return new IState_Reflector;
}

} // namespace lua
