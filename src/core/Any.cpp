/*
 * Coral - Lightweight C++ Component Framework
 * See Copyright Notice in Coral.h
 */

#include "Any.h"
#include <co/IType.h>
#include <co/IEnum.h>
#include <co/INamespace.h>
#include <co/IReflector.h>
#include <co/IInterface.h>
#include <co/IllegalCastException.h>
#include <cstdlib>
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
		case co::__any::State::AK_Range:		arrayKindString = "co::Range"; break;
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
		co::INamespace* namespaceStack[NAMESPACE_STACK_MAX_SIZE];

		int namespaceStackSize = 0;
		co::INamespace* ns = s.type->getNamespace();

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
	const co::__any::State& s = a.getState();

	// type info
	out << "(" << s << ")";

	assert( s.kind != co::TK_EXCEPTION && s.kind != co::TK_COMPONENT );

	// print pointer
	if( s.kind == co::TK_ARRAY || s.isPointer || s.isReference )
	{
		if( s.data.ptr == NULL )
		{
			out << "NULL";
		}
		else if( s.kind == co::TK_STRING && !s.isPointer )
		{
			// special case: print strings up to 30 chars.
			const std::string& str = a.get<const std::string&>();
			out << '"';
			if( str.length() < 30 )
				out << str;
			else
				out << str.substr( 0, 27 ) << "...";
			out << '"';
		}
		else
		{
			out << s.data.ptr;
		}
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

void Any::clear()
{
	destroyObject();
	_state = State();
}

int32 Any::getSize() const
{
	if( _state.isPointer )
		return sizeof(void*);

	int32 size = -1;
	switch( _state.kind )
	{
	case TK_NONE:		break;
	case TK_ANY:		size = sizeof(Any);			break;
	case TK_BOOLEAN:	size = sizeof(bool);		break;
	case TK_INT8:		size = sizeof(int8);		break;
	case TK_UINT8:		size = sizeof(uint8);		break;
	case TK_INT16:		size = sizeof(int16);		break;
	case TK_UINT16:		size = sizeof(uint16);		break;
	case TK_INT32:		size = sizeof(int32);		break;
	case TK_UINT32:		size = sizeof(uint32);		break;
	case TK_INT64:		size = sizeof(int64);		break;
	case TK_UINT64:		size = sizeof(uint64);		break;
	case TK_FLOAT:		size = sizeof(float);		break;
	case TK_DOUBLE:		size = sizeof(double);		break;
	case TK_STRING:		size = sizeof(std::string);	break;
	case TK_ARRAY:
		size = _state.type->getReflector()->getSize();
		break;
	case TK_ENUM:		size = sizeof(uint32);		break;
	case TK_STRUCT:
	case TK_NATIVECLASS:
		size = _state.type->getReflector()->getSize();
		break;
	default:
		assert( false );
	}
	return size;
}

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
		if( to.data.u32 >= static_cast<IEnum*>( to.type )->getIdentifiers().getSize() )
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

		// all array kinds are implicitly convertable to co::Range
		if( to.arrayKind == __any::State::AK_Range )
		{
			if( from.arrayKind == __any::State::AK_Range )
				to.arraySize = from.arraySize;

			TypeKind toKind = to.type->getKind();
			if( toKind != from.type->getKind() || to.isPointer != from.isPointer )
				return false;

			// there are some relaxations/conversions we can accept for co::Ranges
			if( toKind != TK_INTERFACE )
			{
				// for types without inheritance, type must match and we can only add 'const':
				return to.type == from.type && ( !from.isPointerConst || to.isPointerConst );
			}
			else
			{
				// for types with inheritance, upcasts are only allowed if the pointer is const
				if( to.getInterface() == from.getInterface() )
					return ( !from.isPointerConst || to.isPointerConst );
				else
					return to.isPointerConst && ( from.getInterface() == to.getInterface() ||
													from.getInterface()->isSubTypeOf( to.getInterface() ) );
			}
		}
		else
		{
			// array kinds other than co::Range must be retrieved by exact type, and cannot be null
			assert( from.data.ptr != NULL );
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
		return from.getInterface() == to.getInterface() || from.getInterface()->isSubTypeOf( to.getInterface() );
	}
}

