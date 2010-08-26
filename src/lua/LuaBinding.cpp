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
#include <co/InterfaceInfo.h>
#include <co/ParameterInfo.h>
#include <co/ModuleLoadException.h>
#include <co/IllegalArgumentException.h>
#include <co/reserved/FileLookUp.h>
#include <lua/Exception.h>
#include <cstring>
#include <sstream>

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
	catch( const co::Exception& e )
	{
		lua_pushstring( L, e.getMessage().c_str() );
	}
    catch( const std::exception& e )
    {
		lua_pushfstring( L, "C++ exception: %s", e.what() );
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
		{ "getType", getType },
		{ "new", genericNew },
		{ "packageLoader", packageLoader },
		{ NULL, NULL }
	};

	// create our lib table, containing the libFunctions
	luaL_newlib( L, libFunctions );

	// set the 'co.system' singleton
	LuaState::instance().push( co::getSystem() );
	lua_setfield( L, -2, "system" );

	// save the table as a global
	lua_setglobal( L, "co" );
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

int coPackage::getType( lua_State* L )
{
	const char* typeName = luaL_checkstring( L, 1 );

	__BEGIN_EXCEPTIONS_BARRIER__

	co::Type* type = co::getType( typeName );
	LuaState::instance().push( type );

	return 1;

	__END_EXCEPTIONS_BARRIER__
}

int coPackage::genericNew( lua_State* L )
{
	const char* typeName = luaL_checkstring( L, 1 );

	__BEGIN_EXCEPTIONS_BARRIER__

	co::Type* type = co::getType( typeName );
	co::TypeKind kind = type->getKind();
	if( kind == co::TK_STRUCT || kind == co::TK_NATIVECLASS )
	{
		ComplexValueBinding::push( L, type, NULL );
	}
	else if( kind == co::TK_COMPONENT )
	{
		co::Reflector* reflector = type->getReflector();
		co::RefPtr<co::Component> c = reflector->newInstance();
		LuaState::instance().push( c.get() );
	}
	else
	{
		lua_pushvalue( L, 1 );
		lua_pushliteral( L, " is not instantiable" );
		lua_concat( L, 2 );
		throw lua::MsgOnStackException();
	}

	return 1;

	__END_EXCEPTIONS_BARRIER__
}

int coPackage::packageLoader( lua_State* L )
{
	static const std::string s_extension( "lua" );

	const char* moduleName = luaL_checkstring( L, 1 );
	bool raiseError = false;

	// look for a Lua script file corresponding to 'moduleName' in the Coral path
	{
		co::FileLookUp fileLookUp( co::getPaths(), co::ArrayRange<const std::string>( &s_extension, 1 ) );
		fileLookUp.addFilePath( moduleName, true );

		std::string filename;
		if( !fileLookUp.locate( filename ) )
		{
			lua_pushnil( L );
			return 1;
		}

		if( luaL_loadfile( L, filename.c_str() ) != LUA_OK )
			raiseError = true;
	}

	if( raiseError )
		return lua_error( L );

	return 1;
}

/*****************************************************************************/
/*  Class with re-usable functions for binding co::CompoundTypes to Lua      */
/*****************************************************************************/

void CompoundTypeBinding::getInstance( lua_State* L, int udataIdx, co::Any& instance )
{
	tryGetInstance( L, udataIdx, instance );
	if( !instance.isValid() )
		throw lua::Exception( "unknown userdata type" );
}

void CompoundTypeBinding::tryGetInstance( lua_State* L, int udataIdx, co::Any& instance )
{
	assert( !instance.isValid() );

	co::CompoundType* ct = getType( L, udataIdx );
	if( !ct )
		return;

	void* ud = lua_touserdata( L, udataIdx );
	switch( ct->getKind() )
	{
	case co::TK_STRUCT:
	case co::TK_NATIVECLASS:
		instance.setVariable( ct, co::Any::VarIsReference, ud );
		break;
	case co::TK_INTERFACE:
		instance.setInterface( *reinterpret_cast<co::Interface**>( ud ) );
		break;
	default:
		assert( false );
	}
}

