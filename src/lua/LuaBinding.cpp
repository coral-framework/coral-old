/*
 * Coral - A C++ Component Framework.
 * See Copyright Notice in Coral.h
 */

#include "LuaBinding.h"
#include "LuaState.h"
#include <co/System.h>
#include <co/Component.h>
#include <co/Reflector.h>
#include <co/MethodInfo.h>
#include <co/AttributeInfo.h>
#include <co/ModuleLoadException.h>
#include <lua/Exception.h>

/*****************************************************************************/
/*  Helper Macros to Shield Lua from C++ Exceptions                          */
/*****************************************************************************/

#define __BEGIN_EXCEPTIONS_BARRIER__ try {

#define __END_EXCEPTIONS_BARRIER__ \
	} catch( ... ) { handleException( L ); } return lua_error( L );


/*****************************************************************************/
/*  Helper Function to Handle C++ Exceptions and Push a Lua Error String     */
/*****************************************************************************/

namespace lua { class MsgOnStackException {}; }

static void handleException( lua_State* L )
{
	try
	{
		throw;
	}
	catch( const lua::MsgOnStackException& )
	{
		// message is already on the stack
	}
	catch( const lua::Exception& e )
	{
		lua_pushfstring( L, "lua::Exception( %s )", e.getMessage().c_str() );
	}
	catch( const co::ModuleLoadException& e )
	{
		lua_pushfstring( L, "co::ModuleLoadException( %s )", e.getMessage().c_str() );
	}
	catch( const co::Exception& e )
	{
		lua_pushfstring( L, "co::Exception( %s )", e.getMessage().c_str() );
	}
    catch( const std::exception& e )
    {
		lua_pushfstring( L, "std::exception( %s )", e.what() );
    }
	catch( ... )
	{
		lua_pushstring( L, "unknown C++ exception" );
	}
}

/*****************************************************************************/
/*  Helper class that manages the 'co' package                               */
/*****************************************************************************/

void coPackage::open( lua_State* L )
{
	static luaL_Reg libFunctions[] = {
		{ "newInstance", newInstance },
		{ NULL, NULL }
	};

	luaL_openlib( L, "co", libFunctions, 0 );

	// create the 'co.system' singleton
	LuaState::instance().push( co::getSystem() );
	lua_setfield( L, -2, "system" );

	// pop the lib table
	lua_pop( L, 1 );
}

void coPackage::close( lua_State* L )
{
	// release the 'co.system' interface
	lua_getglobal( L, "co" );
	if( lua_istable( L, -1 ) )
	{
		lua_pushnil( L );
		lua_setfield( L, -2, "system" );
	}
	lua_pop( L, 1 );
}

int coPackage::newInstance( lua_State* L )
{
	const char* typeName = luaL_checkstring( L, 1 );

	__BEGIN_EXCEPTIONS_BARRIER__

	co::RefPtr<co::Component> c = co::newInstance( typeName );
	LuaState::instance().push( c.get() );

	return 1;

	__END_EXCEPTIONS_BARRIER__
}

/*****************************************************************************/
/*  Class with re-usable functions for binding co::CompoundTypes to Lua      */
/*****************************************************************************/

void CompoundTypeBinding::getInstance( lua_State* L, int index, co::Any& instance )
{
	// retrieve tag from the udata's metatable (at array index 1)
	lua_getmetatable( L, index );
	lua_rawgeti( L, -1, 1 );

	co::TypeKind tag = co::TK_NONE;
	if( lua_type( L, -1 ) == LUA_TNUMBER )
		tag = static_cast<co::TypeKind>( lua_tointeger( L, -1 ) );

	lua_pop( L, 2 );

	switch( tag )
	{
	case co::TK_STRUCT:
	case co::TK_NATIVECLASS:
	case co::TK_INTERFACE:
	case co::TK_COMPONENT:
		instance.set( *reinterpret_cast<co::Interface**>( lua_touserdata( L, index ) ) );
		break;

	default:
		throw lua::Exception( "unknown Lua userdata type" );
	}
}

const char* CompoundTypeBinding::getTypeName( co::TypeKind tag )
{
	const char* name;
	switch( tag )
	{
	case co::TK_STRUCT:			name = "struct";
	case co::TK_NATIVECLASS:	name = "native class";
	case co::TK_INTERFACE:		name = "co.Interface";
	case co::TK_COMPONENT:		name = "co.Component";
	default:					name = "unknown userdata";
	}
	return name;
}