void Any::setService( IService* instance, IInterface* type )
{
	// type cannot be NULL when the instance is NULL
	assert( type || instance );

	_state.kind = TK_INTERFACE;
	_state.type = ( instance ? instance->getInterface() : type );
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

void Any::setVariable( IType* type, uint32 flags, void* ptr )
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
		assert( kind == TK_ENUM );
		_state.data.u32 = *reinterpret_cast<uint32*>( ptr );
	}
}

void Any::setBasic( TypeKind kind, uint32 flags, void* ptr )
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

void Any::setArray( ArrayKind arrayKind, IType* elementType, uint32 flags, void* ptr, size_t size )
{
	assert( elementType );

#ifndef CORAL_NDEBUG
	TypeKind elementKind = elementType->getKind();
#endif

	// validate the element type kind
	assert( elementKind > TK_NONE && elementKind < TK_COMPONENT && elementKind != TK_EXCEPTION );

	// the element type cannot be an array
	assert( elementKind != TK_ARRAY );

	// arraySize should only be passed for co::Ranges
	assert( arrayKind == AK_Range || size == 0 );

	// process the array kind
	switch( arrayKind )
	{
	case AK_StdVector:
		assert( ptr != NULL );
		_state.arrayKind = __any::State::AK_StdVector;
		break;
	case AK_RefVector:
		assert( ptr != NULL );
		flags |= ( VarIsPointer | VarIsPointerConst );
		_state.arrayKind = __any::State::AK_RefVector;
		break;
	case AK_Range:
		assert( ptr == NULL || size > 0 );
		_state.arrayKind = __any::State::AK_Range;
		assert( size < MAX_UINT32 );
		_state.arraySize = static_cast<uint32>( size );
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

void Any::makeOut( IType* paramType )
{
	TypeKind paramKind = paramType->getKind();
	switch( paramKind )
	{
	case TK_ANY:
		if( _state.objectKind != TK_ANY )
			createAny();
		break;

	case TK_BOOLEAN:
		_state.objectKind = TK_BOOLEAN;
		if( isValid() )
			_object.data.b = get<bool>();
		set<bool&>( _object.data.b );
		break;

	case TK_INT8:
		_state.objectKind = TK_INT8;
		if( isValid() )
			_object.data.i8 = get<int8>();
		set<int8&>( _object.data.i8 );
		break;

	case TK_UINT8:
		_state.objectKind = TK_UINT8;
		if( isValid() )
			_object.data.u8 = get<uint8>();
		set<uint8&>( _object.data.u8 );
		break;

	case TK_INT16:
		_state.objectKind = TK_INT16;
		if( isValid() )
			_object.data.i16 = get<int16>();
		set<int16&>( _object.data.i16 );
		break;

	case TK_UINT16:
		_state.objectKind = TK_UINT16;
		if( isValid() )
			_object.data.u16 = get<uint16>();
		set<uint16&>( _object.data.u16 );
		break;

	case TK_INT32:
		_state.objectKind = TK_INT32;
		if( isValid() )
			_object.data.i32 = get<int32>();
		set<int32&>( _object.data.i32 );
		break;

	case TK_UINT32:
		_state.objectKind = TK_UINT32;
		if( isValid() )
			_object.data.u32 = get<uint32>();
		set<uint32&>( _object.data.u32 );
		break;

	case TK_INT64:
		_state.objectKind = TK_INT64;
		if( isValid() )
			_object.data.i64 = get<int64>();
		set<int64&>( _object.data.i64 );
		break;

	case TK_UINT64:
		_state.objectKind = TK_UINT64;
		if( isValid() )
			_object.data.u64 = get<uint64>();
		set<uint64&>( _object.data.u64 );
		break;

	case TK_FLOAT:
		_state.objectKind = TK_FLOAT;
		if( isValid() )
			_object.data.f = get<float>();
		set<float&>( _object.data.f );
		break;

	case TK_DOUBLE:
		_state.objectKind = TK_DOUBLE;
		if( isValid() )
			_object.data.d = get<double>();
		set<double&>( _object.data.d );
		break;

	case TK_STRING:
		if( isValid() )
		{
			assert( getKind() == TK_STRING );
			if( isConst() )
			{
				const std::string& originalStr = get<const std::string&>();
				createString() = originalStr;
			}
		}
		else
		{
			createString();
		}
		break;

	case TK_ARRAY:
		if( isValid() )
			assert( getKind() == TK_ARRAY && isConst() == false );
		else
		{
			IType* elementType = static_cast<IArray*>( paramType )->getElementType();
			TypeKind elementKind = elementType->getKind();

			PseudoVector& pv = createArray( elementType );
			setArray(
				( elementKind == TK_INTERFACE ? Any::AK_RefVector : Any::AK_StdVector ), elementType,
				( elementKind == TK_INTERFACE ? Any::VarIsPointer : Any::VarIsValue ), &pv );
		}
		break;

	case TK_ENUM:
		_state.objectKind = TK_ENUM;
		if( isValid() )
			_object.data.u32 = get<uint32>();
		setVariable( paramType, VarIsReference, &_object.data.u32 );
		break;

	case TK_STRUCT:
	case TK_NATIVECLASS:
		if( isValid() )
		{
			assert( paramType == getType() );
			assert( !isPointer() );
			setVariable( paramType, Any::VarIsReference, getState().data.ptr );
		}
		else
		{
			createComplexValue( paramType );
		}
		break;

	case TK_INTERFACE:
		_state.objectKind = TK_INTERFACE;
		_object.data.ptr = getState().data.ptr;
		setVariable( paramType, ( Any::VarIsPointer | Any::VarIsReference ), &_object.data.ptr );
		break;

	default:
		assert( false );
	}
}

void Any::makeIn()
{
	switch( getKind() )
	{
	case TK_ANY:
		{
			// ugly hack to retain the object kind
			uint8 objectKind = _state.objectKind;
			_state = get<const Any&>().getState();
			_state.objectKind = objectKind;
		}
		break;

	case TK_BOOLEAN:
	case TK_INT8:
	case TK_UINT8:
	case TK_INT16:
	case TK_UINT16:
	case TK_INT32:
	case TK_UINT32:
	case TK_INT64:
	case TK_UINT64:
	case TK_FLOAT:
	case TK_DOUBLE:
		assert( isReference() && !isPointer() );
		setBasic( getKind(), Any::VarIsValue, getState().data.ptr );
		break;

	case TK_STRING:
	case TK_ARRAY:
		// empty
		break;

	case TK_ENUM:
		assert( isReference() && !isPointer() );
		setVariable( getType(), Any::VarIsValue, getState().data.ptr );
		break;

	case TK_STRUCT:
	case TK_NATIVECLASS:
		// empty
		break;

	case TK_INTERFACE:
		setService( reinterpret_cast<IService*>( _object.data.ptr ), getInterface() );
		break;

	default:
		assert( false );
	}
}

Any& Any::createAny()
{
	if( _state.objectKind == TK_ANY )
	{
		Any& any = *reinterpret_cast<Any*>( _object.data.ptr );
		any.clear();
		return any;
	}

	if( _state.objectKind != TK_NONE )
		throw Exception( "co::Any::createAny() called while a co::Any contains an object" );

	Any* res = new Any();

	// preserve the current variable (if any) in the new co::Any
	if( isValid() )
		res->_state = _state;

	_state.objectKind = TK_ANY;
	_object.data.ptr = res;
	set<Any&>( *res );

	return *res;
}

// avoid incorrect warning about breaking strict-aliasing rules in GCC 4.x
template<typename T>
inline T* reinterpretPtr( const void* ptr )
{
	return reinterpret_cast<T*>( const_cast<void*>( ptr ) );
}

std::string& Any::createString()
{
	destroyObject();
	new( _object.stringArea ) std::string();
	_state.objectKind = TK_STRING;
	std::string& res = *reinterpretPtr<std::string>( _object.stringArea );
	set<std::string&>( res );
	return res;
}

Any::PseudoVector& Any::createArray( IType* elementType, size_t n )
{
	destroyObject();

	_state.objectKind = TK_ARRAY;
	_object.array.reflector = elementType->getReflector();

	PseudoVector* res = reinterpret_cast<PseudoVector*>( _object.array.vectorArea );

	TypeKind elementKind = elementType->getKind();
	setArray( ( elementKind == TK_INTERFACE ? AK_RefVector : AK_StdVector ), elementType,
				( elementKind == TK_INTERFACE ? VarIsPointer : VarIsValue ), res );

	if( n == 0 )
	{
		new( _object.array.vectorArea ) PseudoVector();
		return *res;
	}

	switch( elementKind )
	{
	case TK_ANY:
		new( _object.array.vectorArea ) std::vector<Any>( n );
		break;

	case TK_BOOLEAN:
	case TK_INT8:
	case TK_UINT8:
	case TK_INT16:
	case TK_UINT16:
	case TK_INT32:
	case TK_UINT32:
	case TK_INT64:
	case TK_UINT64:
	case TK_FLOAT:
	case TK_DOUBLE:
	case TK_ENUM:
		new( _object.array.vectorArea ) PseudoVector( _object.array.reflector->getSize() * n );
		break;

	case TK_STRING:
		new( _object.array.vectorArea ) std::vector<std::string>( n );
		break;

	case TK_STRUCT:
	case TK_NATIVECLASS:
		{
			size_t elementSize = _object.array.reflector->getSize();
			new( _object.array.vectorArea ) PseudoVector( elementSize * n );

			// call the constructor of each element
			uint8* beginPtr = &res->front();
			uint8* endPtr = beginPtr + ( elementSize * n );

			for( uint8* p = beginPtr; p < endPtr; p += elementSize )
				_object.array.reflector->createValue( p );
		}
		break;

	case TK_INTERFACE:
		new( _object.array.vectorArea ) RefVector<IService>( n );
		break;

	default:
		assert( false );
	}

	return *res;
}

void Any::swapArray( const Any& other )
{
	assert( _state.kind == TK_ARRAY && other._state.kind == TK_ARRAY );
	assert( _state.type == other._state.type );
	assert( _state.isPointer == other._state.isPointer );
	assert( _state.isReference == other._state.isReference );
	assert( _state.arrayKind == other._state.arrayKind );
	assert( _state.arrayKind == State::AK_StdVector || _state.arrayKind == State::AK_RefVector );
	assert( _state.objectKind == TK_ARRAY ); // the array must be allocated in this co::Any
	PseudoVector* myVec = reinterpret_cast<PseudoVector*>( _object.array.vectorArea );
	PseudoVector* otherVec = reinterpret_cast<PseudoVector*>( other._state.data.ptr );
	myVec->swap( *otherVec );
}

void* Any::createComplexValue( IType* type )
{
	destroyObject();

	_object.complex.reflector = type->getReflector();
	size_t size = _object.complex.reflector->getSize();

	void* res;
	if( size > INPLACE_CAPACITY )
	{
		_state.objectKind = TK_STRUCT;
		res = _object.complex.ptr = malloc( size );
	}
	else
	{
		_state.objectKind = TK_NATIVECLASS;
		res = _object.complex.inplaceArea;
	}

	_object.complex.reflector->createValue( res );
	setVariable( type, Any::VarIsReference, res );

	return res;
}

void Any::destroyObject()
{
	switch( _state.objectKind )
	{
	case TK_NONE:
		return;

	case TK_ANY:
		delete reinterpret_cast<Any*>( _object.data.ptr );
		break;

	case TK_STRING:
		reinterpretPtr<std::string>( _object.stringArea )->~basic_string();
		break;

	case TK_ARRAY:
		{
			// call the destructor of each element
			PseudoVector* pv = reinterpret_cast<PseudoVector*>( _object.array.vectorArea );
			switch( _object.array.reflector->getType()->getKind() )
			{
			case TK_ANY:
				reinterpret_cast<std::vector<Any>*>( pv )->~vector();
				break;

			case TK_BOOLEAN:
			case TK_INT8:
			case TK_UINT8:
			case TK_INT16:
			case TK_UINT16:
			case TK_INT32:
			case TK_UINT32:
			case TK_INT64:
			case TK_UINT64:
			case TK_FLOAT:
			case TK_DOUBLE:
			case TK_ENUM:
				pv->~vector();
				break;

			case TK_STRING:
				reinterpret_cast<std::vector<std::string>*>( pv )->~vector();
				break;

			case TK_STRUCT:
			case TK_NATIVECLASS:
				{
					size_t arraySize = pv->size();
					if( arraySize > 0 )
					{
						uint8* beginPtr = &pv->front();
						uint8* endPtr = beginPtr + arraySize;

						size_t elementSize = _object.array.reflector->getSize();
						assert( arraySize % elementSize == 0 );

						for( uint8* p = beginPtr; p < endPtr; p += elementSize )
							_object.array.reflector->destroyValue( p );
					}
					pv->~vector();
				}
				break;

			case TK_INTERFACE:
				reinterpret_cast<RefVector<IService>*>( pv )->~RefVector();
				break;

			default:
				assert( false );
			}
		}
		break;

	case TK_STRUCT:
		_object.complex.reflector->destroyValue( _object.complex.ptr );
		free( _object.complex.ptr );
		break;

	case TK_NATIVECLASS:
		_object.complex.reflector->destroyValue( _object.complex.inplaceArea );
		break;

	default:
		break;
	}

	_state.objectKind = TK_NONE;
}

void Any::swap( Any& other )
{
	State tmpState( _state );
	TemporaryObjectData tmpObject( _object );

	// copy 'other' to 'this'
	_state = other._state;
	_object = other._object;
	if( _state.data.ptr == &other._object )
		_state.data.ptr = &_object;

	// copy 'this' to 'other'
	other._state = tmpState;
	other._object = tmpObject;
	if( tmpState.data.ptr == &_object )
		other._state.data.ptr = &other._object;
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
			( _state.arrayKind == __any::State::AK_Range
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

void Any::castFrom( const State& s )
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

void Any::copy( const Any& other )
{
	destroyObject();

	_state = other._state;
	_state.objectKind = TK_NONE;

	// copy any temporary object from 'other'
	if( other._state.objectKind != TK_NONE )
	{
		switch( other._state.objectKind )
		{
		case TK_ANY:
			createAny() = *reinterpret_cast<Any*>( other._object.data.ptr );
			break;

		case TK_STRING:
			createString() = *reinterpretPtr<const std::string>( other._object.stringArea );
			break;

		case TK_ARRAY:
			{
				_state.objectKind = TK_ARRAY;
				_object.array.reflector = other._object.array.reflector;
				IType* elementType = _object.array.reflector->getType();
				switch( elementType->getKind() )
				{
				case TK_ANY:
					new( _object.array.vectorArea ) std::vector<Any>(
						*reinterpretPtr<const std::vector<Any> >( other._object.array.vectorArea ) );
					break;

				case TK_BOOLEAN:
				case TK_INT8:
				case TK_UINT8:
				case TK_INT16:
				case TK_UINT16:
				case TK_INT32:
				case TK_UINT32:
				case TK_INT64:
				case TK_UINT64:
				case TK_FLOAT:
				case TK_DOUBLE:
				case TK_ENUM:
					new( _object.array.vectorArea ) PseudoVector(
							*reinterpretPtr<const PseudoVector>( other._object.array.vectorArea ) );
					break;

				case TK_STRING:
					new( _object.array.vectorArea ) std::vector<std::string>(
							*reinterpretPtr<const std::vector<std::string> >( other._object.array.vectorArea ) );
					break;

				case TK_STRUCT:
				case TK_NATIVECLASS:
					{
						const PseudoVector* opv = reinterpret_cast<const PseudoVector*>( other._object.array.vectorArea );
						size_t arraySize = opv->size();

						new( _object.array.vectorArea ) PseudoVector( arraySize );
						PseudoVector* pv = reinterpret_cast<PseudoVector*>( _object.array.vectorArea );

						if( arraySize > 0 )
						{
							const uint8* opvStart = &opv->front();
							uint8* pvStart = &pv->front();

							size_t elementSize = _object.array.reflector->getSize();
							assert( arraySize % elementSize == 0 );

							for( size_t offset = 0; offset < arraySize; offset += elementSize )
								_object.complex.reflector->copyValue( opvStart + offset, pvStart + offset );
						}
					}
					break;

				case TK_INTERFACE:
					new( _object.array.vectorArea ) RefVector<IService>(
							*reinterpretPtr<const RefVector<IService> >( other._object.array.vectorArea ) );
					break;

				default:
					assert( false );
				}
			}
			break;

		case TK_STRUCT:
			other._object.complex.reflector->copyValue( other._object.complex.ptr,
					createComplexValue( other._object.complex.reflector->getType() ) );
			break;

		case TK_NATIVECLASS:
			other._object.complex.reflector->copyValue( other._object.complex.inplaceArea,
					createComplexValue( other._object.complex.reflector->getType() ) );
			break;

		default:
			break;
		}
	}

	assert( _state.objectKind == other._state.objectKind );
}

} // namespace co
