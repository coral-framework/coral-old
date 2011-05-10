/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#include "LuaBinding.h"
#include "LuaState.h"
#include "LuaComponent.h"
#include <co/ISystem.h>
#include <co/IObject.h>
#include <co/IReflector.h>
#include <co/IMethod.h>
#include <co/IField.h>
#include <co/IPort.h>
#include <co/IParameter.h>
#include <co/ModuleLoadException.h>
#include <co/IllegalArgumentException.h>
#include <lua/Exception.h>
#include <cstring>
#include <sstream>

namespace lua {

/*****************************************************************************/
/*  Helper Macros to Shield Lua from C++ Exceptions                          */
/*****************************************************************************/

#define __BEGIN_EXCEPTIONS_BARRIER__ try {

#define __END_EXCEPTIONS_BARRIER__ \
	} catch( ... ) { handleException( L ); } return lua_error( L );


/*****************************************************************************/
/*  Helper Function to Handle C++ Exceptions and Push a Lua Error String     */
/*****************************************************************************/

class MsgOnStackException {};

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
		lua_pushfstring( L, "{%s}%s", e.getTypeName(), e.getMessage().c_str() );
	}
    catch( const std::exception& e )
    {
		lua_pushfstring( L, "std::exception: %s", e.what() );
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
		{ "findModuleFile", findModuleFile },
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
	CompositeTypeBinding::releaseBindings( L );
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

int coPackage::findModuleFile( lua_State* L )
{
	const char* moduleName = luaL_checkstring( L, 1 );
	const char* fileName = luaL_checkstring( L, 2 );
	std::string path;
	if( co::findModuleFile( moduleName, fileName, path ) )
		lua_pushlstring( L, path.data(), path.length() );
	else
		lua_pushnil( L );
	return 1;
}

int coPackage::getType( lua_State* L )
{
	const char* typeName = luaL_checkstring( L, 1 );

	__BEGIN_EXCEPTIONS_BARRIER__

	co::IType* type = co::getType( typeName );
	LuaState::push( L, type );

	return 1;

	__END_EXCEPTIONS_BARRIER__
}

int coPackage::genericNew( lua_State* L )
{
	const char* typeName = luaL_checkstring( L, 1 );

	__BEGIN_EXCEPTIONS_BARRIER__

	co::IType* type = co::getType( typeName );
	co::TypeKind kind = type->getKind();
	if( kind == co::TK_STRUCT || kind == co::TK_NATIVECLASS )
	{
		ComplexValueBinding::push( L, type, NULL );
	}
	else if( kind == co::TK_COMPONENT )
	{
		co::IReflector* reflector = type->getReflector();
		co::RefPtr<co::IObject> c = reflector->newInstance();
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
	CompositeTypeBinding::getInstance( L, 1, any );
	co::IComponent* ct = any.get<co::IComponent*>();
	sm_luaComponentTypes.push_back( ct );

	lua_pushvalue( L, 2 );
	int tableRef = luaL_ref( L, LUA_REGISTRYINDEX );

	LuaComponent* lc = new LuaComponent;
	lc->setComponentType( ct, tableRef );

	LuaState::push( L, static_cast<co::IReflector*>( lc ) );

	return 1;

	__END_EXCEPTIONS_BARRIER__
}

int coPackage::newComponentInstance( lua_State* L )
{
	luaL_checktype( L, 2, LUA_TTABLE );

	__BEGIN_EXCEPTIONS_BARRIER__

	co::Any any;
	CompositeTypeBinding::getInstance( L, 1, any );
	co::IReflector* prototype = any.get<co::IReflector*>();
	if( !prototype )
		throw lua::Exception( "bad argument #1 to co.newComponentInstance (reflector expected)" );

	assert( prototype->getFacet()->getOwner()->getFullName() == "lua.Component" );

	lua_pushvalue( L, 2 );
	int tableRef = luaL_ref( L, LUA_REGISTRYINDEX );

	LuaComponent* lc = new LuaComponent;
	lc->setComponentInstance( static_cast<LuaComponent*>( prototype ), tableRef );

	LuaState::push( L, static_cast<co::IObject*>( lc ) );

	return 1;

	__END_EXCEPTIONS_BARRIER__
}

/*****************************************************************************/
/*  Class with re-usable functions for binding co::CompositeTypes to Lua     */
/*****************************************************************************/

CompositeTypeBinding::CompositeTypeList CompositeTypeBinding::sm_boundTypes;

co::ICompositeType* CompositeTypeBinding::getType( lua_State* L, int udataIdx )
{
	if( !lua_isuserdata( L, udataIdx ) )
		return NULL;

	lua_getmetatable( L, udataIdx );
	lua_rawgeti( L, -1, 1 );

	co::ICompositeType* ct = reinterpret_cast<co::ICompositeType*>( lua_touserdata( L, -1 ) );
	lua_pop( L, 2 );

	return ct;
}

void CompositeTypeBinding::getInstance( lua_State* L, int udataIdx, co::Any& instance )
{
	if( !tryGetInstance( L, udataIdx, instance ) )
		throw lua::Exception( "unknown userdata type" );
}

bool CompositeTypeBinding::tryGetInstance( lua_State* L, int udataIdx, co::Any& instance )
{
	co::ICompositeType* ct = getType( L, udataIdx );
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
		instance.setService( ServiceBinding::getInstance( L, udataIdx ) );
		break;
	default:
		assert( false );
		return false;
	}

	return true;
}

void CompositeTypeBinding::releaseBindings( lua_State* L )
{
	for( CompositeTypeList::iterator it = sm_boundTypes.begin(); it != sm_boundTypes.end(); ++it )
	{
		co::ICompositeType* ct = *it;
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
	{ ServiceBinding::index, ServiceBinding::newIndex, ServiceBinding::gc, ServiceBinding::toString },
	{ ObjectBinding::index, ObjectBinding::newIndex, ObjectBinding::gc, ObjectBinding::toString }
};

void CompositeTypeBinding::pushMetatable( lua_State* L, co::ICompositeType* ct, co::IReflector* reflector )
{
	assert( ct );
	lua_pushlightuserdata( L, ct );
	lua_rawget( L, LUA_REGISTRYINDEX );

	// type doesn't have a metatable yet?
	if( lua_isnil( L, -1 ) )
	{
		co::TypeKind tag = ct->getKind();
		if( tag == co::TK_INTERFACE && ct->getFullName() == "co.IObject" )
			tag = co::TK_COMPONENT;

		assert( tag >= co::TK_STRUCT && tag <= co::TK_COMPONENT );
		const Metamethods& mms = METAMETHODS_TABLE[tag - co::TK_STRUCT];

		lua_pop( L, 1 );
		lua_createtable( L, 1, 5 );

		// save the ICompositeType at MT[1]
		lua_pushlightuserdata( L, ct );
		lua_rawseti( L, -2, 1 );

		// if a reflector was provided, save it at MT[2]
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

		lua_pushliteral( L, "__coraltype" );
		lua_pushstring( L, ct->getFullName().c_str() );
		lua_rawset( L, -3 );

		// save the metatable in the registry
		lua_pushlightuserdata( L, ct );
		lua_pushvalue( L, -2 );
		lua_rawset( L, LUA_REGISTRYINDEX );

		sm_boundTypes.push_back( ct );
	}
}

bool CompositeTypeBinding::pushMember( lua_State* L, co::ICompositeType* ct, bool mustExist )
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
			co::IMember* member = ct->getMember( memberName );
			if( !member )
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
			lua_pushlightuserdata( L, member );
			if( member->getKind() == co::MK_METHOD )
				lua_pushcclosure( L, callMethod, 1 );

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
		// got a cached non-method (field or interface)
		break;

	default:
		assert( false );
	}

	// remove the metatable
	lua_remove( L, -2 );

	return true;
}