void CompoundTypeBinding::checkType( lua_State* L, int narg, co::TypeKind expectedTag )
{
	if( !lua_isuserdata( L, narg ) )
		luaL_typeerror( L, narg, getTypeName( expectedTag ) );

	lua_getmetatable( L, narg );
	lua_rawgeti( L, -1, 1 );

	co::TypeKind tag = co::TK_NONE;
	if( lua_type( L, -1 ) == LUA_TNUMBER )
		tag = static_cast<co::TypeKind>( lua_tointeger( L, -1 ) );

	lua_pop( L, 2 );

	if( tag != expectedTag )
	{
		lua_Debug ar;
		lua_getstack( L, 0, &ar );
		lua_getinfo( L, "n", &ar );
		luaL_error( L, "bad argument %d to '%s' (%s expected, got %s)",
			narg, ar.name, getTypeName( expectedTag ), getTypeName( tag ) );
	}
}

struct Metamethods
{
	lua_CFunction index;
	lua_CFunction newIndex;
	lua_CFunction gc;
};

static const Metamethods TAG_MM[] = {
	// struct
	{ NULL, NULL, NULL },
	// native class
	{ NULL, NULL, NULL },
	// interface
	{ InterfaceBinding::index, InterfaceBinding::newIndex, InterfaceBinding::gc },
	// component
	{ NULL, NULL, NULL }
};

void CompoundTypeBinding::pushMetatable( lua_State* L, void* key, co::TypeKind tag )
{
	lua_pushlightuserdata( L, key );
	lua_rawget( L, LUA_REGISTRYINDEX );
	
	// type doesn't have a metatable yet?
	if( lua_isnil( L, -1 ) )
	{
		lua_pop( L, 1 );
		lua_createtable( L, 1, 3 );

		lua_pushinteger( L, tag );
		lua_rawseti( L, -2, 1 );
	
		assert( tag >= co::TK_STRUCT && tag <= co::TK_COMPONENT );
		const Metamethods& mms = TAG_MM[tag - co::TK_STRUCT];

		lua_pushliteral( L, "__index" );
		lua_pushcfunction( L, mms.index );
		lua_rawset( L, -3 );

		lua_pushliteral( L, "__newindex" );
		lua_pushcfunction( L, mms.newIndex );
		lua_rawset( L, -3 );

		lua_pushliteral( L, "__gc" );
		lua_pushcfunction( L, mms.gc );
		lua_rawset( L, -3 );

		// save the metatable in the registry
		lua_pushlightuserdata( L, key );
		lua_pushvalue( L, -2 );
		lua_rawset( L, LUA_REGISTRYINDEX );
	}
}

void CompoundTypeBinding::pushMember( lua_State* L, co::CompoundType* ct )
{
	// check if member is cached (in the type's metatable)
	lua_getmetatable( L, 1 );
	lua_pushvalue( L, 2 );
	lua_rawget( L, -2 );

	switch( lua_type( L, -1 ) )
	{
	case LUA_TNIL:
		// not in the cache yet
		{
			// pop the nil
			lua_pop( L, 1 );
	
			// try to find the member
			const char* memberName = lua_tostring( L, 2 );
			co::MemberInfo* mi = ct->getMember( memberName );
			if( !mi )
			{
				lua_pushliteral( L, "non-existing member '" );
				lua_pushvalue( L, 2 );
				lua_pushliteral( L, "'" );
				lua_concat( L, 3 );
				throw lua::MsgOnStackException();
			}

			// push the member
			lua_pushlightuserdata( L, mi );
			if( dynamic_cast<co::MethodInfo*>( mi ) != NULL )
			{
				lua_pushcclosure( L, callMethod, 1 );
			}
			
			// cache the member for future use
			lua_pushvalue( L, 2 );
			lua_pushvalue( L, -2 );
			lua_rawset( L, -4 );
		}
		break;

	case LUA_TFUNCTION:
		// got a cached method
		break;

	case LUA_TLIGHTUSERDATA:
		// got a cached non-method (attribute or interface)
		break;

	default:
		assert( false );
	}

	// pop the metatable
	lua_remove( L, -2 );
}

