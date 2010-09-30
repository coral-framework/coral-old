/*
 * Coral - A C++ Component Framework.
 * See Copyright Notice in Coral.h
 */

#include "LuaState.h"
#include "LuaBinding.h"
#include <co/Reflector.h>
#include <co/reserved/FileLookUp.h>
#include <lua/Exception.h>
#include <sstream>

extern "C" {
	#include <lfs.h>
}

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

static int traceback( lua_State* L )
{
	luaL_traceback( L, L, lua_tostring( L, 1 ), 1 );
	return 1;
}

void LuaState::setup()
{
	assert( !sm_L );

	sm_L = luaL_newstate();
	assert( sm_L );

	// open all standard Lua libraries
	luaL_openlibs( sm_L );

	// open the LuaFileSystem library
	luaL_requiref( sm_L, "lfs", luaopen_lfs, 0 );
    lua_pop( sm_L, 1 );

	// create the 'co' package
	coPackage::open( sm_L );

	// keep our stack-tracing function at the first stack position
	lua_settop( sm_L, 0 );
	lua_pushcfunction( sm_L, traceback );
}

void LuaState::tearDown()
{
	// release the 'co.system' interface
	coPackage::close( sm_L );

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

void LuaState::doString( lua_State* L, const char* code )
{
	loadString( L, code );
	call( L, 0, 0 );
}

bool LuaState::searchScriptFile( const std::string& name, std::string& filename )
{
	static const std::string s_extension( "lua" );

	co::FileLookUp fileLookUp( co::getPaths(), co::ArrayRange<const std::string>( &s_extension, 1 ) );

	// look for '?.lua'
	std::string filePath( name );
	fileLookUp.addFilePath( filePath, true );

	// look for '?/init.lua'
	filePath.append( ".init" );
	fileLookUp.addFilePath( filePath, true );

	return fileLookUp.locate( filename );
}

void LuaState::loadFile( lua_State* L, const std::string& filename )
{
	int res = luaL_loadfile( L, filename.c_str() );
	if( res != LUA_OK )
		raiseException( L, res );
}

void LuaState::loadString( lua_State* L, const char* code )
{
	int res = luaL_loadstring( L, code );
	if( res != LUA_OK )
		raiseException( L, res );
}

void LuaState::call( lua_State* L, co::int32 numArgs, co::int32 numResults )
{
	assert( lua_isfunction( L, 1 ) );
	int res = lua_pcall( L, numArgs, numResults, 1 );
	if( res != LUA_OK )
		raiseException( L, res );
}

void LuaState::push( lua_State* L, const co::Any& var )
{
	push( L, var, 0 );
}

void LuaState::push( lua_State* L, const co::Any& var, int depth )
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
		lua_pushnil( L );
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
			lua_pushnil( L );
			break;
		}
		push( L, var.get<const co::Any&>(), depth + 1 );
		break;
	case co::TK_BOOLEAN:
		lua_pushboolean( L, s.data.b );
		break;
	case co::TK_INT8:
		lua_pushnumber( L, s.data.i8 );
		break;
	case co::TK_UINT8:
		lua_pushnumber( L, s.data.u8 );
		break;
	case co::TK_INT16:
		lua_pushnumber( L, s.data.i16 );
		break;
	case co::TK_UINT16:
		lua_pushnumber( L, s.data.u16 );
		break;
	case co::TK_INT32:
		lua_pushnumber( L, s.data.i32 );
		break;
	case co::TK_UINT32:
		lua_pushnumber( L, s.data.u32 );
		break;
	case co::TK_INT64:
		lua_pushnumber( L, static_cast<lua_Number>( s.data.i64 ) );
		break;
	case co::TK_UINT64:
		lua_pushnumber( L, static_cast<lua_Number>( s.data.u64 ) );
		break;
	case co::TK_FLOAT:
		lua_pushnumber( L, s.data.f );
		break;
	case co::TK_DOUBLE:
		lua_pushnumber( L, s.data.d );
		break;
	case co::TK_STRING:
		push( L, var.get<const std::string&>() );
		break;
	case co::TK_ARRAY:
		pushArray( L, var );
		break;
	case co::TK_ENUM:
		{
			co::ArrayRange<std::string const> ids = static_cast<co::EnumType*>( s.type )->getIdentifiers();
			if( s.data.u32 < ids.getSize() )
				push( L, ids[s.data.u32] );
			else
			{
				// enum value is out of bounds
				assert( false );
				lua_pushliteral( L, "<invalid>" );
			}
		}
		break;
	case co::TK_STRUCT:
		StructBinding::push( L, static_cast<co::StructType*>( s.type ), s.data.ptr );
		break;
	case co::TK_NATIVECLASS:
		NativeClassBinding::push( L, static_cast<co::NativeClassType*>( s.type ), s.data.ptr );
		break;
	case co::TK_INTERFACE:
		push( L, reinterpret_cast<co::Interface*>( s.data.ptr ) );
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

