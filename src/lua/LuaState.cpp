/*
 * Coral - A C++ Component Framework.
 * See Copyright Notice in Coral.h
 */

#include "LuaState.h"
#include "LuaBinding.h"
#include <co/Reflector.h>
#include <lua/Exception.h>
#include <sstream>

LuaState LuaState::sm_instance;

LuaState::LuaState()
{
	_L = NULL;
	_instancesTableRegIdx = 0;
}

LuaState::~LuaState()
{
	lua_close( _L );
}

static int traceback( lua_State* L )
{
	luaL_traceback( L, L, lua_tostring( L, 1 ), 1 );
	return 1;
}

void LuaState::setup()
{
	assert( !_L );

	_L = luaL_newstate();
	assert( _L );

	// open all standard Lua libraries
	luaL_openlibs( _L );

	// create the 'co' package
	coPackage::open( _L );

	/*
		We install the co.packageLoader function into Lua's package.loaders table
		in order to enable 'require' to load Lua scripts in the CORAL_PATH.
	 */
	if( luaL_dostring( _L, "table.insert( package.loaders, 2, co.packageLoader )" ) )
	{
		lua_pop( _L, 1 );
		throw lua::Exception( "error installing the co.packageLoader into Lua's package.loaders table" );
	}

	// keep our stack-tracing function at the first stack position
	lua_settop( _L, 0 );
	lua_pushcfunction( _L, traceback );
}

void LuaState::tearDown()
{
	// release the 'co.system' interface
	coPackage::close( _L );

	// perform a full garbage-collection cycle
	lua_gc( _L, LUA_GCCOLLECT, 0 );
}

