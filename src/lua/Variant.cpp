/*
 * Coral - A C++ Component Framework.
 * See Copyright Notice in Coral.h
 */

#include "Variant.h"

void Variant::makeOut( co::Type* type, co::Any& arg )
{
	co::TypeKind kind = type->getKind();
	switch( kind )
	{
	case co::TK_ANY:
		// if we had two Variants per parameter we could accept any value for
		// an 'inout any' parameter; currenty we only accept real co::Any's.
		createAny();
		if( arg.isValid() )
			getAny() = arg.get<const co::Any&>();
		arg.set<co::Any&>( getAny() );
		break;
	case co::TK_BOOLEAN:
		if( arg.isValid() )
			data.b = arg.get<bool>();
		arg.set<bool&>( data.b );
		break;
	case co::TK_INT8:
		if( arg.isValid() )
			data.i8 = arg.get<co::int8>();
		arg.set<co::int8&>( data.i8 );
		break;
	case co::TK_UINT8:
		if( arg.isValid() )
			data.u8 = arg.get<co::uint8>();
		arg.set<co::uint8&>( data.u8 );
		break;
	case co::TK_INT16:
		if( arg.isValid() )
			data.i16 = arg.get<co::int16>();
		arg.set<co::int16&>( data.i16 );
		break;
	case co::TK_UINT16:
		if( arg.isValid() )
			data.u16 = arg.get<co::uint16>();
		arg.set<co::uint16&>( data.u16 );
		break;
	case co::TK_INT32:
		if( arg.isValid() )
			data.i32 = arg.get<co::int32>();
		arg.set<co::int32&>( data.i32 );
		break;
	case co::TK_UINT32:
		if( arg.isValid() )
			data.u32 = arg.get<co::uint32>();
		arg.set<co::uint32&>( data.u32 );
		break;
	case co::TK_INT64:
		if( arg.isValid() )
			data.i64 = arg.get<co::int64>();
		arg.set<co::int64&>( data.i64 );
		break;
	case co::TK_UINT64:
		if( arg.isValid() )
			data.u64 = arg.get<co::uint64>();
		arg.set<co::uint64&>( data.u64 );
		break;
	case co::TK_FLOAT:
		if( arg.isValid() )
			data.f = arg.get<float>();
		arg.set<float&>( data.f );
		break;
	case co::TK_DOUBLE:
		if( arg.isValid() )
			data.d = arg.get<double>();
		arg.set<double&>( data.d );
		break;
	case co::TK_STRING:			
		if( arg.isValid() )
		{
			assert( arg.getKind() == co::TK_STRING );
			assert( arg.isConst() == false );
		}
		else
		{
			createString();
			arg.set<std::string&>( getString() );
		}
		break;
	case co::TK_ARRAY:
		// TODO
		break;
	case co::TK_ENUM:
		if( arg.isValid() )
			data.u32 = arg.get<co::uint32>();
		arg.set<co::uint32&>( data.u32 );
		break;
	case co::TK_STRUCT:
	case co::TK_NATIVECLASS:
		if( arg.isValid() )
		{
			assert( type == arg.getType() );
			assert( !arg.isPointer() );
			arg.setVariable( type, co::Any::VarIsReference, arg.getState().data.ptr );
		}
		else
		{
			void* ptr = createComplexValue( type );
			arg.setVariable( type, co::Any::VarIsReference, ptr );
		}
		break;
	case co::TK_INTERFACE:
		data.ptr = arg.getState().data.ptr;
		arg.setVariable( type, ( co::Any::VarIsPointer | co::Any::VarIsReference ), &data.ptr );
		break;
	default:
		assert( false );
	}
}

void Variant::makeIn( co::Any& arg )
{
	co::TypeKind kind = arg.getKind();
	switch( kind )
	{
	case co::TK_ANY:
		arg = getAny();
		break;
	case co::TK_BOOLEAN:
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
		assert( arg.isReference() && !arg.isPointer() );
		arg.setBasic( arg.getKind(), co::Any::VarIsValue, arg.getState().data.ptr );
		break;
	case co::TK_STRING:
		// empty
		break;
	case co::TK_ARRAY:
		// TODO
		break;
	case co::TK_ENUM:
		assert( arg.isReference() && !arg.isPointer() );
		arg.setVariable( arg.getType(), co::Any::VarIsValue, arg.getState().data.ptr );
		break;
	case co::TK_STRUCT:
	case co::TK_NATIVECLASS:
		// empty
		break;
	case co::TK_INTERFACE:
		arg.setInterface( reinterpret_cast<co::Interface*>( data.ptr ), arg.getInterfaceType() );
		break;
	default:
		assert( false );
	}
}