void LuaState::push( lua_State* L, co::Interface* itf )
{
	if( itf && itf->getInterfaceType()->getFullName() == "co.Component" )
		push( L, static_cast<co::Component*>( itf ) );
	else
		pushInstance<InterfaceBinding, co::Interface>( L, itf );
}

void LuaState::push( lua_State* L, co::Component* component )
{
	pushInstance<ComponentBinding, co::Component>( L, component );
}

void LuaState::toCoral( lua_State* L, int index, co::Type* expectedType, co::Any& var )
{
	assert( expectedType );

	co::Any* any;
	co::TypeKind expectedKind = expectedType->getKind();
	if( expectedKind == co::TK_ANY )
		any = &var.createAny();
	else
		any = &var;
	
	switch( lua_type( L, index ) )
	{
	case LUA_TNIL:
		if( expectedKind == co::TK_INTERFACE )
		{
			any->setInterface( NULL, static_cast<co::InterfaceType*>( expectedType ) );
			break;
		}
		throw lua::Exception( "no conversion from Lua's nil" );

	case LUA_TBOOLEAN:
		any->set<bool>( lua_toboolean( L, index ) != 0 );
		break;

	case LUA_TLIGHTUSERDATA:
		throw lua::Exception( "no conversion from a Lua light userdata" );

	case LUA_TNUMBER:
		any->set( lua_tonumber( L, index ) );
		break;

	case LUA_TSTRING:
		if( expectedKind == co::TK_ENUM )
		{
			const char* str = lua_tostring( L, index );
			co::int32 id = static_cast<co::EnumType*>( expectedType )->getValueOf( str );
			if( id == -1 )
				CORAL_THROW( lua::Exception, "invalid identifier '" << str << "' for enum '"
								<< expectedType->getFullName() << "'" );
			any->set( id );
		}
		else
		{
			size_t length;
			const char* cstr = lua_tolstring( L, index, &length );
			any->createString().assign( cstr, length );
		}
		break;

	case LUA_TTABLE:
		if( expectedKind == co::TK_ARRAY )
			toArray( L, index, expectedType, *any );
		else
			throw lua::Exception( "no conversion from a Lua table" );

	case LUA_TFUNCTION:
		throw lua::Exception( "no conversion from a Lua function" );

	case LUA_TUSERDATA:
		CompoundTypeBinding::getInstance( L, index, *any );
		break;

	case LUA_TTHREAD:
		throw lua::Exception( "no conversion from a Lua thread" );

	default:
		assert( false );
	}
}

bool LuaState::searchScript( const std::string& name, std::string& filename )
{
	return searchScriptFile( name, filename );
}

void LuaState::loadFile( const std::string& filename )
{
	loadFile( getL(), filename );
}

void LuaState::call( co::int32 numArgs, co::int32 numResults )
{
	call( getL(), numArgs, numResults );
}