void LuaState::dumpStack()
{
	lua_State* L = _L;
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


void LuaState::loadFile( const std::string& filename )
{
	int res = luaL_loadfile( _L, filename.c_str() );
	if( res != LUA_OK )
		raiseException( res );
}

void LuaState::call( int numArgs, int numResults )
{
	assert( lua_isfunction( _L, 1 ) );
	int res = lua_pcall( _L, numArgs, numResults, 1 );
	if( res != LUA_OK )
		raiseException( res );
}

void LuaState::push( const co::Any& var, int depth )
{
	const co::__any::State& s = var.getState();

	/*
		The variable must be either a value or a reference (to a string or complex value).
		Pointers are only accepted for interfaces.
	 */
	if( s.kind != co::TK_INTERFACE && s.kind != co::TK_ARRAY && ( s.isPointer ||
		   ( s.isReference && ( s.kind < co::TK_STRING || s.kind == co::TK_ENUM ) ) ) )
	{
		assert( false );
		lua_pushnil( _L );
		return;
	}

	switch( s.kind )
	{
	case co::TK_ANY:
		if( depth > 1 )
		{
			/*
				Since a co::Any may reference any other co::Any,
				we must limit the number of indirections to avoid cycles.
			 */
			assert( false );
			lua_pushnil( _L );
			break;
		}
		push( var.get<const co::Any&>(), depth + 1 );
		break;
	case co::TK_BOOLEAN:
		lua_pushboolean( _L, s.data.b );
		break;
	case co::TK_INT8:
		lua_pushnumber( _L, s.data.i8 );
		break;
	case co::TK_UINT8:
		lua_pushnumber( _L, s.data.u8 );
		break;
	case co::TK_INT16:
		lua_pushnumber( _L, s.data.i16 );
		break;
	case co::TK_UINT16:
		lua_pushnumber( _L, s.data.u16 );
		break;
	case co::TK_INT32:
		lua_pushnumber( _L, s.data.i32 );
		break;
	case co::TK_UINT32:
		lua_pushnumber( _L, s.data.u32 );
		break;
	case co::TK_INT64:
		lua_pushnumber( _L, static_cast<lua_Number>( s.data.i64 ) );
		break;
	case co::TK_UINT64:
		lua_pushnumber( _L, static_cast<lua_Number>( s.data.u64 ) );
		break;
	case co::TK_FLOAT:
		lua_pushnumber( _L, s.data.f );
		break;
	case co::TK_DOUBLE:
		lua_pushnumber( _L, s.data.d );
		break;
	case co::TK_STRING:
		push( var.get<const std::string&>() );
		break;
	case co::TK_ARRAY:
		pushArray( var );
		break;
	case co::TK_ENUM:
		{
			co::ArrayRange<std::string const> ids = static_cast<co::EnumType*>( s.type )->getIdentifiers();
			if( s.data.u32 < ids.getSize() )
				push( ids[s.data.u32] );
			else
			{
				// enum value is out of bounds
				assert( false );
				lua_pushliteral( _L, "<invalid>" );
			}
		}
		break;
	case co::TK_STRUCT:
		StructBinding::push( _L, static_cast<co::StructType*>( s.type ), s.data.ptr );
		break;
	case co::TK_NATIVECLASS:
		NativeClassBinding::push( _L, static_cast<co::NativeClassType*>( s.type ), s.data.ptr );
		break;
	case co::TK_INTERFACE:
		push( reinterpret_cast<co::Interface*>( s.data.ptr ) );
		break;
	default:
		assert( false );
		lua_pushnil( _L );
	}
}

void LuaState::push( const std::string& str )
{
	lua_pushlstring( _L, str.c_str(), str.length() );
}

template<typename BindingClass, typename InstanceType>
inline void LuaState::pushInstance( InstanceType* ptr )
{
	if( !ptr )
	{
		lua_pushnil( _L );
		return;
	}

	// try to get an existing object (userdata) from the instances table
	pushInstancesTable();
	lua_pushlightuserdata( _L, ptr );
	lua_rawget( _L, -2 );

	// 'itf' not in the instances table yet?
	if( lua_isnil( _L, -1 ) )
	{
		lua_pop( _L, 1 );

		// create the userdata ...
		BindingClass::create( _L, ptr );

		// ... and add it to the instances table
		lua_pushlightuserdata( _L, ptr );
		lua_pushvalue( _L, -2 );
		lua_rawset( _L, -4 );
	}

	// remove the instances table from the stack
	lua_remove( _L, -2 );
}

void LuaState::push( co::Interface* itf )
{
	if( itf && itf->getInterfaceType()->getFullName() == "co.Component" )
		push( static_cast<co::Component*>( itf ) );
	else
		pushInstance<InterfaceBinding, co::Interface>( itf );
}

void LuaState::push( co::Component* component )
{
	pushInstance<ComponentBinding, co::Component>( component );
}

void LuaState::toCoral( int index, co::Type* expectedType, co::Any& var, co::AnyValue& value )
{
	assert( expectedType );	

	int type = lua_type( _L, index );

	switch( type )
	{
	case LUA_TNIL:
		if( expectedType->getKind() == co::TK_INTERFACE )
		{
			var.setInterface( NULL, static_cast<co::InterfaceType*>( expectedType ) );
			break;
		}
		throw lua::Exception( "no conversion from Lua's nil" );

	case LUA_TBOOLEAN:
		var.set<bool>( lua_toboolean( _L, index ) != 0 );
		break;

	case LUA_TLIGHTUSERDATA:
		throw lua::Exception( "no conversion from a Lua light userdata" );

	case LUA_TNUMBER:
		var.set( lua_tonumber( _L, index ) );
		break;

	case LUA_TSTRING:
		if( expectedType->getKind() == co::TK_ENUM )
		{
			const char* str = lua_tostring( _L, index );
			co::int32 id = static_cast<co::EnumType*>( expectedType )->getValueOf( str );
			if( id == -1 )
				CORAL_THROW( lua::Exception, "invalid identifier '" << str << "' for enum '"
								<< expectedType->getFullName() << "'" );
			var.set( id );
		}
		else
		{
			size_t length;
			const char* cstr = lua_tolstring( _L, index, &length );
			std::string& str = value.createString();
			str.assign( cstr, length );
			var.set<std::string&>( str );
		}
		break;

	case LUA_TTABLE:
		if( expectedType->getKind() == co::TK_ARRAY )
			toArray( index, expectedType, var, value );
		else
			throw lua::Exception( "no conversion from a Lua table" );

	case LUA_TFUNCTION:
		throw lua::Exception( "no conversion from a Lua function" );

	case LUA_TUSERDATA:
		CompoundTypeBinding::getInstance( _L, index, var );
		break;

	case LUA_TTHREAD:
		throw lua::Exception( "no conversion from a Lua thread" );

	default:
		assert( false );
	}
}

void LuaState::pushInstancesTable()
{
	if( _instancesTableRegIdx == 0 )
	{
		// the instances table
		lua_newtable( _L );

		// the instances table's metatable, for making it a weak table
		lua_createtable( _L, 0, 1 );
		lua_pushliteral( _L, "__mode" );
		lua_pushliteral( _L, "v" );
		lua_rawset( _L, -3 );

		lua_setmetatable( _L, -2 );

		_instancesTableRegIdx = luaL_ref( _L, LUA_REGISTRYINDEX );
	}

	lua_rawgeti( _L, LUA_REGISTRYINDEX, _instancesTableRegIdx );
}

void LuaState::pushArray( const co::Any& var )
{
	const co::__any::State& s = var.getState();
	assert( s.kind == co::TK_ARRAY );

	co::TypeKind kind = static_cast<co::TypeKind>( s.type->getKind() );
	assert( !s.isPointer || s.type->getKind() == co::TK_INTERFACE );

	co::int32 elemSize = var.getSize();
	assert( kind != co::TK_INTERFACE || elemSize == sizeof(void*) );

	// get the array as a memory block
	co::int32 elemCount;
	co::uint8* blockStart;
	if( s.arrayKind == co::__any::State::AK_ArrayRange )
	{
		elemCount = s.arraySize;
		blockStart = reinterpret_cast<co::uint8*>( s.data.ptr );
	}
	else
	{
		std::vector<co::uint8>& pseudoVector = *reinterpret_cast<std::vector<co::uint8>*>( s.data.ptr );
		elemCount = pseudoVector.size() / elemSize;
		blockStart = &pseudoVector[0];
	}

	lua_createtable( _L, elemCount, 0 );

	if( kind == co::TK_INTERFACE )
	{
		for( co::int32 i = 0; i < elemCount; ++i )
		{
			co::Interface* itf = *reinterpret_cast<co::Interface**>( blockStart + i * sizeof(void*) );
			if( itf )
				push( itf );
			else
			{
				// should we allow holes? would it mess with Lua's concept of array?
				assert( false );
				lua_pushnil( _L );
			}

			lua_rawseti( _L, -2, i + 1 );
		}
	}
	else
	{
		co::Any elem;
		co::uint32 flags = ( ( s.isReference || kind >= co::TK_STRUCT ) ? co::Any::VarIsReference : co::Any::VarIsValue );
		for( co::int32 i = 0; i < elemCount; ++i )
		{
			if( kind < co::TK_ARRAY )
				elem.setBasic( kind, flags, blockStart + i * elemSize );
			else
				elem.setVariable( var.getType(), flags, blockStart + i * elemSize );

			push( elem );
			lua_rawseti( _L, -2, i + 1 );
		}
	}
}

void LuaState::toArray( int index, co::Type* expectedType, co::Any& var, co::AnyValue& value )
{
	co::ArrayType* arrayType = dynamic_cast<co::ArrayType*>( expectedType );
	assert( arrayType );

	co::Type* elementType = arrayType->getElementType();
	co::TypeKind elementKind = elementType->getKind();
	co::Reflector* elementReflector = elementType->getReflector();
	co::int32 elementSize = elementReflector->getSize();

	size_t len = lua_rawlen( _L, index );

	co::AnyValue::PseudoVector& pv = value.createArray( elementType, len );
	var.setArray(
		( elementKind == co::TK_INTERFACE ? co::Any::AK_RefVector : co::Any::AK_StdVector ), elementType,
		( elementKind == co::TK_INTERFACE ? co::Any::VarIsPointer : co::Any::VarIsValue ), &pv );

	co::uint8* p = &pv[0];

	co::Any elementVar;
	co::AnyValue elementValue;

	size_t i = 1;
	int stackTop = lua_gettop( _L );

	try
	{
		for( ; i <= len; ++i )
		{
			lua_rawgeti( _L, index, i );
			toCoral( stackTop + 1, elementType, elementVar, elementValue );
			lua_settop( _L, stackTop );

			switch( elementKind )
			{
			case co::TK_BOOLEAN:
				*reinterpret_cast<bool*>( p ) = elementVar.get<bool>();
				break;
			case co::TK_INT8:
				*reinterpret_cast<co::int8*>( p ) = elementVar.get<co::int8>();
				break;
			case co::TK_UINT8:
				*reinterpret_cast<co::uint8*>( p ) = elementVar.get<co::uint8>();
				break;
			case co::TK_INT16:
				*reinterpret_cast<co::int16*>( p ) = elementVar.get<co::int16>();
				break;
			case co::TK_UINT16:
				*reinterpret_cast<co::uint16*>( p ) = elementVar.get<co::uint16>();
				break;
			case co::TK_INT32:
				*reinterpret_cast<co::int32*>( p ) = elementVar.get<co::int32>();
				break;
			case co::TK_UINT32:
				*reinterpret_cast<co::uint32*>( p ) = elementVar.get<co::uint32>();
				break;
			case co::TK_INT64:
				*reinterpret_cast<co::int64*>( p ) = elementVar.get<co::int64>();
				break;
			case co::TK_UINT64:
				*reinterpret_cast<co::uint64*>( p ) = elementVar.get<co::uint64>();
				break;
			case co::TK_FLOAT:
				*reinterpret_cast<float*>( p ) = elementVar.get<float>();
				break;
			case co::TK_DOUBLE:
				*reinterpret_cast<double*>( p ) = elementVar.get<double>();
				break;
			case co::TK_STRING:
				*reinterpret_cast<std::string*>( p ) = elementVar.get<const std::string&>();
				break;
			case co::TK_ENUM:
				*reinterpret_cast<co::uint32*>( p ) = elementVar.get<co::uint32>();
				break;
			case co::TK_STRUCT:
			case co::TK_NATIVECLASS:
				if( elementVar.getType() != elementType )
					CORAL_THROW( lua::Exception, elementType->getFullName() << " expected, got " << elementVar );
				elementReflector->copyValue( elementVar.getState().data.ptr, p );
				break;
			case co::TK_INTERFACE:
				{
					co::Interface* src = elementVar.get<co::Interface*>();
					co::Interface*& dest = *reinterpret_cast<co::Interface**>( p );
					co::Interface* prev = dest;
					if( src )
						src->componentRetain();

					if( prev )
						prev->componentRelease();
					dest = src;
				}
				break;
			default:
				assert( false );
			}

			p += elementSize;
		}
	}
	catch( std::exception& e )
	{
		lua_settop( _L, stackTop );
		CORAL_THROW( lua::Exception, "invalid array element #" << i << " (" << e.what() << ")" );
	}
}

void LuaState::raiseException( int errorCode )
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

	const char* msg = lua_tostring( _L, -1 );
	lua_pop( _L, 1 );

	throw lua::Exception( msg );
}
