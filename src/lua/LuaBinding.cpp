/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#include "LuaBinding.h"
#include "LuaState.h"
#include "LuaComponent.h"
#include <co/System.h>
#include <co/Component.h>
#include <co/Reflector.h>
#include <co/MethodInfo.h>
#include <co/AttributeInfo.h>
#include <co/InterfaceInfo.h>
#include <co/ParameterInfo.h>
#include <co/ModuleLoadException.h>
#include <co/IllegalArgumentException.h>
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

coPackage::ComponentTypeList coPackage::sm_luaComponentTypes;

void coPackage::open( lua_State* L )
{
	static luaL_Reg libFunctions[] = {
		{ "addPath", addPath },
		{ "getPaths", getPaths },
		{ "findScript", findScript },
		{ "getType", getType },
		{ "new", genericNew },
		{ "newComponentType", newComponentType },
		{ "newComponentInstance", newComponentInstance },
		{ NULL, NULL }
	};

	// create our lib table, containing the libFunctions
	luaL_newlib( L, libFunctions );

	// set the 'co.version' constant
	lua_pushliteral( L, CORAL_VERSION_STR );
	lua_setfield( L, -2, "version" );

	// set the 'co.buildKey' constant
	lua_pushliteral( L, CORAL_BUILD_KEY );
	lua_setfield( L, -2, "buildKey" );

	// set the 'co.buildMode' constant
	lua_pushliteral( L, CORAL_BUILD_MODE );
	lua_setfield( L, -2, "buildMode" );

	// set the 'co.compilerOutputRevision' constant
	lua_pushinteger( L, CORAL_COMPILER_OUTPUT_REVISION );
	lua_setfield( L, -2, "compilerOutputRevision" );

	// set the 'co.system' singleton
	LuaState::push( L, co::getSystem() );
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

	// release all LuaComponent reflectors
	size_t numComponentTypes = sm_luaComponentTypes.size();
	for( size_t i = 0; i < numComponentTypes; ++i )
		sm_luaComponentTypes[i]->setReflector( NULL );

	// release all type bindings
	CompoundTypeBinding::releaseBindings( L );
}

int coPackage::addPath( lua_State* L )
{
	int numArgs = lua_gettop( L );
	for( int i = 1; i <= numArgs; ++i )
	{
		const char* dirList = luaL_checkstring( L, i );
		co::addPath( dirList );
	}
	return 0;
}

int coPackage::getPaths( lua_State* L )
{
	__BEGIN_EXCEPTIONS_BARRIER__

	LuaState::push( L, co::getPaths() );

	return 1;

	__END_EXCEPTIONS_BARRIER__
}

int coPackage::findScript( lua_State* L )
{
	const char* scriptName = luaL_checkstring( L, 1 );
	std::string filename;
	if( LuaState::findScript( L, scriptName, filename ) )
		lua_pushlstring( L, filename.data(), filename.length() );
	else
		lua_pushnil( L );
	return 1;
}