inline co::AttributeInfo* checkAttributeInfo( lua_State* L, int index )
{
	assert( lua_islightuserdata( L, index ) );
	co::MemberInfo* mi = reinterpret_cast<co::MemberInfo*>( lua_touserdata( L, index ) );
	return static_cast<co::AttributeInfo*>( mi );
}

void CompoundTypeBinding::getAttribute( lua_State* L, const co::Any& instance )
{
	co::AttributeInfo* ai = checkAttributeInfo( L, -1 );
	co::Reflector* reflector = ai->getOwner()->getReflector();
	co::Any value;
	reflector->getAttribute( instance, ai, value );
	lua_pop( L, 1 );
	LuaState::instance().push( value );
}

void CompoundTypeBinding::setAttribute( lua_State* L, const co::Any& instance )
{
	co::AttributeInfo* ai = checkAttributeInfo( L, -2 );
	co::Reflector* reflector = ai->getOwner()->getReflector();
	LuaValue value;
	LuaState::instance().toCoral( -1, value );
	reflector->setAttribute( instance, ai, value.any );
	lua_pop( L, 2 );
}

int CompoundTypeBinding::callMethod( lua_State* L )
{
	// check if a valid instance was provided as the first argument

	// get the co::MethodInfo* from upvalue 1
	co::MethodInfo* mi = static_cast<co::MethodInfo*>(
							reinterpret_cast<co::MemberInfo*>(
								lua_touserdata( L, lua_upvalueindex( 1 ) ) ) );
	CORAL_UNUSED( mi );

	//co::Reflector* reflector = mi->getOwner()->getReflector();
	return 0;
}

/*****************************************************************************/
/*  Helper class for binding co::Interfaces to Lua                           */
/*****************************************************************************/

void InterfaceBinding::create( lua_State* L, co::Interface* itf )
{
	// create the userdata
	co::Interface** ud = reinterpret_cast<co::Interface**>( lua_newuserdata( L, sizeof(co::Interface*) ) );

	*ud = itf;
	itf->componentRetain();

	// set the userdata's metatable
	pushMetatable( L, itf->getInterfaceType(), co::TK_INTERFACE );
	lua_setmetatable( L, -2 );
}

co::Interface* InterfaceBinding::checkInstance( lua_State* L, int narg )
{
	checkType( L, narg, co::TK_INTERFACE );
	return *reinterpret_cast<co::Interface**>( lua_touserdata( L, narg ) );
}

int InterfaceBinding::index( lua_State* L )
{
	__BEGIN_EXCEPTIONS_BARRIER__

	assert( lua_isuserdata( L, 1 ) );
	assert( lua_isstring( L, 2 ) );

	// get the interface pointer
	co::Interface** ud = reinterpret_cast<co::Interface**>( lua_touserdata( L, 1 ) );
	assert( ud );
	co::Interface* itf = *ud;

	pushMember( L, itf->getInterfaceType() );
	if( lua_islightuserdata( L, -1 ) )
		getAttribute( L, itf );

	return 1;

	__END_EXCEPTIONS_BARRIER__
}

int InterfaceBinding::newIndex( lua_State* L )
{
	__BEGIN_EXCEPTIONS_BARRIER__

	assert( lua_isuserdata( L, 1 ) );
	assert( lua_isstring( L, 2 ) );

	// get the interface pointer
	co::Interface** ud = reinterpret_cast<co::Interface**>( lua_touserdata( L, 1 ) );
	assert( ud );
	co::Interface* itf = *ud;
	
	pushMember( L, itf->getInterfaceType() );
	if( lua_isfunction( L, -1 ) )
	{
		lua_pushliteral( L, "'" );
		lua_pushvalue( L, 2 );
		lua_pushliteral( L, "' is a method and cannot be overwritten" );
		lua_concat( L, 3 );
		throw lua::MsgOnStackException();
	}

	lua_pushvalue( L, 3 );
	setAttribute( L, itf );

	return 0;

	__END_EXCEPTIONS_BARRIER__
}

int InterfaceBinding::gc( lua_State* L )
{
	__BEGIN_EXCEPTIONS_BARRIER__

	assert( lua_isuserdata( L, 1 ) );

	co::Interface** ud = reinterpret_cast<co::Interface**>( lua_touserdata( L, 1 ) );
	assert( ud );

	( *ud )->componentRelease();

	return 0;

	__END_EXCEPTIONS_BARRIER__
}