inline co::IField* checkField( lua_State* L, int index )
{
	assert( lua_islightuserdata( L, index ) );
	co::IMember* member = reinterpret_cast<co::IMember*>( lua_touserdata( L, index ) );
	return static_cast<co::IField*>( member );
}

void CompositeTypeBinding::getField( lua_State* L, const co::Any& instance )
{
	co::IField* field = checkField( L, -1 );
	co::IReflector* reflector = field->getOwner()->getReflector();
	co::Any value;
	reflector->getField( instance, field, value );
	lua_pop( L, 1 );
	LuaState::push( L, value );
}

void CompositeTypeBinding::setField( lua_State* L, const co::Any& instance )
{
	co::IField* field = checkField( L, -2 );
	co::IReflector* reflector = field->getOwner()->getReflector();
	co::Any value;
	LuaState::getAny( L, -1, field->getType(), value );
	reflector->setField( instance, field, value );
	lua_pop( L, 2 );
}

int CompositeTypeBinding::callMethod( lua_State* L )
{
	// get the co::IMethod* from upvalue 1
	co::IMethod* method = static_cast<co::IMethod*>( reinterpret_cast<co::IMember*>(
								lua_touserdata( L, lua_upvalueindex( 1 ) ) ) );

	// get the method's list of parameters
	co::Range<co::IParameter* const> paramList = method->getParameters();

	/*
		We currently set a hardcoded limit on the number of method parameters.
		Supporting an unbounded number of parameters would degrade performance.
		Generally any method with more than 8 parameters should be refactored.
	 */
	const int MAX_NUM_PARAMETERS = 8;
	int numParams = static_cast<int>( paramList.getSize() );
	if( numParams > MAX_NUM_PARAMETERS )
		luaL_error( L, "method '%s' exceeds the hardcoded limit of %d parameters",
						method->getName().c_str(), MAX_NUM_PARAMETERS );

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
			co::IParameter* paramInfo = paramList[i];
			co::IType* paramType = paramInfo->getType();

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
					i + 1, method->getName().c_str(), e.getMessage().c_str() );
		throw lua::MsgOnStackException();
	}

	// check the number of required/passed parameters
	if( numRequiredArgs > numPassedArgs )
	{
		lua_pushfstring( L, "insufficient number of arguments to method '%s' (%d expected, got %d)",
			method->getName().c_str(), numRequiredArgs, numPassedArgs );
		throw lua::MsgOnStackException();
	}

	// invoke the method
	co::Any instance, returnValue;
	tryGetInstance( L, 1, instance );
	co::IReflector* reflector = method->getOwner()->getReflector();
	reflector->invoke( instance, method, co::Range<co::Any const>( args, numParams ), returnValue );

	// return result and output parameters

	int numOut = 0;
	if( method->getReturnType() != NULL )
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
/*  Helper class for binding co::IObjects to Lua                             */
/*****************************************************************************/