int coPackage::getType( lua_State* L )
{
	const char* typeName = luaL_checkstring( L, 1 );

	__BEGIN_EXCEPTIONS_BARRIER__

	co::Type* type = co::getType( typeName );
	LuaState::push( L, type );

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
		LuaState::push( L, c.get() );
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

int coPackage::newComponentType( lua_State* L )
{
	luaL_checktype( L, 2, LUA_TTABLE );

	__BEGIN_EXCEPTIONS_BARRIER__

	co::Any any;
	CompoundTypeBinding::getInstance( L, 1, any );
	co::ComponentType* ct = any.get<co::ComponentType*>();
	sm_luaComponentTypes.push_back( ct );

	lua_pushvalue( L, 2 );
	int tableRef = luaL_ref( L, LUA_REGISTRYINDEX );

	LuaComponent* lc = new LuaComponent;
	lc->setComponentType( ct, tableRef );

	LuaState::push( L, static_cast<co::Reflector*>( lc ) );

	return 1;

	__END_EXCEPTIONS_BARRIER__
}

int coPackage::newComponentInstance( lua_State* L )
{
	luaL_checktype( L, 2, LUA_TTABLE );

	__BEGIN_EXCEPTIONS_BARRIER__

	co::Any any;
	CompoundTypeBinding::getInstance( L, 1, any );
	LuaComponent* prototype = dynamic_cast<LuaComponent*>( any.get<co::Reflector*>() );
	if( !prototype )
		throw lua::Exception( "bad argument #1 to co.newComponentInstance (lua.Component.reflector expected)" );

	lua_pushvalue( L, 2 );
	int tableRef = luaL_ref( L, LUA_REGISTRYINDEX );

	LuaComponent* lc = new LuaComponent;
	lc->setComponentInstance( prototype, tableRef );

	LuaState::push( L, static_cast<co::Component*>( lc ) );

	return 1;

	__END_EXCEPTIONS_BARRIER__
}

/*****************************************************************************/
/*  Class with re-usable functions for binding co::CompoundTypes to Lua      */
/*****************************************************************************/

CompoundTypeBinding::CompoundTypeList CompoundTypeBinding::sm_boundTypes;

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

void CompoundTypeBinding::getInstance( lua_State* L, int udataIdx, co::Any& instance )
{
	if( !tryGetInstance( L, udataIdx, instance ) )
		throw lua::Exception( "unknown userdata type" );
}

bool CompoundTypeBinding::tryGetInstance( lua_State* L, int udataIdx, co::Any& instance )
{
	co::CompoundType* ct = getType( L, udataIdx );
	if( !ct )
		return false;

	switch( ct->getKind() )
	{
	case co::TK_STRUCT:
	case co::TK_NATIVECLASS:
		instance.setVariable( ct, co::Any::VarIsReference, ComplexValueBinding::getInstance( L, udataIdx ) );
		break;
	case co::TK_INTERFACE:
	case co::TK_COMPONENT:
		instance.setInterface( InterfaceBinding::getInstance( L, udataIdx ) );
		break;
	default:
		assert( false );
		return false;
	}

	return true;
}

void CompoundTypeBinding::releaseBindings( lua_State* L )
{
	for( CompoundTypeList::iterator it = sm_boundTypes.begin(); it != sm_boundTypes.end(); ++it )
	{
		co::CompoundType* ct = *it;
		lua_pushlightuserdata( L, ct );
		lua_pushnil( L );
		lua_rawset( L, LUA_REGISTRYINDEX );
	}
	sm_boundTypes.clear();
}

struct Metamethods
{
	lua_CFunction index;
	lua_CFunction newIndex;
	lua_CFunction gc;
	lua_CFunction toString;
};

static const Metamethods METAMETHODS_TABLE[] = {
	{ StructBinding::index, StructBinding::newIndex, StructBinding::gc, StructBinding::toString },
	{ NativeClassBinding::index, NativeClassBinding::newIndex, NativeClassBinding::gc, NativeClassBinding::toString },
	{ InterfaceBinding::index, InterfaceBinding::newIndex, InterfaceBinding::gc, InterfaceBinding::toString },
	{ ComponentBinding::index, ComponentBinding::newIndex, ComponentBinding::gc, ComponentBinding::toString }
};

void CompoundTypeBinding::pushMetatable( lua_State* L, co::CompoundType* ct, co::Reflector* reflector )
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
		lua_createtable( L, 1, 4 );

		// save the CompoundType at MT[1]
		lua_pushlightuserdata( L, ct );
		lua_rawseti( L, -2, 1 );

		// if a Reflector was provided, save it at MT[2]
		if( reflector )
		{
			lua_pushlightuserdata( L, reflector );
			lua_rawseti( L, -2, 2 );
		}

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

		assert( mms.toString );
		lua_pushliteral( L, "__tostring" );
		lua_pushcfunction( L, mms.toString );
		lua_rawset( L, -3 );

		// save the metatable in the registry
		lua_pushlightuserdata( L, ct );
		lua_pushvalue( L, -2 );
		lua_rawset( L, LUA_REGISTRYINDEX );

		sm_boundTypes.push_back( ct );
	}
}

bool CompoundTypeBinding::pushMember( lua_State* L, co::CompoundType* ct, bool mustExist )
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
				if( mustExist )
				{
					lua_pushliteral( L, "non-existing member '" );
					lua_pushvalue( L, 2 );
					lua_pushliteral( L, "'" );
					lua_concat( L, 3 );
					throw lua::MsgOnStackException();
				}
				else
				{
					lua_pushnil( L );
					return false;
				}
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
	
	return true;
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
	LuaState::push( L, value );
}