void LuaState::push( const co::Any& var )
{
	push( getL(), var );
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
	const co::__any::State& s = var.getState();
	assert( s.kind == co::TK_ARRAY );

	co::TypeKind kind = static_cast<co::TypeKind>( s.type->getKind() );
	assert( !s.isPointer || s.type->getKind() == co::TK_INTERFACE );

	co::int32 elemSize = var.getSize();
	assert( kind != co::TK_INTERFACE || elemSize == sizeof(void*) );

	// get the array as a memory block
	int elemCount;
	co::uint8* blockStart;
	if( s.arrayKind == co::__any::State::AK_ArrayRange )
	{
		elemCount = static_cast<int>( s.arraySize );
		blockStart = reinterpret_cast<co::uint8*>( s.data.ptr );
	}
	else
	{
		std::vector<co::uint8>& pseudoVector = *reinterpret_cast<std::vector<co::uint8>*>( s.data.ptr );
		elemCount = static_cast<int>( pseudoVector.size() / elemSize );
		blockStart = &pseudoVector[0];
	}

	lua_createtable( L, elemCount, 0 );

	if( kind == co::TK_INTERFACE )
	{
		for( int i = 0; i < elemCount; ++i )
		{
			co::Interface* itf = *reinterpret_cast<co::Interface**>( blockStart + i * sizeof(void*) );
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
		co::uint32 flags = ( kind < co::TK_STRING || kind == co::TK_ENUM ? co::Any::VarIsValue : co::Any::VarIsReference );
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

void LuaState::toArray( lua_State* L, int index, co::Type* expectedType, co::Any& var )
{
	co::ArrayType* arrayType = dynamic_cast<co::ArrayType*>( expectedType );
	assert( arrayType );

	co::Type* elementType = arrayType->getElementType();
	co::TypeKind elementKind = elementType->getKind();
	co::Reflector* elementReflector = elementType->getReflector();
	co::int32 elementSize = elementReflector->getSize();

	int len = static_cast<int>( lua_rawlen( L, index ) );

	co::Any::PseudoVector& pv = var.createArray( elementType, len );
	co::uint8* p = &pv[0];

	int i = 1;
	int stackTop = lua_gettop( L );
	co::Any elementValue;

	try
	{
		for( ; i <= len; ++i )
		{
			lua_rawgeti( L, index, i );
			toCoral( L, stackTop + 1, elementType, elementValue );
			lua_settop( L, stackTop );

			switch( elementKind )
			{
			case co::TK_BOOLEAN:
				*reinterpret_cast<bool*>( p ) = elementValue.get<bool>();
				break;
			case co::TK_INT8:
				*reinterpret_cast<co::int8*>( p ) = elementValue.get<co::int8>();
				break;
			case co::TK_UINT8:
				*reinterpret_cast<co::uint8*>( p ) = elementValue.get<co::uint8>();
				break;
			case co::TK_INT16:
				*reinterpret_cast<co::int16*>( p ) = elementValue.get<co::int16>();
				break;
			case co::TK_UINT16:
				*reinterpret_cast<co::uint16*>( p ) = elementValue.get<co::uint16>();
				break;
			case co::TK_INT32:
				*reinterpret_cast<co::int32*>( p ) = elementValue.get<co::int32>();
				break;
			case co::TK_UINT32:
				*reinterpret_cast<co::uint32*>( p ) = elementValue.get<co::uint32>();
				break;
			case co::TK_INT64:
				*reinterpret_cast<co::int64*>( p ) = elementValue.get<co::int64>();
				break;
			case co::TK_UINT64:
				*reinterpret_cast<co::uint64*>( p ) = elementValue.get<co::uint64>();
				break;
			case co::TK_FLOAT:
				*reinterpret_cast<float*>( p ) = elementValue.get<float>();
				break;
			case co::TK_DOUBLE:
				*reinterpret_cast<double*>( p ) = elementValue.get<double>();
				break;
			case co::TK_STRING:
				*reinterpret_cast<std::string*>( p ) = elementValue.get<const std::string&>();
				break;
			case co::TK_ENUM:
				*reinterpret_cast<co::uint32*>( p ) = elementValue.get<co::uint32>();
				break;
			case co::TK_STRUCT:
			case co::TK_NATIVECLASS:
				if( elementValue.getType() != elementType )
					CORAL_THROW( lua::Exception, elementType->getFullName() << " expected, got " << elementValue );
				elementReflector->copyValue( elementValue.getState().data.ptr, p );
				break;
			case co::TK_INTERFACE:
				{
					co::Interface* src = elementValue.get<co::Interface*>();
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
		lua_settop( L, stackTop );
		CORAL_THROW( lua::Exception, "invalid array element #" << i << " (" << e.what() << ")" );
	}
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

	const char* msg = lua_tostring( L, -1 );
	lua_pop( L, 1 );

	throw lua::Exception( msg );
}

CORAL_EXPORT_COMPONENT( LuaState, Universe );