struct Metamethods
{
	lua_CFunction index;
	lua_CFunction newIndex;
	lua_CFunction gc;
};

static const Metamethods METAMETHODS_TABLE[] = {
	// struct
	{ StructBinding::index, StructBinding::newIndex, StructBinding::gc },
	// native class
	{ NativeClassBinding::index, NativeClassBinding::newIndex, NativeClassBinding::gc },
	// interface
	{ InterfaceBinding::index, InterfaceBinding::newIndex, InterfaceBinding::gc },
	// component
	{ ComponentBinding::index, ComponentBinding::newIndex, ComponentBinding::gc }
};

void CompoundTypeBinding::pushMetatable( lua_State* L, co::CompoundType* ct )
{
	assert( ct );
	lua_pushlightuserdata( L, ct );
	lua_rawget( L, LUA_REGISTRYINDEX );

	// type doesn't have a metatable yet?
	if( lua_isnil( L, -1 ) )
	{
		co::TypeKind tag = ct->getKind();
		if( tag == co::TK_INTERFACE && ct->getFullName() == "co.Component" )
			tag = co::TK_COMPONENT;

		assert( tag >= co::TK_STRUCT && tag <= co::TK_COMPONENT );
		const Metamethods& mms = METAMETHODS_TABLE[tag - co::TK_STRUCT];

		lua_pop( L, 1 );
		lua_createtable( L, 1, 3 );

		lua_pushlightuserdata( L, ct );
		lua_rawseti( L, -2, 1 );

		assert( mms.index );
		lua_pushliteral( L, "__index" );
		lua_pushcfunction( L, mms.index );
		lua_rawset( L, -3 );

		assert( mms.newIndex );
		lua_pushliteral( L, "__newindex" );
		lua_pushcfunction( L, mms.newIndex );
		lua_rawset( L, -3 );

		assert( mms.gc );
		lua_pushliteral( L, "__gc" );
		lua_pushcfunction( L, mms.gc );
		lua_rawset( L, -3 );

		// save the metatable in the registry
		lua_pushlightuserdata( L, ct );
		lua_pushvalue( L, -2 );
		lua_rawset( L, LUA_REGISTRYINDEX );
	}
}

co::CompoundType* CompoundTypeBinding::getType( lua_State* L, int udataIdx )
{
	if( !lua_isuserdata( L, udataIdx ) )
		return NULL;

	lua_getmetatable( L, udataIdx );
	lua_rawgeti( L, -1, 1 );

	co::CompoundType* ct = reinterpret_cast<co::CompoundType*>( lua_touserdata( L, -1 ) );
	lua_pop( L, 2 );

	return ct;
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

	// remove the metatable
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
	co::Any any; Variant var;
	LuaState::instance().toCoral( -1, ai->getType(), any, var );
	reflector->setAttribute( instance, ai, any );
	lua_pop( L, 2 );
}