void ObjectBinding::create( lua_State* L, co::IObject* object )
{
	// create the userdata
	co::IObject** ud = reinterpret_cast<co::IObject**>( lua_newuserdata( L, sizeof(co::IObject*) ) );

	*ud = object;
	object->serviceRetain();

	// set the userdata's metatable
	pushMetatable( L, object->getComponent() );
	lua_setmetatable( L, -2 );
}

inline co::IPort* checkPort( lua_State* L, int index )
{
	assert( lua_islightuserdata( L, index ) );
	co::IMember* member = reinterpret_cast<co::IMember*>( lua_touserdata( L, index ) );
	return static_cast<co::IPort*>( member );
}

inline bool isStringComponent( lua_State* L, int index )
{
	size_t length;
	const char* str = lua_tolstring( L, index, &length );
	return ( length == 9 && strncmp( str, "component", length ) == 0 );
}

int ObjectBinding::index( lua_State* L )
{
	co::IObject** ud = reinterpret_cast<co::IObject**>( lua_touserdata( L, 1 ) );
	assert( ud );
	assert( lua_isstring( L, 2 ) );

	__BEGIN_EXCEPTIONS_BARRIER__

	co::IObject* object = *ud;

	if( isStringComponent( L, 2 ) )
	{
		LuaState::push( L, object->getComponent() );
		return 1;
	}

	if( pushMember( L, object->getComponent() ) )
	{
		assert( lua_islightuserdata( L, -1 ) );
		co::IPort* port = checkPort( L, -1 );
		co::IService* service = object->getService( port );
		LuaState::push( L, service );
	}

	return 1;

	__END_EXCEPTIONS_BARRIER__
}

