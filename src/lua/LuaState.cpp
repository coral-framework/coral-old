/*
 * Coral - Lightweight C++ Component Framework
 * See Copyright Notice in Coral.h
 */

#include "LuaState.h"
#include "LuaBinding.h"
#include <co/IReflector.h>
#include <co/reserved/OS.h>
#include <co/IllegalArgumentException.h>
#include <lua/Exception.h>
#include <sstream>

extern "C" {
	#include "lfs/lfs.h"
	#include "socket/mime.h"
	#include "socket/luasocket.h"
}

namespace lua {

lua_State* LuaState::sm_L( NULL );
int LuaState::sm_instancesTableRegIdx( 0 );

LuaState::LuaState()
{
	// empty
}

LuaState::~LuaState()
{
	// empty
}

void LuaState::setup()
{
	assert( !sm_L );

	sm_L = luaL_newstate();
	assert( sm_L );

	// open all standard Lua libraries
	luaL_openlibs( sm_L );

	// pre-load all statically-linked Lua modules
	lua_getglobal( sm_L, "package" );
	lua_getfield( sm_L, -1, "preload" );

	// pre-load the LuaFileSystem library
	lua_pushcfunction( sm_L, luaopen_lfs );
	lua_setfield( sm_L, -2, "lfs" );

	// pre-load the LuaSocket libraries
	lua_pushcfunction( sm_L, luaopen_socket_core );
	lua_setfield( sm_L, -2, "socket.core" );
	lua_pushcfunction( sm_L, luaopen_mime_core );
	lua_setfield( sm_L, -2, "mime.core" );

	// create the 'co' package
	coPackage::open( sm_L );

	// clear the stack
	lua_settop( sm_L, 0 );
}

void LuaState::tearDown()
{
	// release the 'co.system' service
	coPackage::close( sm_L );

	// force a full GC cycle to ensure all __gc finalizers are called.
	lua_settop( sm_L, 0 );
	lua_gc( sm_L, LUA_GCCOLLECT, 0 );

	// close our Lua universe
	lua_close( sm_L );
	sm_L = NULL;
}

void LuaState::dumpStack( lua_State* L )
{
	int top = lua_gettop( L );
	printf( "dumpStack -- START\n" );
	for( int i = 1; i <= top; ++i )
	{
		printf( "%d\t%s\t", i, luaL_typename( L, i ) );
		switch( lua_type( L, i ) )
		{
		case LUA_TNUMBER:
			printf( "%g\n", lua_tonumber( L, i ) );
			break;
		case LUA_TSTRING:
			printf( "%s\n", lua_tostring( L, i ) );
			break;
		case LUA_TBOOLEAN:
			printf( "%s\n", lua_toboolean( L, i ) ? "true" : "false" );
			break;
		case LUA_TNIL:
			printf( "%s\n", "nil" );
			break;
		default:
			printf( "%p\n", lua_topointer( L, i ) );
			break;
		}
	}
	printf( "dumpStack -- END\n" );
}

bool LuaState::findScript( lua_State*, const std::string& name, std::string& filename )
{
	static const std::string s_extension( "lua" );

	// look for "?.lua" first, then "?/__init.lua"
	std::string filePaths[2];
	filePaths[0] = name;
	co::OS::convertDotsToDirSeps( filePaths[0] );
	filePaths[1] = filePaths[0];
	filePaths[0].append( ".lua" );
	filePaths[1].append( CORAL_OS_DIR_SEP_STR "__init.lua" );

	return co::OS::searchFile2( co::getPaths(), co::Range<const std::string>( filePaths, 2 ), filename );
}

void LuaState::loadFile( lua_State* L, const std::string& filename )
{
	int res = luaL_loadfile( L, filename.c_str() );
	if( res != LUA_OK )
		raiseException( L, res );
}

void LuaState::require( lua_State* L, const std::string& moduleName )
{
	// first we try to retrieve the value cached in 'package.loaded' directly.
	lua_getfield( L, LUA_REGISTRYINDEX, "_LOADED" );
	push( L, moduleName );
	lua_gettable( L, -2 );
	if( !lua_isnil( L, -1 ) )
	{
		lua_replace( L, -2 );
		return;
	}

	// no luck, so we resort to calling the real "require" function
	lua_pop( L, 2 );
	lua_getglobal( L, "require" );
	push( L, moduleName );
	call( L, 1, 1 );
}

static int traceback( lua_State* L )
{
	luaL_traceback( L, L, lua_tostring( L, 1 ), 1 );
	return 1;
}

void LuaState::call( lua_State* L, int numArgs, int numResults )
{
	int errFunc = lua_gettop( L ) - numArgs;
	lua_pushcfunction( L, traceback );
	lua_insert( L, errFunc );

	int res = lua_pcall( L, numArgs, numResults, errFunc );
	lua_remove( L, errFunc );

	if( res != LUA_OK )
		raiseException( L, res );
}

void LuaState::push( lua_State* L, const co::Any& var )
{
	push( L, var, 0 );
}

void LuaState::push( lua_State* L, const co::Any& var, int depth )
{
	const co::Any::State& s = var.getState();

	// only arrays or interfaces can be passed as pointers
	assert( !s.isPointer || s.kind == co::TK_INTERFACE || s.kind == co::TK_ARRAY );

	// dereference the variable, if necessary
	const co::Any::State::Data* d = &s.data;
	if( s.isReference && s.kind != co::TK_ANY && ( s.kind < co::TK_STRING || s.kind == co::TK_ENUM ) )
		d = reinterpret_cast<const co::Any::State::Data*>( d->ptr );

	switch( s.kind )
	{
	case co::TK_NONE:
		lua_pushnil( L );
		break;
	case co::TK_ANY:
		if( depth > 1 )
		{
			/*
				Since a co::Any may reference any other co::Any,
				we must limit the number of indirections to avoid cycles.
			 */
			assert( false );
			lua_pushnil( L );
			break;
		}
		push( L, var.get<const co::Any&>(), depth + 1 );
		break;
	case co::TK_BOOLEAN:
		lua_pushboolean( L, d->b );
		break;
	case co::TK_INT8:
		lua_pushnumber( L, d->i8 );
		break;
	case co::TK_UINT8:
		lua_pushnumber( L, d->u8 );
		break;
	case co::TK_INT16:
		lua_pushnumber( L, d->i16 );
		break;
	case co::TK_UINT16:
		lua_pushnumber( L, d->u16 );
		break;
	case co::TK_INT32:
		lua_pushnumber( L, d->i32 );
		break;
	case co::TK_UINT32:
		lua_pushnumber( L, d->u32 );
		break;
	case co::TK_INT64:
		lua_pushnumber( L, static_cast<lua_Number>( d->i64 ) );
		break;
	case co::TK_UINT64:
		lua_pushnumber( L, static_cast<lua_Number>( d->u64 ) );
		break;
	case co::TK_FLOAT:
		lua_pushnumber( L, d->f );
		break;
	case co::TK_DOUBLE:
		lua_pushnumber( L, d->d );
		break;
	case co::TK_STRING:
		push( L, var.get<const std::string&>() );
		break;
	case co::TK_ARRAY:
		pushArray( L, var );
		break;
	case co::TK_ENUM:
		{
			co::Range<std::string const> ids = static_cast<co::IEnum*>( s.type )->getIdentifiers();
			co::uint32 id = d->u32;
			if( id < ids.getSize() )
				push( L, ids[id] );
			else
			{
				// enum value is out of bounds
				assert( false );
				lua_pushliteral( L, "<invalid>" );
			}
		}
		break;
	case co::TK_STRUCT:
		StructBinding::push( L, static_cast<co::IStruct*>( s.type ), s.data.ptr );
		break;
	case co::TK_NATIVECLASS:
		NativeClassBinding::push( L, static_cast<co::INativeClass*>( s.type ), s.data.ptr );
		break;
	case co::TK_INTERFACE:
		push( L, reinterpret_cast<co::IService*>( s.data.ptr ) );
		break;
	default:
		assert( false );
		lua_pushnil( L );
	}
}

void LuaState::push( lua_State* L, const std::string& str )
{
	lua_pushlstring( L, str.c_str(), str.length() );
}

template<typename BindingClass, typename InstanceType>
inline void LuaState::pushInstance( lua_State* L, InstanceType* ptr )
{
	if( !ptr )
	{
		lua_pushnil( L );
		return;
	}

	// try to get an existing object (userdata) from the instances table
	pushInstancesTable( L );
	lua_pushlightuserdata( L, ptr );
	lua_rawget( L, -2 );

	// 'itf' not in the instances table yet?
	if( lua_isnil( L, -1 ) )
	{
		lua_pop( L, 1 );

		// create the userdata ...
		BindingClass::create( L, ptr );

		// ... and add it to the instances table
		lua_pushlightuserdata( L, ptr );
		lua_pushvalue( L, -2 );
		lua_rawset( L, -4 );
	}

	// remove the instances table from the stack
	lua_remove( L, -2 );
}

void LuaState::push( lua_State* L, co::IService* itf )
{
	if( itf && itf->getInterface()->getFullName() == "co.IObject" )
		push( L, static_cast<co::IObject*>( itf ) );
	else
		pushInstance<ServiceBinding, co::IService>( L, itf );
}

void LuaState::push( lua_State* L, co::IObject* object )
{
	pushInstance<ObjectBinding, co::IObject>( L, object );
}

void LuaState::getAny( lua_State* L, int index, co::IType* expectedType, co::Any& any )
{
	co::Any* var;
	co::TypeKind expectedKind = ( !expectedType ? co::TK_NONE : expectedType->getKind() );
	switch( expectedKind )
	{
	case co::TK_ANY:
		var = &any.createAny();
		break;

	case co::TK_ENUM:
		any.set( getEnumIdentifier( L, index, static_cast<co::IEnum*>( expectedType ) ) );
		return;

	case co::TK_NONE:
		expectedKind = co::TK_ANY;
		// continue

	default:
		var = &any;
	}

	switch( lua_type( L, index ) )
	{
	case LUA_TNIL:
		if( expectedKind == co::TK_INTERFACE )
		{
			var->setService( NULL, static_cast<co::IInterface*>( expectedType ) );
			break;
		}
		else if( expectedKind == co::TK_ANY )
		{
			var->clear();
			break;
		}
		throw lua::Exception( "no conversion from Lua's nil" );

	case LUA_TBOOLEAN:
		var->set<bool>( lua_toboolean( L, index ) != 0 );
		break;

	case LUA_TLIGHTUSERDATA:
		throw lua::Exception( "no conversion from a Lua light userdata" );
		break;

	case LUA_TNUMBER:
		var->set( lua_tonumber( L, index ) );
		break;

	case LUA_TSTRING:
		{
			size_t length;
			const char* cstr = lua_tolstring( L, index, &length );
			var->createString().assign( cstr, length );
		}
		break;

	case LUA_TTABLE:
		if( expectedKind == co::TK_ARRAY )
			toArray( L, index, static_cast<co::IArray*>( expectedType )->getElementType(), *var );
		else
			throw lua::Exception( expectedKind != co::TK_ANY ? "no conversion from a Lua table" :
						"unsupported conversion from a Lua table to a Coral 'any'" );
		break;

	case LUA_TFUNCTION:
		throw lua::Exception( "no conversion from a Lua function" );
		break;

	case LUA_TUSERDATA:
		CompositeTypeBinding::getInstance( L, index, *var );
		break;

	case LUA_TTHREAD:
		throw lua::Exception( "no conversion from a Lua thread" );
		break;

	default:
		assert( false );
	}
}

void LuaState::getValue( lua_State* L, int index, const co::Any& var )
{
	co::TypeKind kind = var.getKind();

	// sanity check 'var': it must be a valid output variable
	assert( kind > co::TK_NONE && kind < co::TK_COMPONENT );
	assert( kind == co::TK_ARRAY || ( var.isReference() && !var.isConst() && !var.isPointerConst() ) );
	assert( kind != co::TK_INTERFACE || var.isPointer() );

	// get the value from Lua and store at the location indicated by var
	switch( kind )
	{
	case co::TK_ANY:
		getAny( L, index, NULL, var.get<co::Any&>() );
		break;
	case co::TK_BOOLEAN:
		var.get<bool&>() = ( lua_toboolean( L, index ) != 0 );
		break;
	case co::TK_INT8:
	case co::TK_UINT8:
	case co::TK_INT16:
	case co::TK_UINT16:
	case co::TK_INT32:
	case co::TK_UINT32:
	case co::TK_INT64:
	case co::TK_UINT64:
	case co::TK_FLOAT:
	case co::TK_DOUBLE:
		checkType( L, index, LUA_TNUMBER );
		switch( kind )
		{
		case co::TK_INT8:
			var.get<co::int8&>() = static_cast<co::int8>( lua_tointeger( L, index ) );
			break;
		case co::TK_UINT8:
			var.get<co::uint8&>() = static_cast<co::uint8>( lua_tointeger( L, index ) );
			break;
		case co::TK_INT16:
			var.get<co::int16&>() = static_cast<co::int16>( lua_tointeger( L, index ) );
			break;
		case co::TK_UINT16:
			var.get<co::uint16&>() = static_cast<co::uint16>( lua_tointeger( L, index ) );
			break;
		case co::TK_INT32:
			var.get<co::int32&>() = static_cast<co::int32>( lua_tointeger( L, index ) );
			break;
		case co::TK_UINT32:
			var.get<co::uint32&>() = static_cast<co::uint32>( lua_tointeger( L, index ) );
			break;
		case co::TK_INT64:
			var.get<co::int64&>() = static_cast<co::int64>( lua_tonumber( L, index ) );
			break;
		case co::TK_UINT64:
			var.get<co::uint64&>() = static_cast<co::uint64>( lua_tonumber( L, index ) );
			break;
		case co::TK_FLOAT:
			var.get<float&>() = static_cast<float>( lua_tonumber( L, index ) );
			break;
		case co::TK_DOUBLE:
			var.get<double&>() = static_cast<double>( lua_tonumber( L, index ) );
			break;
		default:
			assert( false );
		}
		break;
	case co::TK_STRING:
		checkType( L, index, LUA_TSTRING );
		{
			size_t len;
			const char* cstr = lua_tolstring( L, index, &len );
			var.get<std::string&>().assign( cstr, len );
		}
		break;
	case co::TK_ARRAY:
		// array not requested?
		if( var.getState().data.ptr == NULL )
			break;
		// array not provided?
		if( lua_isnil( L, index ) )
		{
			co::Any emptyArray;
			emptyArray.createArray( var.getType(), 0 );
			emptyArray.swapArray( var );
			break;
		}
		checkType( L, index, LUA_TTABLE );
		{
			co::Any anyArray;
			toArray( L, index, var.getType(), anyArray );
			anyArray.swapArray( var );
		}
		break;
	case co::TK_ENUM:
		*reinterpret_cast<co::uint32*>( var.getState().data.ptr ) =
			getEnumIdentifier( L, index, static_cast<co::IEnum*>( var.getType() ) );
		break;
	case co::TK_STRUCT:
	case co::TK_NATIVECLASS:
		{
			co::IType* expected = var.getType();
			co::ICompositeType* actual = CompositeTypeBinding::getType( L, index );
			if( actual != expected )
				CORAL_THROW( lua::Exception, expected->getFullName() << " expected, got " <<
					( actual ? actual->getFullName().c_str() : lua_typename( L, lua_type( L, index ) ) ) );
			expected->getReflector()->copyValue( ComplexValueBinding::getInstance( L, index ), var.getState().data.ptr );
		}
		break;
	case co::TK_INTERFACE:
		if( lua_isnil( L, index ) )
		{
			*reinterpret_cast<co::IService**>( var.getState().data.ptr ) = NULL;
		}
		else
		{
			co::IService* itf = NULL;
			co::ICompositeType* ct = NULL;

			int stackTop = lua_gettop( L );
			int tp = lua_type( L, index );
			if( tp == LUA_TTABLE )
			{
				lua_getfield( L, index, "__service" );
				if( lua_type( L, -1 ) == LUA_TUSERDATA )
				{
					tp = LUA_TUSERDATA;
					index = -1;
				}
			}

			if( tp == LUA_TUSERDATA )
			{
				ct = CompositeTypeBinding::getType( L, index );
				co::TypeKind kind = ct ? ct->getKind() : co::TK_NONE;
				if( kind == co::TK_INTERFACE || kind == co::TK_COMPONENT )
					itf = ServiceBinding::getInstance( L, index );
			}

			lua_settop( L, stackTop );

			if( !itf || !itf->getInterface()->isSubTypeOf( var.getInterface() ) )
				CORAL_THROW( lua::Exception, var.getInterface()->getFullName() << " expected, got "
								<< ( ct ? ct->getFullName().c_str() : lua_typename( L, tp ) ) );

			*reinterpret_cast<co::IService**>( var.getState().data.ptr ) = itf;
		}
		break;
	default:
		assert( false );
	}
}

bool LuaState::findScript( const std::string& name, std::string& filename )
{
	return findScript( getL(), name, filename );
}

co::int32 LuaState::callFunction( const std::string& moduleName, const std::string& functionName,
									co::Range<const co::Any> args, co::Range<const co::Any> results )
{
	lua_State* L = getL();
	int top = lua_gettop( L );

	try
	{
		require( L, moduleName );

		// should we retrieve a function from (presumably) the returned module table?
		if( !functionName.empty() )
		{
			if( !lua_istable( L, -1 ) )
				throw lua::Exception( "module did not return a table" );

			push( L, functionName );
			lua_gettable( L, -2 );
			lua_replace( L, -2 );

			if( lua_isnil( L, -1 ) )
				throw lua::Exception( "no such function" );
		}

		// push the args and call the function
		int numArgs = static_cast<int>( args.getSize() );
		for( int i = 0; i < numArgs; ++i )
			push( L, args[i] );

		call( L, numArgs, LUA_MULTRET );

		int numResults = lua_gettop( L ) - top;
		assert( numResults >= 0 );

		int maxResults = std::min( numResults, static_cast<int>( results.getSize() ) );
		for( int i = 0; i < maxResults; ++i )
			getValue( L, top + i + 1, results[i] );

		lua_settop( L, top );

		return numResults;
	}
	catch( lua::Exception& e )
	{
		lua_settop( L, top );

		std::stringstream ss;
		ss << "error calling function ";
		if( !functionName.empty() )
			ss << "'" << functionName << "' ";
		ss << "from Lua module '" << moduleName << "': " << e.getMessage();

		throw lua::Exception( ss.str() );
	}
}

void LuaState::collectGarbage()
{
	lua_gc( getL(), LUA_GCCOLLECT, 0 );
}

void LuaState::pushInstancesTable( lua_State* L )
{
	if( sm_instancesTableRegIdx == 0 )
	{
		// the instances table
		lua_newtable( L );

		// the instances table's metatable, for making it a weak table
		lua_createtable( L, 0, 1 );
		lua_pushliteral( L, "__mode" );
		lua_pushliteral( L, "v" );
		lua_rawset( L, -3 );

		lua_setmetatable( L, -2 );

		sm_instancesTableRegIdx = luaL_ref( L, LUA_REGISTRYINDEX );
	}

	lua_rawgeti( L, LUA_REGISTRYINDEX, sm_instancesTableRegIdx );
}

void LuaState::pushArray( lua_State* L, const co::Any& var )
{
	const co::Any::State& s = var.getState();
	assert( s.kind == co::TK_ARRAY );

	if( s.data.ptr == NULL )
	{
		lua_pushnil( L );
	}

	co::TypeKind kind = static_cast<co::TypeKind>( s.type->getKind() );
	assert( !s.isPointer || s.type->getKind() == co::TK_INTERFACE );

	co::int32 elemSize = var.getSize();
	assert( kind != co::TK_INTERFACE || elemSize == sizeof(void*) );

	// get the array as a memory block
	int elemCount;
	co::uint8* blockStart;
	if( s.arrayKind == co::__any::State::AK_Range )
	{
		elemCount = static_cast<int>( s.arraySize );
		blockStart = reinterpret_cast<co::uint8*>( s.data.ptr );
	}
	else
	{
		std::vector<co::uint8>& pseudoVector = *reinterpret_cast<std::vector<co::uint8>*>( s.data.ptr );
		elemCount = static_cast<int>( pseudoVector.size() / elemSize );
		blockStart = ( elemCount == 0 ? NULL : &pseudoVector[0] );
	}

	lua_createtable( L, elemCount, 0 );

	if( kind == co::TK_INTERFACE )
	{
		for( int i = 0; i < elemCount; ++i )
		{
			co::IService* itf = *reinterpret_cast<co::IService**>( blockStart + i * sizeof(void*) );
			if( itf )
				push( L, itf );
			else
			{
				// should we allow holes? would it mess with Lua's concept of array?
				assert( false );
				lua_pushnil( L );
			}

			lua_rawseti( L, -2, i + 1 );
		}
	}
	else
	{
		co::Any elem;
		co::uint32 flags = ( ( kind > co::TK_ANY && ( kind < co::TK_STRING || kind == co::TK_ENUM ) ) ?
								co::Any::VarIsValue : co::Any::VarIsReference );
		for( int i = 0; i < elemCount; ++i )
		{
			if( kind < co::TK_ARRAY )
				elem.setBasic( kind, flags, blockStart + i * elemSize );
			else
				elem.setVariable( var.getType(), flags, blockStart + i * elemSize );

			push( L, elem );
			lua_rawseti( L, -2, i + 1 );
		}
	}
}

void LuaState::toArray( lua_State* L, int index, co::IType* elementType, co::Any& var )
{
	co::TypeKind elementKind = elementType->getKind();
	co::IReflector* elementReflector = elementType->getReflector();
	co::int32 elementSize = elementReflector->getSize();

	int len = static_cast<int>( lua_rawlen( L, index ) );

	co::Any::PseudoVector& pv = var.createArray( elementType, len );
	co::uint8* p = ( len == 0 ? NULL : &pv[0] );

	co::Any elementVar;
	co::uint32 flags = ( elementKind == co::TK_INTERFACE ? co::Any::VarIsPointer : 0 );
	elementVar.setVariable( elementType, flags | co::Any::VarIsReference, p );

	int i = 1;
	int stackTop = lua_gettop( L );

	try
	{
		for( ; i <= len; ++i )
		{
			lua_rawgeti( L, index, i );

			if( elementKind == co::TK_INTERFACE )
			{
				// emulate a co::RefVector
				co::IService* oldItf = elementVar.get<co::IService*&>();
				getValue( L, -1, elementVar );
				co::IService* newItf = elementVar.get<co::IService*&>();
				if( newItf )
					newItf->serviceRetain();
				if( oldItf )
					oldItf->serviceRelease();
			}
			else
			{
				getValue( L, -1, elementVar );
			}
			lua_settop( L, stackTop );

			p += elementSize;
			elementVar.getState().data.ptr = p;
		}
	}
	catch( std::exception& e )
	{
		lua_settop( L, stackTop );
		CORAL_THROW( lua::Exception, "invalid array element #" << i << " (" << e.what() << ")" );
	}
}

co::int32 LuaState::getEnumIdentifier( lua_State* L, int index, co::IEnum* enumType )
{
	co::int32 res;
	int type = lua_type( L, index );
	switch( type )
	{
	case LUA_TSTRING:
		res = enumType->getValueOf( lua_tostring( L, index ) );
		if( res == -1 )
			CORAL_THROW( lua::Exception, "invalid identifier '" << lua_tostring( L, index )
							<< "' for enum " << enumType->getFullName() );
		break;

	case LUA_TNUMBER:
		res = static_cast<co::int32>( lua_tointeger( L, index ) );
		if( res < 0 || res >= static_cast<co::int32>( enumType->getIdentifiers().getSize() ) )
			CORAL_THROW( lua::Exception, "integer '" << res
							<< "' is out of range for enum " << enumType->getFullName() );
		break;

	default:
		CORAL_THROW( lua::Exception, enumType->getFullName() << " identifier expected, got "
							<< lua_typename( L, type ) );
	}
	return res;
}

void LuaState::checkType( lua_State* L, int index, int expectedType )
{
	int actualType = lua_type( L, index );
	if( actualType != expectedType )
		CORAL_THROW( lua::Exception, lua_typename( L, expectedType )
						<< " expected, got " << lua_typename( L, actualType ) );
}

void LuaState::raiseException( lua_State* L, int errorCode )
{
	switch( errorCode )
	{
	case LUA_ERRRUN:
	case LUA_ERRSYNTAX:
	case LUA_ERRMEM:
	case LUA_ERRGCMM:
	case LUA_ERRERR:
	case LUA_ERRFILE:
		break;
	default:
		assert( false );
	}

	size_t size;
	const char* msg = lua_tolstring( L, -1, &size );
	lua_pop( L, 1 );

	/*
		Exception messages in the form "{some.IException}some message" are raised
		as instances of some.IException, using Coral's reflection API.
	 */

	// the message must be at least 5 chars long and start with a parenthesis
	if( size >= 5 && msg[0] == '{' )
	{
		const char* typeStart = msg + 1;
		const char* typeEnd = typeStart + 1;
		const char* msgStart = NULL;
		const char* msgEnd = msg + size;

		while( typeEnd < msgEnd )
		{
			if( *typeEnd == '}' )
			{
				msgStart = typeEnd + 1;
				break;
			}
			++typeEnd;
		}

		// the parenthesis must have been closed
		if( msgStart )
		{
			std::string typeName( typeStart, typeEnd - typeStart );
			std::string parsedMsg( msgStart, msgEnd - msgStart );

			// try to obtain an exception type/reflector
			co::IReflector* reflector;
			try
			{
				co::IType* type = co::getType( typeName );
				if( type->getKind() != co::TK_EXCEPTION )
					throw co::IllegalArgumentException( "type is not an exception" );
				reflector = type->getReflector();
			}
			catch( std::exception& e )
			{
				CORAL_THROW( lua::Exception, "[error throwing an exception of type '" << typeName << "' from Lua: "
								<< e.what() << "] " << parsedMsg );
			}

			reflector->raise( parsedMsg );
		}
	}

	throw lua::Exception( std::string( msg, size ) );
}

CORAL_EXPORT_COMPONENT( LuaState, Universe );

} // namespace lua