int CompoundTypeBinding::callMethod( lua_State* L )
{
	// get the co::MethodInfo* from upvalue 1
	co::MethodInfo* mi = static_cast<co::MethodInfo*>(
							reinterpret_cast<co::MemberInfo*>(
								lua_touserdata( L, lua_upvalueindex( 1 ) ) ) );

	// check the passed instance
	co::CompoundType* objectType = mi->getOwner();

	co::Any instance;
	tryGetInstance( L, 1, instance );
	if( !instance.isValid() ||
		( instance.getType() != objectType &&
			( instance.getKind() != co::TK_INTERFACE || objectType->getKind() != co::TK_INTERFACE ||
				!instance.getInterfaceType()->isSubTypeOf( static_cast<co::InterfaceType*>( objectType ) ) ) ) )
	{
		const char* typeName = ( instance.isValid() ? instance.getType()->getFullName().c_str() : luaL_typename( L, 1 ) );
		luaL_error( L, "bad instance for method '%s' (%s expected, got %s)",
			mi->getName().c_str(), objectType->getFullName().c_str(), typeName );
	}

	// get the method's list of parameters
	co::ArrayRange<co::ParameterInfo* const> paramList = mi->getParameters();

	/*
		We currently set a hardcoded limit on the number of method parameters.
		Supporting an unbounded number of parameters would degrade performance.
		Generally any method with more than 8 parameters should be refactored.
	 */
	const int MAX_NUM_PARAMETERS = 8;
	int numParams = static_cast<int>( paramList.getSize() );
	if( numParams > MAX_NUM_PARAMETERS )
		luaL_error( L, "method '%s' exceeds the hardcoded limit of %d parameters",
						mi->getName().c_str(), MAX_NUM_PARAMETERS );

	__BEGIN_EXCEPTIONS_BARRIER__

	// prepare the argument list

	co::Any args[MAX_NUM_PARAMETERS];
	Variant data[MAX_NUM_PARAMETERS];
	
	LuaState& luaState = LuaState::instance();

	int numRequiredParams = numParams;
	int i = 0;

	try
	{
		for( ; i < numParams; ++i )
		{
			co::ParameterInfo* paramInfo = paramList[i];
			co::Type* paramType = paramInfo->getType();

			if( paramInfo->getIsIn() )
				luaState.toCoral( i + 2, paramType, args[i], data[i] );
			else
				--numRequiredParams;

			if( paramInfo->getIsOut() )
				data[i].makeOut( paramType, args[i] );
		}
	}
	catch( const co::Exception& e )
	{
		lua_pushfstring( L, "bad argument %d to method '%s': %s",
							i + 1, mi->getName().c_str(), e.getMessage().c_str() );
		throw lua::MsgOnStackException();
	}

	// check the number of required/passed parameters
	int numPassedParams = lua_gettop( L ) - 1;
	if( numRequiredParams > numPassedParams )
	{
		lua_pushfstring( L, "insufficient number of arguments to method '%s' (%d expected, got %d)",
						mi->getName().c_str(), numRequiredParams, numPassedParams );
		throw lua::MsgOnStackException();
	}

	// invoke the method

	co::Any returnValue;
	co::Reflector* reflector = objectType->getReflector();
	reflector->invokeMethod( instance, mi, co::ArrayRange<co::Any const>( args, numParams ), returnValue );

	// return result and output parameters

	int numOut = 0;
	if( mi->getReturnType() != NULL )
	{
		++numOut;
		luaState.push( returnValue );
	}

	for( i = 0; i < numParams; ++i )
	{
		if( paramList[i]->getIsOut() )
		{
			++numOut;
			data[i].makeIn( args[i] );
			luaState.push( args[i] );
		}
	}

	return numOut;

	__END_EXCEPTIONS_BARRIER__
}

/*****************************************************************************/
/*  Helper class for binding co::Components to Lua                           */
/*****************************************************************************/

void ComponentBinding::create( lua_State* L, co::Component* component )
{
	// create the userdata
	co::Component** ud = reinterpret_cast<co::Component**>( lua_newuserdata( L, sizeof(co::Component*) ) );

	*ud = component;
	component->componentRetain();

	// set the userdata's metatable
	pushMetatable( L, component->getComponentType() );
	lua_setmetatable( L, -2 );
}

inline co::InterfaceInfo* checkInterfaceInfo( lua_State* L, int index )
{
	assert( lua_islightuserdata( L, index ) );
	co::MemberInfo* mi = reinterpret_cast<co::MemberInfo*>( lua_touserdata( L, index ) );
	return static_cast<co::InterfaceInfo*>( mi );
}