void CompoundTypeBinding::setAttribute( lua_State* L, const co::Any& instance )
{
	co::AttributeInfo* ai = checkAttributeInfo( L, -2 );
	co::Reflector* reflector = ai->getOwner()->getReflector();
	co::Any value;
	LuaState::getAny( L, -1, ai->getType(), value );
	reflector->setAttribute( instance, ai, value );
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

	int numRequiredArgs = numParams;
	int numPassedArgs = lua_gettop( L ) - 1;
	int i = 0;

	try
	{
		for( ; i < numParams; ++i )
		{
			co::ParameterInfo* paramInfo = paramList[i];
			co::Type* paramType = paramInfo->getType();

			if( paramInfo->getIsIn() )
			{
				if( numPassedArgs <= i )
				{
					// a required argument was not passed, but we don't break the loop
					// because we want to compute numRequiredArgs
					continue; 
				}

				if( paramInfo->getIsOut() )
				{
					// inout: allocate an 'out' var and set it with the 'in' value.
					args[i].makeOut( paramType );
					LuaState::getValue( L, i + 2, args[i] );
				}
				else // only 'in'
				{
					LuaState::getAny( L, i + 2, paramType, args[i] );
				}
			}
			else // only 'out'
			{
				--numRequiredArgs;
				args[i].makeOut( paramType );				
			}
		}
	}
	catch( const co::Exception& e )
	{
		lua_pushfstring( L, "bad argument #%d to method '%s' (%s)",
							i + 1, mi->getName().c_str(), e.getMessage().c_str() );
		throw lua::MsgOnStackException();
	}

	// check the number of required/passed parameters
	if( numRequiredArgs > numPassedArgs )
	{
		lua_pushfstring( L, "insufficient number of arguments to method '%s' (%d expected, got %d)",
							mi->getName().c_str(), numRequiredArgs, numPassedArgs );
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
		LuaState::push( L, returnValue );
	}

	for( i = 0; i < numParams; ++i )
	{
		if( paramList[i]->getIsOut() )
		{
			++numOut;
			args[i].makeIn();
			LuaState::push( L, args[i] );
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
		LuaState::push( L, component->getComponentType() );
		return 1;
	}

	if( pushMember( L, component->getComponentType() ) )
	{
		assert( lua_islightuserdata( L, -1 ) );
		co::InterfaceInfo* itfInfo = checkInterfaceInfo( L, -1 );
		co::Interface* itf = component->getInterface( itfInfo );
		LuaState::push( L, itf );
	}

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
	pushMember( L, component->getComponentType(), true );
	if( lua_islightuserdata( L, -1 ) )
	{
		co::InterfaceInfo* itfInfo = checkInterfaceInfo( L, -1 );
		if( itfInfo->getIsFacet() )
		{
			lua_pushliteral( L, "'" );
			lua_pushvalue( L, 2 );
			lua_pushliteral( L, "' is a facet and cannot be set" );
			lua_concat( L, 3 );
			throw lua::MsgOnStackException();
		}
		co::Interface* itf;
		co::Any any;
		any.setVariable( itfInfo->getType(), co::Any::VarIsPointer|co::Any::VarIsReference, &itf );
		LuaState::getValue( L, 3, any );
		component->bindInterface( itfInfo, itf );
	}

	return 0;

	__END_EXCEPTIONS_BARRIER__
}

int ComponentBinding::gc( lua_State* L )
{
	co::Component** ud = reinterpret_cast<co::Component**>( lua_touserdata( L, 1 ) );
	assert( ud );

	__BEGIN_EXCEPTIONS_BARRIER__

	( *ud )->componentRelease();

	return 0;

	__END_EXCEPTIONS_BARRIER__
}

int ComponentBinding::toString( lua_State* L )
{
	co::Component** ud = reinterpret_cast<co::Component**>( lua_touserdata( L, 1 ) );
	assert( ud );

	co::Component* comp = *ud;
	lua_pushfstring( L, "%s: %p", comp->getComponentType()->getFullName().c_str(), comp );

	return 1;
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
	if( pushMember( L, itf->getInterfaceType() ) )
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
	pushMember( L, itf->getInterfaceType(), true );
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

int InterfaceBinding::toString( lua_State* L )
{
	co::Interface** ud = reinterpret_cast<co::Interface**>( lua_touserdata( L, 1 ) );
	assert( ud );

	co::Interface* itf = *ud;
	lua_pushfstring( L, "%s: %p", itf->getInterfaceType()->getFullName().c_str(), itf );

	return 1;
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
	pushMetatable( L, static_cast<co::CompoundType*>( type ), reflector );
	lua_setmetatable( L, -2 );
}

int ComplexValueBinding::gc( lua_State* L )
{	
	__BEGIN_EXCEPTIONS_BARRIER__

	lua_getmetatable( L, 1 );
	lua_rawgeti( L, -1, 2 );
	assert( lua_islightuserdata( L, -1 ) );

	co::Reflector* reflector = reinterpret_cast<co::Reflector*>( lua_touserdata( L, -1 ) );
	reflector->destroyValue( lua_touserdata( L, 1 ) );
	reflector->componentRelease();

	return 0;

	__END_EXCEPTIONS_BARRIER__
}

int ComplexValueBinding::toString( lua_State* L )
{
	co::CompoundType* ct = getType( L, 1 );
	assert( ct );

	lua_pushfstring( L, "%s: %p", ct->getFullName().c_str(), lua_touserdata( L, 1 ) );

	return 1;
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

	if( pushMember( L, ct ) )
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

	pushMember( L, ct, true );
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

	if( pushMember( L, ct ) )
	{
		assert( lua_islightuserdata( L, -1 ) );
		getAttribute( L, co::Any( ct, co::Any::VarIsPointer, lua_touserdata( L, 1 ) ) );
	}

	return 1;

	__END_EXCEPTIONS_BARRIER__
}

int StructBinding::newIndex( lua_State* L )
{
	assert( lua_isstring( L, 2 ) );

	__BEGIN_EXCEPTIONS_BARRIER__

	co::CompoundType* ct = getType( L, 1 );
	assert( ct );

	pushMember( L, ct, true );
	assert( lua_islightuserdata( L, -1 ) );

	lua_pushvalue( L, 3 );
	setAttribute( L, co::Any( ct, co::Any::VarIsPointer, lua_touserdata( L, 1 ) ) );

	return 0;

	__END_EXCEPTIONS_BARRIER__
}
