/*
 * Coral - A C++ Component Framework.
 * See Copyright Notice in Coral.h
 */

#include "Any.h"
#include <co/Type.h>
#include <co/EnumType.h>
#include <co/Namespace.h>
#include <co/InterfaceType.h>
#include <co/IllegalCastException.h>
#include <iomanip>
#include <sstream>

std::ostream& operator<<( std::ostream& out, const co::__any::State& s )
{
	if( s.kind == co::TK_NONE )
	{
		return out << co::TK_STRINGS_CPP[co::TK_NONE];
	}

	if( s.kind == co::TK_ARRAY )
	{
		const char* arrayKindString;
		switch( s.arrayKind )
		{
		case co::__any::State::AK_StdVector:	arrayKindString = "std::vector"; break;
		case co::__any::State::AK_RefVector:	arrayKindString = "co::RefVector"; break;
		case co::__any::State::AK_ArrayRange:	arrayKindString = "co::ArrayRange"; break;
		default:
			arrayKindString = NULL;
			assert( false );
		}

		out << arrayKindString << '<';
	}

	if( s.isConst )
		out << "const ";

	if( s.kind < co::TK_ARRAY )
	{
		out << co::TK_STRINGS_CPP[s.kind];
	}
	else if( s.kind == co::TK_ARRAY && s.type->getKind() < co::TK_ARRAY )
	{
		out << co::TK_STRINGS_CPP[s.type->getKind()];
	}
	else
	{
		assert( s.kind == co::TK_ARRAY ||
				s.kind == co::TK_ENUM ||
				s.kind == co::TK_STRUCT ||
				s.kind == co::TK_NATIVECLASS ||
				s.kind == co::TK_INTERFACE );

		static const int NAMESPACE_STACK_MAX_SIZE = 8;
		co::Namespace* namespaceStack[NAMESPACE_STACK_MAX_SIZE];

		int namespaceStackSize = 0;
		co::Namespace* ns = s.type->getNamespace();

		while( ns )
		{
			assert( namespaceStackSize < NAMESPACE_STACK_MAX_SIZE );
			namespaceStack[namespaceStackSize++] = ns;
			ns = ns->getParentNamespace();
		}

		for( int i = namespaceStackSize - 2; i >= 0; --i )
		{
			out << namespaceStack[i]->getName() << "::";
		}

		out << s.type->getName();
	}

	// should not print '*' for co::RefVectors
	if( s.kind == co::TK_ARRAY && s.arrayKind == co::__any::State::AK_RefVector )
	{
		assert( s.isPointer && s.isPointerConst && !s.isReference );
	}
	else
	{
		if( s.isPointer )
		{
			if( s.isPointerConst )
				out << "* const";
			else
				out << "*";
		}

		if( s.isReference )
			out << "&";
	}

	if( s.kind == co::TK_ARRAY )
		out << '>';

	return out;
}

std::ostream& operator<<( std::ostream& out, const co::Any& a )
{
	const co::__any::State& s = a._state;

	// type info
	out << "(" << s << ")";

	assert( s.kind != co::TK_EXCEPTION && s.kind != co::TK_COMPONENT );

	// print pointer
	if( s.kind == co::TK_ARRAY || s.isPointer || s.isReference )
	{
		if( s.data.ptr )
			out << s.data.ptr;
		else
			out << "NULL";
	}
	else
	{
		// print value
		switch( s.kind )
		{
		case co::TK_NONE:		break;
		case co::TK_BOOLEAN:	out << ( s.data.b ? "true" : "false" ); break;
		case co::TK_INT8:		out << s.data.i8; break;
		case co::TK_UINT8:		out << s.data.u8; break;
		case co::TK_INT16:		out << s.data.i16; break;
		case co::TK_UINT16:		out << s.data.u16; break;
		case co::TK_INT32:		out << s.data.i32; break;
		case co::TK_UINT32:		out << s.data.u32; break;
		case co::TK_INT64:		out << s.data.i64; break;
		case co::TK_UINT64:		out << s.data.u64; break;
		case co::TK_FLOAT:		out << s.data.f; break;
		case co::TK_DOUBLE:		out << s.data.d; break;
		case co::TK_ENUM:		out << s.data.u32; break;
		default:
			assert( false );
		}
	}

	return out;
}