inline bool isStringComponentType( lua_State* L, int index )
{
	size_t strlen;
	const char* str = lua_tolstring( L, index, &strlen );
	return ( strlen == 13 && strncmp( str, "componentType", strlen ) == 0 );
}

int ComponentBinding::index( lua_State* L )
{
	co::Component** ud = reinterpret_cast<co::Component**>( lua_touserdata( L, 1 ) );
	assert( ud );
	assert( lua_isstring( L, 2 ) );

	__BEGIN_EXCEPTIONS_BARRIER__

	co::Component* component = *ud;

	if( isStringComponentType( L, 2 ) )
	{
		LuaState::instance().push( component->getComponentType() );
		return 1;
	}

	pushMember( L, component->getComponentType() );
	assert( lua_islightuserdata( L, -1 ) );

	co::InterfaceInfo* itfInfo = checkInterfaceInfo( L, -1 );
	co::Interface* itf = component->getInterface( itfInfo );
	LuaState::instance().push( itf );

	return 1;

	__END_EXCEPTIONS_BARRIER__
}

int ComponentBinding::newIndex( lua_State* L )
{
	co::Component** ud = reinterpret_cast<co::Component**>( lua_touserdata( L, 1 ) );
	assert( ud );
	assert( lua_isstring( L, 2 ) );

	__BEGIN_EXCEPTIONS_BARRIER__

	if( isStringComponentType( L, 2 ) )
		throw co::IllegalArgumentException( "attribute 'componentType' is read-only and cannot be changed" );

	co::Component* component = *ud;
	pushMember( L, component->getInterfaceType() );
	if( lua_islightuserdata( L, -1 ) )
	{
		co::InterfaceInfo* itfInfo = checkInterfaceInfo( L, -1 );
		if( itfInfo->getIsProvided() )
		{
			lua_pushliteral( L, "'" );
			lua_pushvalue( L, 2 );
			lua_pushliteral( L, "' is a provided interface and cannot be set" );
			lua_concat( L, 3 );
			throw lua::MsgOnStackException();
		}
		co::Any any; Variant var;
		LuaState::instance().toCoral( 3, itfInfo->getInterfaceType(), any, var );
		component->bindInterface( itfInfo, any.get<co::Interface*>() );
	}

	return 0;

	__END_EXCEPTIONS_BARRIER__
}

int ComponentBinding::gc( lua_State* L )
{
	co::Interface** ud = reinterpret_cast<co::Interface**>( lua_touserdata( L, 1 ) );
	assert( ud );

	__BEGIN_EXCEPTIONS_BARRIER__

	( *ud )->componentRelease();

	return 0;

	__END_EXCEPTIONS_BARRIER__
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
	pushMetatable( L, itf->getInterfaceType() );
	lua_setmetatable( L, -2 );
}

int InterfaceBinding::index( lua_State* L )
{
	co::Interface** ud = reinterpret_cast<co::Interface**>( lua_touserdata( L, 1 ) );
	assert( ud );
	assert( lua_isstring( L, 2 ) );

	__BEGIN_EXCEPTIONS_BARRIER__

	co::Interface* itf = *ud;
	pushMember( L, itf->getInterfaceType() );
	if( lua_islightuserdata( L, -1 ) )
		getAttribute( L, itf );

	return 1;

	__END_EXCEPTIONS_BARRIER__
}

int InterfaceBinding::newIndex( lua_State* L )
{
	co::Interface** ud = reinterpret_cast<co::Interface**>( lua_touserdata( L, 1 ) );
	assert( ud );
	assert( lua_isstring( L, 2 ) );

	__BEGIN_EXCEPTIONS_BARRIER__

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
	co::Interface** ud = reinterpret_cast<co::Interface**>( lua_touserdata( L, 1 ) );
	assert( ud );

	__BEGIN_EXCEPTIONS_BARRIER__

	( *ud )->componentRelease();

	return 0;

	__END_EXCEPTIONS_BARRIER__
}