int ObjectBinding::newIndex( lua_State* L )
{
	co::IObject** ud = reinterpret_cast<co::IObject**>( lua_touserdata( L, 1 ) );
	assert( ud );
	assert( lua_isstring( L, 2 ) );

	__BEGIN_EXCEPTIONS_BARRIER__

	if( isStringComponent( L, 2 ) )
		throw co::IllegalArgumentException( "'component' is a facet and cannot be set" );

	co::IObject* object = *ud;
	pushMember( L, object->getComponent(), true );
	if( lua_islightuserdata( L, -1 ) )
	{
		co::IPort* port = checkPort( L, -1 );
		if( port->getIsFacet() )
		{
			lua_pushliteral( L, "'" );
			lua_pushvalue( L, 2 );
			lua_pushliteral( L, "' is a facet and cannot be set" );
			lua_concat( L, 3 );
			throw lua::MsgOnStackException();
		}
		co::IService* service;
		co::Any any;
		any.setVariable( port->getType(), co::Any::VarIsPointer|co::Any::VarIsReference, &service );
		LuaState::getValue( L, 3, any );
		object->setService( port, service );
	}

	return 0;

	__END_EXCEPTIONS_BARRIER__
}

int ObjectBinding::gc( lua_State* L )
{
	co::IObject** ud = reinterpret_cast<co::IObject**>( lua_touserdata( L, 1 ) );
	assert( ud );

	__BEGIN_EXCEPTIONS_BARRIER__

	( *ud )->serviceRelease();

	return 0;

	__END_EXCEPTIONS_BARRIER__
}

int ObjectBinding::toString( lua_State* L )
{
	co::IObject** ud = reinterpret_cast<co::IObject**>( lua_touserdata( L, 1 ) );
	assert( ud );

	co::IObject* comp = *ud;
	lua_pushfstring( L, "%s: %p", comp->getComponent()->getFullName().c_str(), comp );

	return 1;
}

/*****************************************************************************/
/*  Helper class for binding co::Interfaces to Lua                           */
/*****************************************************************************/

void ServiceBinding::create( lua_State* L, co::IService* service )
{
	// create the userdata
	co::IService** ud = reinterpret_cast<co::IService**>( lua_newuserdata( L, sizeof(co::IService*) ) );

	*ud = service;
	service->serviceRetain();

	// set the userdata's metatable
	pushMetatable( L, service->getInterface() );
	lua_setmetatable( L, -2 );
}

int ServiceBinding::index( lua_State* L )
{
	co::IService** ud = reinterpret_cast<co::IService**>( lua_touserdata( L, 1 ) );
	assert( ud );
	assert( lua_isstring( L, 2 ) );

	__BEGIN_EXCEPTIONS_BARRIER__

	co::IService* service = *ud;
	if( pushMember( L, service->getInterface() ) )
		if( lua_islightuserdata( L, -1 ) )
			getField( L, service );

	return 1;

	__END_EXCEPTIONS_BARRIER__
}

int ServiceBinding::newIndex( lua_State* L )
{
	co::IService** ud = reinterpret_cast<co::IService**>( lua_touserdata( L, 1 ) );
	assert( ud );
	assert( lua_isstring( L, 2 ) );

	__BEGIN_EXCEPTIONS_BARRIER__

	co::IService* service = *ud;
	pushMember( L, service->getInterface(), true );
	if( lua_isfunction( L, -1 ) )
	{
		lua_pushliteral( L, "'" );
		lua_pushvalue( L, 2 );
		lua_pushliteral( L, "' is a method and cannot be overwritten" );
		lua_concat( L, 3 );
		throw lua::MsgOnStackException();
	}

	lua_pushvalue( L, 3 );
	setField( L, service );

	return 0;

	__END_EXCEPTIONS_BARRIER__
}