namespace co {

#define THROW_ILLEGAL_CAST( from, to, MORE_MESSAGES ) \
	{ std::stringstream sstream; \
	sstream << "illegal cast from '" << from << "' to '" << to << "'" MORE_MESSAGES; \
	throw co::IllegalCastException( sstream.str() ); }

void castValue( const __any::State& from, __any::State& to )
{
	assert( ( to.kind > TK_ANY && to.kind < TK_STRING ) || to.kind == TK_ENUM );
	assert( ( from.kind > TK_ANY && from.kind < TK_STRING ) || from.kind == TK_ENUM );

	switch( to.kind )
	{
	case TK_BOOLEAN:
		switch( from.kind )
		{
		case TK_BOOLEAN:	to.data.b = from.data.b;		break;
		case TK_INT8:		to.data.b = from.data.i8 != 0;	break;
		case TK_UINT8:		to.data.b = from.data.u8 != 0;	break;
		case TK_INT16:		to.data.b = from.data.i16 != 0;	break;
		case TK_UINT16:		to.data.b = from.data.u16 != 0;	break;
		case TK_INT32:		to.data.b = from.data.i32 != 0;	break;
		case TK_UINT32:		to.data.b = from.data.u32 != 0;	break;
		case TK_INT64:		to.data.b = from.data.i64 != 0;	break;
		case TK_UINT64:		to.data.b = from.data.u64 != 0;	break;
		case TK_FLOAT:		to.data.b = from.data.f != 0;	break;
		case TK_DOUBLE:		to.data.b = from.data.d != 0;	break;
		case TK_ENUM:		to.data.b = from.data.u32 != 0;	break;
		}
		break;

	case TK_INT8:
		switch( from.kind )
		{
		case TK_BOOLEAN:	to.data.i8 = static_cast<int8>( from.data.b );		break;
		case TK_INT8:		to.data.i8 = from.data.i8;							break;
		case TK_UINT8:		to.data.i8 = static_cast<int8>( from.data.u8 );		break;
		case TK_INT16:		to.data.i8 = static_cast<int8>( from.data.i16 );	break;
		case TK_UINT16:		to.data.i8 = static_cast<int8>( from.data.u16 );	break;
		case TK_INT32:		to.data.i8 = static_cast<int8>( from.data.i32 );	break;
		case TK_UINT32:		to.data.i8 = static_cast<int8>( from.data.u32 );	break;
		case TK_INT64:		to.data.i8 = static_cast<int8>( from.data.i64 );	break;
		case TK_UINT64:		to.data.i8 = static_cast<int8>( from.data.u64 );	break;
		case TK_FLOAT:		to.data.i8 = static_cast<int8>( from.data.f );		break;
		case TK_DOUBLE:		to.data.i8 = static_cast<int8>( from.data.d );		break;
		case TK_ENUM:		to.data.i8 = static_cast<int8>( from.data.u32 );	break;
		}
		break;

	case TK_UINT8:
		switch( from.kind )
		{
		case TK_BOOLEAN:	to.data.u8 = static_cast<uint8>( from.data.b );		break;
		case TK_INT8:		to.data.u8 = static_cast<uint8>( from.data.i8 );	break;
		case TK_UINT8:		to.data.u8 = from.data.u8;							break;
		case TK_INT16:		to.data.u8 = static_cast<uint8>( from.data.i16 );	break;
		case TK_UINT16:		to.data.u8 = static_cast<uint8>( from.data.u16 );	break;
		case TK_INT32:		to.data.u8 = static_cast<uint8>( from.data.i32 );	break;
		case TK_UINT32:		to.data.u8 = static_cast<uint8>( from.data.u32 );	break;
		case TK_INT64:		to.data.u8 = static_cast<uint8>( from.data.i64 );	break;
		case TK_UINT64:		to.data.u8 = static_cast<uint8>( from.data.u64 );	break;
		case TK_FLOAT:		to.data.u8 = static_cast<uint8>( from.data.f );		break;
		case TK_DOUBLE:		to.data.u8 = static_cast<uint8>( from.data.d );		break;
		case TK_ENUM:		to.data.u8 = static_cast<uint8>( from.data.u32 );	break;
		}
		break;

	case TK_INT16:
		switch( from.kind )
		{
		case TK_BOOLEAN:	to.data.i16 = static_cast<int16>( from.data.b );	break;
		case TK_INT8:		to.data.i16 = static_cast<int16>( from.data.i8 );	break;
		case TK_UINT8:		to.data.i16 = static_cast<int16>( from.data.u8 );	break;
		case TK_INT16:		to.data.i16 = from.data.i16;						break;
		case TK_UINT16:		to.data.i16 = static_cast<int16>( from.data.u16 );	break;
		case TK_INT32:		to.data.i16 = static_cast<int16>( from.data.i32 );	break;
		case TK_UINT32:		to.data.i16 = static_cast<int16>( from.data.u32 );	break;
		case TK_INT64:		to.data.i16 = static_cast<int16>( from.data.i64 );	break;
		case TK_UINT64:		to.data.i16 = static_cast<int16>( from.data.u64 );	break;
		case TK_FLOAT:		to.data.i16 = static_cast<int16>( from.data.f );	break;
		case TK_DOUBLE:		to.data.i16 = static_cast<int16>( from.data.d );	break;
		case TK_ENUM:		to.data.i16 = static_cast<int16>( from.data.u32 );	break;
		}
		break;

	case TK_UINT16:
		switch( from.kind )
		{
		case TK_BOOLEAN:	to.data.u16 = static_cast<uint16>( from.data.b );	break;
		case TK_INT8:		to.data.u16 = static_cast<uint16>( from.data.i8 );	break;
		case TK_UINT8:		to.data.u16 = static_cast<uint16>( from.data.u8 );	break;
		case TK_INT16:		to.data.u16 = static_cast<uint16>( from.data.i16 );	break;
		case TK_UINT16:		to.data.u16 = from.data.u16;						break;
		case TK_INT32:		to.data.u16 = static_cast<uint16>( from.data.i32 );	break;
		case TK_UINT32:		to.data.u16 = static_cast<uint16>( from.data.u32 );	break;
		case TK_INT64:		to.data.u16 = static_cast<uint16>( from.data.i64 );	break;
		case TK_UINT64:		to.data.u16 = static_cast<uint16>( from.data.u64 );	break;
		case TK_FLOAT:		to.data.u16 = static_cast<uint16>( from.data.f );	break;
		case TK_DOUBLE:		to.data.u16 = static_cast<uint16>( from.data.d );	break;
		case TK_ENUM:		to.data.u16 = static_cast<uint16>( from.data.u32 );	break;
		}
		break;

	case TK_INT32:
		switch( from.kind )
		{
		case TK_BOOLEAN:	to.data.i32 = static_cast<int32>( from.data.b );	break;
		case TK_INT8:		to.data.i32 = static_cast<int32>( from.data.i8 );	break;
		case TK_UINT8:		to.data.i32 = static_cast<int32>( from.data.u8 );	break;
		case TK_INT16:		to.data.i32 = static_cast<int32>( from.data.i16 );	break;
		case TK_UINT16:		to.data.i32 = static_cast<int32>( from.data.u16 );	break;
		case TK_INT32:		to.data.i32 = from.data.i32;						break;
		case TK_UINT32:		to.data.i32 = static_cast<int32>( from.data.u32 );	break;
		case TK_INT64:		to.data.i32 = static_cast<int32>( from.data.i64 );	break;
		case TK_UINT64:		to.data.i32 = static_cast<int32>( from.data.u64 );	break;
		case TK_FLOAT:		to.data.i32 = static_cast<int32>( from.data.f );	break;
		case TK_DOUBLE:		to.data.i32 = static_cast<int32>( from.data.d );	break;
		case TK_ENUM:		to.data.i32 = static_cast<int32>( from.data.u32 );	break;
		}
		break;

	case TK_UINT32:
		switch( from.kind )
		{
		case TK_BOOLEAN:	to.data.u32 = static_cast<uint32>( from.data.b );	break;
		case TK_INT8:		to.data.u32 = static_cast<uint32>( from.data.i8 );	break;
		case TK_UINT8:		to.data.u32 = static_cast<uint32>( from.data.u8 );	break;
		case TK_INT16:		to.data.u32 = static_cast<uint32>( from.data.i16 );	break;
		case TK_UINT16:		to.data.u32 = static_cast<uint32>( from.data.u16 );	break;
		case TK_INT32:		to.data.u32 = static_cast<uint32>( from.data.i32 );	break;
		case TK_UINT32:		to.data.u32 = from.data.u32;						break;
		case TK_INT64:		to.data.u32 = static_cast<uint32>( from.data.i64 );	break;
		case TK_UINT64:		to.data.u32 = static_cast<uint32>( from.data.u64 );	break;
		case TK_FLOAT:		to.data.u32 = static_cast<uint32>( from.data.f );	break;
		case TK_DOUBLE:		to.data.u32 = static_cast<uint32>( from.data.d );	break;
		case TK_ENUM:		to.data.u32 = from.data.u32;						break;
		}
		break;

	case TK_INT64:
		switch( from.kind )
		{
		case TK_BOOLEAN:	to.data.i64 = static_cast<int64>( from.data.b );	break;
		case TK_INT8:		to.data.i64 = static_cast<int64>( from.data.i8 );	break;
		case TK_UINT8:		to.data.i64 = static_cast<int64>( from.data.u8 );	break;
		case TK_INT16:		to.data.i64 = static_cast<int64>( from.data.i16 );	break;
		case TK_UINT16:		to.data.i64 = static_cast<int64>( from.data.u16 );	break;
		case TK_INT32:		to.data.i64 = static_cast<int64>( from.data.i32 );	break;
		case TK_UINT32:		to.data.i64 = static_cast<int64>( from.data.u32 );	break;
		case TK_INT64:		to.data.i64 = from.data.i64;						break;
		case TK_UINT64:		to.data.i64 = static_cast<int64>( from.data.u64 );	break;
		case TK_FLOAT:		to.data.i64 = static_cast<int64>( from.data.f );	break;
		case TK_DOUBLE:		to.data.i64 = static_cast<int64>( from.data.d );	break;
		case TK_ENUM:		to.data.i64 = static_cast<int64>( from.data.u32 );	break;
		}
		break;

	case TK_UINT64:
		switch( from.kind )
		{
		case TK_BOOLEAN:	to.data.u64 = static_cast<uint64>( from.data.b );	break;
		case TK_INT8:		to.data.u64 = static_cast<uint64>( from.data.i8 );	break;
		case TK_UINT8:		to.data.u64 = static_cast<uint64>( from.data.u8 );	break;
		case TK_INT16:		to.data.u64 = static_cast<uint64>( from.data.i16 );	break;
		case TK_UINT16:		to.data.u64 = static_cast<uint64>( from.data.u16 );	break;
		case TK_INT32:		to.data.u64 = static_cast<uint64>( from.data.i32 );	break;
		case TK_UINT32:		to.data.u64 = static_cast<uint64>( from.data.u32 );	break;
		case TK_INT64:		to.data.u64 = static_cast<uint64>( from.data.i64 );	break;
		case TK_UINT64:		to.data.u64 = from.data.u64;						break;
		case TK_FLOAT:		to.data.u64 = static_cast<uint64>( from.data.f );	break;
		case TK_DOUBLE:		to.data.u64 = static_cast<uint64>( from.data.d );	break;
		case TK_ENUM:		to.data.u64 = static_cast<uint64>( from.data.u32 );	break;
		}
		break;

	case TK_FLOAT:
		switch( from.kind )
		{
		case TK_BOOLEAN:	to.data.f = static_cast<float>( from.data.b );		break;
		case TK_INT8:		to.data.f = static_cast<float>( from.data.i8 );		break;
		case TK_UINT8:		to.data.f = static_cast<float>( from.data.u8 );		break;
		case TK_INT16:		to.data.f = static_cast<float>( from.data.i16 );	break;
		case TK_UINT16:		to.data.f = static_cast<float>( from.data.u16 );	break;
		case TK_INT32:		to.data.f = static_cast<float>( from.data.i32 );	break;
		case TK_UINT32:		to.data.f = static_cast<float>( from.data.u32 );	break;
		case TK_INT64:		to.data.f = static_cast<float>( from.data.i64 );	break;
		case TK_UINT64:		to.data.f = static_cast<float>( from.data.u64 );	break;
		case TK_FLOAT:		to.data.f = from.data.f;							break;
		case TK_DOUBLE:		to.data.f = static_cast<float>( from.data.d );		break;
		case TK_ENUM:		to.data.f = static_cast<float>( from.data.u32 );	break;
		}
		break;

	case TK_DOUBLE:
		switch( from.kind )
		{
		case TK_BOOLEAN:	to.data.d = static_cast<double>( from.data.b );		break;
		case TK_INT8:		to.data.d = static_cast<double>( from.data.i8 );	break;
		case TK_UINT8:		to.data.d = static_cast<double>( from.data.u8 );	break;
		case TK_INT16:		to.data.d = static_cast<double>( from.data.i16 );	break;
		case TK_UINT16:		to.data.d = static_cast<double>( from.data.u16 );	break;
		case TK_INT32:		to.data.d = static_cast<double>( from.data.i32 );	break;
		case TK_UINT32:		to.data.d = static_cast<double>( from.data.u32 );	break;
		case TK_INT64:		to.data.d = static_cast<double>( from.data.i64 );	break;
		case TK_UINT64:		to.data.d = static_cast<double>( from.data.u64 );	break;
		case TK_FLOAT:		to.data.d = static_cast<double>( from.data.f );		break;
		case TK_DOUBLE:		to.data.d = from.data.d;							break;
		case TK_ENUM:		to.data.d = static_cast<double>( from.data.u32 );	break;
		}
		break;

	case TK_ENUM:
		switch( from.kind )
		{
		case TK_BOOLEAN:	to.data.u32 = static_cast<uint32>( from.data.b );	break;
		case TK_INT8:		to.data.u32 = static_cast<uint32>( from.data.i8 );	break;
		case TK_UINT8:		to.data.u32 = static_cast<uint32>( from.data.u8 );	break;
		case TK_INT16:		to.data.u32 = static_cast<uint32>( from.data.i16 );	break;
		case TK_UINT16:		to.data.u32 = static_cast<uint32>( from.data.u16 );	break;
		case TK_INT32:		to.data.u32 = static_cast<uint32>( from.data.i32 );	break;
		case TK_UINT32:		to.data.u32 = from.data.u32;						break;
		case TK_INT64:		to.data.u32 = static_cast<uint32>( from.data.i64 );	break;
		case TK_UINT64:		to.data.u32 = static_cast<uint32>( from.data.u64 );	break;
		case TK_FLOAT:		to.data.u32 = static_cast<uint32>( from.data.f );	break;
		case TK_DOUBLE:		to.data.u32 = static_cast<uint32>( from.data.d );	break;
		case TK_ENUM:		to.data.u32 = from.data.u32;						break;
		}

		// check if the resulting value is well defined for the enum
		if( to.data.u32 >= static_cast<EnumType*>( to.type )->getIdentifiers().getSize() )
			THROW_ILLEGAL_CAST( from, to, << ": value '" << to.data.u32 << "' is out of range for the enum" );
		break;
	}
}

bool testAndCopyCompatibleReferences( const __any::State& from, __any::State& to )
{
	if( to.kind != from.kind )
		return false;

	if( from.isConst == true && to.isConst == false )
		return false;

	to.data.ptr = from.data.ptr;

	// we're done for basic types
	if( to.kind < TK_ARRAY )
		return true;

	// additional tests for non-basic types
	assert( from.type && to.type );

	if( to.kind == TK_ARRAY )
	{
		// special treatment for arrays
		assert( !to.isReference && !from.isReference ); // arrays can never contain references

		// all array kinds are implicitly convertable to co::ArrayRange
		if( to.arrayKind == __any::State::AK_ArrayRange )
		{
			if( from.arrayKind == __any::State::AK_ArrayRange )
				to.arraySize = from.arraySize;

			TypeKind toKind = to.type->getKind();
			if( toKind != from.type->getKind() || to.isPointer != from.isPointer )
				return false;

			// there are some relaxations/conversions we can accept for co::ArrayRanges
			if( toKind != TK_INTERFACE )
			{
				// for types without inheritance, type must match and we can only add 'const':
				return to.type == from.type && ( !from.isPointerConst || to.isPointerConst );
			}
			else
			{
				// for types with inheritance, upcasts are only allowed if the pointer is const
				if( to.interfaceType == from.interfaceType )
					return ( !from.isPointerConst || to.isPointerConst );
				else
					return to.isPointerConst && from.interfaceType->isSubTypeOf( to.interfaceType );
			}
		}
		else
		{
			// array kinds other than co::ArrayRange must be retrieved by exact type
			return	to.arrayKind == from.arrayKind &&
					to.type == from.type &&
					to.isPointer == from.isPointer &&
					to.isPointerConst == from.isPointerConst;
		}
	}
	else if( to.kind < TK_INTERFACE )
	{
		// direct type check for enums, structs and native classes:
		return to.type == from.type;
	}
	else
	{
		// is-a type check for interfaces
		assert( to.kind == TK_INTERFACE );
		return from.interfaceType->isSubTypeOf( to.interfaceType );
	}
}

bool Any::operator==( const Any& other ) const
{
	if( _state.kind != other._state.kind || _state.type != other._state.type )
		return false;

	if( _state.kind == TK_NONE )
		return true;

	if( _state.isConst != other._state.isConst ||
		_state.isPointer != other._state.isPointer ||
		_state.isPointerConst != other._state.isPointerConst ||
		_state.isReference != other._state.isReference )
		return false;

	if( _state.kind == TK_ARRAY && ( _state.arrayKind != other._state.arrayKind ||
			( _state.arrayKind == __any::State::AK_ArrayRange
				&& _state.arraySize != other._state.arraySize ) ) )
		return false;

	// pointer comparison
	if( _state.kind == TK_ARRAY || _state.isPointer || _state.isReference )
		return ( _state.data.ptr == other._state.data.ptr );

	// value comparison
	bool res = false;
	switch( _state.kind )
	{
	case TK_BOOLEAN:	res = ( _state.data.b == other._state.data.b );		break;
	case TK_INT8:
	case TK_UINT8:		res = ( _state.data.u8 == other._state.data.u8 );	break;
	case TK_INT16:
	case TK_UINT16:		res = ( _state.data.u16 == other._state.data.u16 );	break;
	case TK_INT32:
	case TK_UINT32:		res = ( _state.data.u32 == other._state.data.u32 );	break;
	case TK_INT64:
	case TK_UINT64:		res = ( _state.data.u64 == other._state.data.u64 );	break;
	case TK_FLOAT:		res = ( _state.data.f == other._state.data.f );		break;
	case TK_DOUBLE:		res = ( _state.data.d == other._state.data.d );		break;
	case TK_ENUM:		res = ( _state.data.u32 == other._state.data.u32 );	break;
	default:
		assert( false );
	}

	return res;
}

void Any::setInterface( co::Interface* instance, co::InterfaceType* type )
{
	// type cannot be NULL when the instance is NULL
	assert( type || instance );

	_state.kind = TK_INTERFACE;
	_state.interfaceType = ( instance ? instance->getInterfaceType() : type );
	_state.isConst = false;
	_state.isPointer = true;
	_state.isPointerConst = false;
	_state.isReference = false;
	_state.data.ptr = instance;
}

inline void Any::setModifiers( uint32 flags )
{
	_state.isConst = ( ( flags & VarIsConst ) != 0 );
	_state.isPointer = ( ( flags & ( VarIsPointer | VarIsPointerConst ) ) != 0 );
	_state.isPointerConst = ( ( flags & VarIsPointerConst ) != 0 );
	_state.isReference = ( ( flags & VarIsReference ) != 0 );
}

void Any::setVariable( Type* type, uint32 flags, void* ptr )
{
	assert( type );
	TypeKind kind = type->getKind();

	if( kind < TK_ARRAY )
		return setBasic( kind, flags, ptr );

	// validate the type kind
	assert( kind < TK_COMPONENT && kind != TK_EXCEPTION );

	// for arrays, use setArray() instead
	assert( kind != TK_ARRAY );

	// set kind and type
	_state.kind = kind;
	_state.type = type;

	// set modifiers
	setModifiers( flags );

	// set reference
	if( _state.isPointer || _state.isReference )
	{
		_state.data.ptr = ptr;
	}
	else
	{
		assert( kind == co::TK_ENUM );
		_state.data.u32 = *reinterpret_cast<uint32*>( ptr );
	}
}

void Any::setBasic( co::TypeKind kind, uint32 flags, void* ptr )
{
	// invalid type kind?
	assert( kind > TK_NONE );

	// for non-primitive types, use setVariable() instead
	assert( kind < TK_ARRAY );

	_state.kind = kind;
	_state.type = NULL;

	// set modifiers
	setModifiers( flags );

	// set data for references
	if( _state.isPointer || _state.isReference )
	{
		_state.data.ptr = ptr;
	}
	else // set data for values
	{
		switch( kind )
		{
		case TK_BOOLEAN:	_state.data.b = *reinterpret_cast<bool*>( ptr ); break;
		case TK_INT8:		_state.data.i8 = *reinterpret_cast<int8*>( ptr ); break;
		case TK_UINT8:		_state.data.u8 = *reinterpret_cast<uint8*>( ptr ); break;
		case TK_INT16:		_state.data.i16 = *reinterpret_cast<int16*>( ptr ); break;
		case TK_UINT16:		_state.data.u16 = *reinterpret_cast<uint16*>( ptr ); break;
		case TK_INT32:		_state.data.i32 = *reinterpret_cast<int32*>( ptr ); break;
		case TK_UINT32:		_state.data.u32 = *reinterpret_cast<uint32*>( ptr ); break;
		case TK_INT64:		_state.data.i64 = *reinterpret_cast<int64*>( ptr ); break;
		case TK_UINT64:		_state.data.u64 = *reinterpret_cast<uint64*>( ptr ); break;
		case TK_FLOAT:		_state.data.f = *reinterpret_cast<float*>( ptr ); break;
		case TK_DOUBLE:		_state.data.d = *reinterpret_cast<double*>( ptr ); break;
		default:
			assert( false ); // forgot to pass a Ptr/Ref flag?
		}
	}
}

void Any::setArray( ArrayKind arrayKind, Type* elementType, uint32 flags, void* ptr, std::size_t size )
{
	assert( elementType );

#ifndef CORAL_NDEBUG
	TypeKind elementKind = elementType->getKind();
#endif

	// validate the element type kind
	assert( elementKind > TK_NONE && elementKind < TK_COMPONENT && elementKind != TK_EXCEPTION );

	// the element type cannot be an array
	assert( elementKind != TK_ARRAY );

	// arraySize should only be passed for co::ArrayRanges
	assert( arrayKind == AK_ArrayRange || size == 0 );

	// process the array kind
	switch( arrayKind )
	{
	case AK_StdVector:
		_state.arrayKind = __any::State::AK_StdVector;
		break;
	case AK_RefVector:
		flags |= ( VarIsPointer | VarIsPointerConst );
		_state.arrayKind = __any::State::AK_RefVector;
		break;
	case AK_ArrayRange:
		assert( ptr == NULL || size > 0 );
		_state.arrayKind = __any::State::AK_ArrayRange;
		_state.arraySize = size;
		break;
	default:
		assert( false );
	}

	// set kind and type
	_state.kind = TK_ARRAY;
	_state.type = elementType;

	// set modifiers
	setModifiers( flags );

	// set data
	_state.data.ptr = ptr;
}

void Any::castFrom( const __any::State& s )
{
	// are we dealing with arrays?
	if( _state.kind == TK_ARRAY || s.kind == TK_ARRAY )
	{
		if( testAndCopyCompatibleReferences( s, _state ) )
		{
			// VariableHelper<T>::retrieve() expects the arrayKind at set<T>() time, not at get<T>() time:
			_state.arrayKind = s.arrayKind;
			return;
		}
	}
	else // we're dealing with non-array variables
	{
		// what is it that we're casting to?
		if( _state.isReference )
		{
			// get<reference>: s must contain a reference
			if( s.isReference )
			{
				// if the referenced type is a pointer, pointer const'ness should match
				if( _state.isPointer == s.isPointer && _state.isPointerConst == s.isPointerConst )
				{
					// referenced types must be compatible
					if( testAndCopyCompatibleReferences( s, _state ) )
						return;
				}
			}
		}
		else if( _state.isPointer )
		{
			// get<pointer>: s must contain a pointer or a reference to a value
			if( ( s.isReference && !s.isPointer ) || s.isPointer )
			{
				// referenced types must be compatible
				if( testAndCopyCompatibleReferences( s, _state ) )
					return;
			}
		}
		else
		{
			// get<value>: s must contain a value
			if( !s.isReference && !s.isPointer )
			{
				// value types must be convertible (currently this is always true, by design)
				castValue( s, _state );
				return;
			}
		}
	}

	THROW_ILLEGAL_CAST( s, _state, );
}

} // namespace co