/*****************************************************************************/
/*  Re-usable class for binding complex values to Lua                        */
/*****************************************************************************/

void ComplexValueBinding::push( lua_State* L, co::Type* type, void* instancePtr )
{
	assert( type->getKind() == co::TK_STRUCT || type->getKind() == co::TK_NATIVECLASS );

	co::Reflector* reflector = type->getReflector();
	reflector->componentRetain();

	// create the userdata
	size_t size = reflector->getSize();
	void* ud = lua_newuserdata( L, size );
	reflector->createValue( ud, size );

	// if an instance was provided, copy it
	if( instancePtr )
		reflector->copyValue( instancePtr, ud );

	// set the userdata's metatable
	pushMetatable( L, static_cast<co::CompoundType*>( type ) );
	lua_setmetatable( L, -2 );
}

int ComplexValueBinding::gc( lua_State* L )
{	
	__BEGIN_EXCEPTIONS_BARRIER__

	co::CompoundType* ct = getType( L, 1 );
	assert( ct );

	co::Reflector* reflector = ct->getReflector();
	reflector->destroyValue( lua_touserdata( L, 1 ) );

	reflector->componentRelease();

	return 0;

	__END_EXCEPTIONS_BARRIER__
}

/*****************************************************************************/
/*  Helper class for binding native classes to Lua                           */
/*****************************************************************************/

int NativeClassBinding::index( lua_State* L )
{
	assert( lua_isstring( L, 2 ) );

	__BEGIN_EXCEPTIONS_BARRIER__

	co::CompoundType* ct = getType( L, 1 );
	assert( ct );

	pushMember( L, ct );
	if( lua_islightuserdata( L, -1 ) )
		getAttribute( L, co::Any( ct, co::Any::VarIsReference, lua_touserdata( L, 1 ) ) );

	return 1;

	__END_EXCEPTIONS_BARRIER__
}

int NativeClassBinding::newIndex( lua_State* L )
{
	assert( lua_isstring( L, 2 ) );

	__BEGIN_EXCEPTIONS_BARRIER__

	co::CompoundType* ct = getType( L, 1 );
	assert( ct );

	pushMember( L, ct );
	if( lua_isfunction( L, -1 ) )
	{
		lua_pushliteral( L, "'" );
		lua_pushvalue( L, 2 );
		lua_pushliteral( L, "' is a method and cannot be overwritten" );
		lua_concat( L, 3 );
		throw lua::MsgOnStackException();
	}

	lua_pushvalue( L, 3 );
	setAttribute( L, co::Any( ct, co::Any::VarIsReference, lua_touserdata( L, 1 ) ) );

	return 0;

	__END_EXCEPTIONS_BARRIER__
}

/*****************************************************************************/
/*  Helper class for binding structs to Lua                                  */
/*****************************************************************************/

int StructBinding::index( lua_State* L )
{
	assert( lua_isstring( L, 2 ) );

	__BEGIN_EXCEPTIONS_BARRIER__

	co::CompoundType* ct = getType( L, 1 );
	assert( ct );

	pushMember( L, ct );
	assert( lua_islightuserdata( L, -1 ) );

	getAttribute( L, co::Any( ct, co::Any::VarIsPointer, lua_touserdata( L, 1 ) ) );

	return 1;

	__END_EXCEPTIONS_BARRIER__
}

int StructBinding::newIndex( lua_State* L )
{
	assert( lua_isstring( L, 2 ) );

	__BEGIN_EXCEPTIONS_BARRIER__

	co::CompoundType* ct = getType( L, 1 );
	assert( ct );

	pushMember( L, ct );
	assert( lua_islightuserdata( L, -1 ) );

	lua_pushvalue( L, 3 );
	setAttribute( L, co::Any( ct, co::Any::VarIsPointer, lua_touserdata( L, 1 ) ) );

	return 0;

	__END_EXCEPTIONS_BARRIER__
}