int ServiceBinding::gc( lua_State* L )
{
	co::IService** ud = reinterpret_cast<co::IService**>( lua_touserdata( L, 1 ) );
	assert( ud );

	__BEGIN_EXCEPTIONS_BARRIER__

	( *ud )->serviceRelease();

	return 0;

	__END_EXCEPTIONS_BARRIER__
}

int ServiceBinding::toString( lua_State* L )
{
	co::IService** ud = reinterpret_cast<co::IService**>( lua_touserdata( L, 1 ) );
	assert( ud );

	co::IService* service = *ud;
	lua_pushfstring( L, "%s: %p", service->getInterface()->getFullName().c_str(), service );

	return 1;
}

/*****************************************************************************/
/*  Re-usable class for binding complex values to Lua                        */
/*****************************************************************************/

void ComplexValueBinding::push( lua_State* L, co::IType* type, void* instancePtr )
{
	assert( type->getKind() == co::TK_STRUCT || type->getKind() == co::TK_NATIVECLASS );

	co::IReflector* reflector = type->getReflector();
	reflector->serviceRetain();

	// create the userdata
	void* ud = lua_newuserdata( L, reflector->getSize() );
	reflector->createValue( ud );

	// if an instance was provided, copy it
	if( instancePtr )
		reflector->copyValue( instancePtr, ud );

	// set the userdata's metatable
	pushMetatable( L, static_cast<co::ICompositeType*>( type ), reflector );
	lua_setmetatable( L, -2 );
}

int ComplexValueBinding::gc( lua_State* L )
{
	__BEGIN_EXCEPTIONS_BARRIER__

	lua_getmetatable( L, 1 );
	lua_rawgeti( L, -1, 2 );
	assert( lua_islightuserdata( L, -1 ) );

	co::IReflector* reflector = reinterpret_cast<co::IReflector*>( lua_touserdata( L, -1 ) );
	reflector->destroyValue( lua_touserdata( L, 1 ) );
	reflector->serviceRelease();

	return 0;

	__END_EXCEPTIONS_BARRIER__
}

int ComplexValueBinding::toString( lua_State* L )
{
	co::ICompositeType* ct = getType( L, 1 );
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

	co::ICompositeType* ct = getType( L, 1 );
	assert( ct );

	if( pushMember( L, ct ) )
		if( lua_islightuserdata( L, -1 ) )
			getField( L, co::Any( ct, co::Any::VarIsReference, lua_touserdata( L, 1 ) ) );

	return 1;

	__END_EXCEPTIONS_BARRIER__
}

int NativeClassBinding::newIndex( lua_State* L )
{
	assert( lua_isstring( L, 2 ) );

	__BEGIN_EXCEPTIONS_BARRIER__

	co::ICompositeType* ct = getType( L, 1 );
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
	setField( L, co::Any( ct, co::Any::VarIsReference, lua_touserdata( L, 1 ) ) );

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

	co::ICompositeType* ct = getType( L, 1 );
	assert( ct );

	if( pushMember( L, ct ) )
	{
		assert( lua_islightuserdata( L, -1 ) );
		getField( L, co::Any( ct, co::Any::VarIsPointer, lua_touserdata( L, 1 ) ) );
	}

	return 1;

	__END_EXCEPTIONS_BARRIER__
}

int StructBinding::newIndex( lua_State* L )
{
	assert( lua_isstring( L, 2 ) );

	__BEGIN_EXCEPTIONS_BARRIER__

	co::ICompositeType* ct = getType( L, 1 );
	assert( ct );

	pushMember( L, ct, true );
	assert( lua_islightuserdata( L, -1 ) );

	lua_pushvalue( L, 3 );
	setField( L, co::Any( ct, co::Any::VarIsPointer, lua_touserdata( L, 1 ) ) );

	return 0;

	__END_EXCEPTIONS_BARRIER__
}

} // namespace lua
