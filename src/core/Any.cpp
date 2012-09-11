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
#include <co/IllegalStateException.h>
#include <cstdlib>
#include <sstream>

/****************************************************************************/
/* Global iostream Insertion Operators                                      */
/****************************************************************************/

std::ostream& operator<<( std::ostream& out, const co::__any::State& s )
{
	if( !s.type )
		return out << co::TK_STRINGS[co::TK_NONE];

	return out << ( s.isIn ? "in " : "out " ) << s.type->getFullName();
}

std::ostream& operator<<( std::ostream& out, const co::Any& a )
{
	const co::__any::State& s = a.state;

	// type info
	out << "(" << s << ")";

	if( !s.type )
		return out;

	co::TypeKind k = s.type->getKind();
	assert( k != co::TK_EXCEPTION && k != co::TK_COMPONENT );

	// print pointer
	if( !s.isIn || !co::isScalarType( k ) )
	{
		if( s.data.ptr == NULL )
		{
			out << "NULL";
		}
		else if( k == co::TK_STRING )
		{
			// special case: print strings up to 30 chars.
			std::string str;
			a.get( str );
			if( str.length() > 30 )
			{
				str.resize( 27 );
				str.append( "..." );
			}
			out << '"' << str << '"';
		}
		else
		{
			out << s.data.ptr;
		}
	}
	else
	{
		// print value
		switch( k )
		{
		case co::TK_BOOL:	out << ( s.data.b ? "true" : "false" );	break;
		case co::TK_INT8:	out << s.data.i8;	break;
		case co::TK_UINT8:	out << s.data.u8;	break;
		case co::TK_INT16:	out << s.data.i16;	break;
		case co::TK_UINT16:	out << s.data.u16;	break;
		case co::TK_INT32:	out << s.data.i32;	break;
		case co::TK_UINT32:	out << s.data.u32;	break;
		case co::TK_INT64:	out << s.data.i64;	break;
		case co::TK_UINT64:	out << s.data.u64;	break;
		case co::TK_FLOAT:	out << s.data.f;	break;
		case co::TK_DOUBLE:	out << s.data.d;	break;
		case co::TK_ENUM:	out << s.data.u32;	break;
		default:
			assert( false );
		}
	}

	return out;
}

/****************************************************************************/
/* Auxiliary Functions to Cast Between Scalar Types                         */
/****************************************************************************/

namespace co {

template<typename From, typename To>
inline void castScalar( const void* from, void* to )
{
	*reinterpret_cast<To*>( to ) = static_cast<To>( *reinterpret_cast<const From*>( from ) );
}

template<typename From>
inline void castScalar( const void* from, TypeKind toKind, void* to )
{
	switch( toKind )
	{
	case TK_BOOL:	castScalar<From, bool>( from, to );		break;
	case TK_INT8:	castScalar<From, int8>( from, to );		break;
	case TK_UINT8:	castScalar<From, uint8>( from, to );	break;
	case TK_INT16:	castScalar<From, int16>( from, to );	break;
	case TK_UINT16:	castScalar<From, uint16>( from, to );	break;
	case TK_INT32:	castScalar<From, int32>( from, to );	break;
	case TK_UINT32:	castScalar<From, uint32>( from, to );	break;
	case TK_INT64:	castScalar<From, int64>( from, to );	break;
	case TK_UINT64:	castScalar<From, uint64>( from, to );	break;
	case TK_FLOAT:	castScalar<From, float>( from, to );	break;
	case TK_DOUBLE:	castScalar<From, double>( from, to );	break;
	case TK_ENUM:	castScalar<From, uint32>( from, to );	break;
	default:		assert( false );
	}
}

void castScalar( TypeKind fromKind, const void* from, TypeKind toKind, void* to )
{
	assert( isScalarType( toKind ) && isScalarType( fromKind ) );
	switch( fromKind )
	{
	case TK_BOOL:	castScalar<bool>( from, toKind, to );	break;
	case TK_INT8:	castScalar<int8>( from, toKind, to );	break;
	case TK_UINT8:	castScalar<uint8>( from, toKind, to );	break;
	case TK_INT16:	castScalar<int16>( from, toKind, to );	break;
	case TK_UINT16:	castScalar<uint16>( from, toKind, to );	break;
	case TK_INT32:	castScalar<int32>( from, toKind, to );	break;
	case TK_UINT32:	castScalar<uint32>( from, toKind, to );	break;
	case TK_INT64:	castScalar<int64>( from, toKind, to );	break;
	case TK_UINT64:	castScalar<uint64>( from, toKind, to );	break;
	case TK_FLOAT:	castScalar<float>( from, toKind, to );	break;
	case TK_DOUBLE:	castScalar<double>( from, toKind, to );	break;
	case TK_ENUM:	castScalar<uint32>( from, toKind, to );	break;
	default:		assert( false );
	}
}

/****************************************************************************/
/* General Utility Functions                                                */
/****************************************************************************/

inline bool typesMatch( TypeKind kSub, TypeKind kSuper, IType* tSub, IType* tSuper )
{
	return tSub == tSuper ||
		( isPolymorphicType( kSub ) && kSub == kSuper && tSub->isA( tSuper ) );
}

typedef std::vector<uint8> StdVector;

inline co::uint8* getVectorData( void* ptr  )
{
	StdVector& pv = *reinterpret_cast<StdVector*>( ptr );
	return &pv[0];
}

inline co::uint8* getVectorData( const __any::State& s )
{
	return getVectorData( s.data.ptr );
}

inline size_t getVectorSize( const __any::State& s )
{
	StdVector& v = *reinterpret_cast<StdVector*>( s.data.ptr );
	IType* elemType = static_cast<IArray*>( s.type )->getElementType();
	return v.size() / elemType->getReflector()->getSize();
}

#define THROW_ILLEGAL_CAST( from, to, MORE_MESSAGES ) \
	{ std::stringstream sstream; \
	sstream << "illegal cast from '" << from << "' to '" << to << "'" MORE_MESSAGES; \
	throw co::IllegalCastException( sstream.str() ); }

/****************************************************************************/
/* co::Any                                                                  */
/****************************************************************************/

bool Any::isA( IType* type ) const
{
	assert( type );

	IType* myType = state.type;
	while( myType )
	{
		if( myType == type )
			return true;

		TypeKind k = myType->getKind();
		if( k == TK_ANY )
		{
			myType = reinterpret_cast<AnyValue*>( state.data.ptr )->getType();
			continue;
		}

		if( !isPolymorphicType( k ) )
			return false;

		return k == type->getKind() && myType->isA( type );
	}

	return false;
}

Any Any::asIn() const
{
	if( state.isIn || !state.type )
		return *this;

	TypeKind k = state.type->getKind();
	if( k == TK_ANY )
		return reinterpret_cast<const AnyValue*>( state.data.ptr )->getAny().asIn();

	if( k == TK_ARRAY )
	{
		size_t size = getVectorSize( state );
		void* data = getVectorData( state );
		return Any( true, state.type, data, size );
	}

	return Any( true, state.type, state.data.ptr );
}

size_t Any::getCount() const
{
	if( state.type && state.type->getKind() == TK_ARRAY )
		return  state.isIn ? state.size : getVectorSize( state );

	return 1;
}

size_t Any::getElementSize() const
{
	IType* type = state.type;
	if( !type )
		return 0;

	if( type->getKind() == TK_ARRAY )
		type = static_cast<IArray*>( type )->getElementType();

	return type->getReflector()->getSize();
}

void Any::set( bool isIn, IType* type, const void* addr, size_t size )
{
	// type must be a valid data type
	assert( type && isDataType( type->getKind() ) );

	// we can point to a co::AnyValue, but not to another co::Any
	assert( type->getKind() != TK_ANY || isIn == false );

	state.type = type;
	state.isIn = isIn;
	state.size = size;
	state.data.cptr = addr;

	if( !isIn )
		return;

	TypeKind k = type->getKind();
	switch( k )
	{
	case TK_BOOL: state.data.b = *reinterpret_cast<const bool*>( addr ); break;
	case TK_INT8:
	case TK_UINT8: state.data.u8 = *reinterpret_cast<const uint8*>( addr ); break;
	case TK_INT16:
	case TK_UINT16: state.data.u16 = *reinterpret_cast<const uint16*>( addr ); break;
	case TK_INT32:
	case TK_UINT32: state.data.u32 = *reinterpret_cast<const uint32*>( addr ); break;
	case TK_INT64:
	case TK_UINT64: state.data.u64 = *reinterpret_cast<const uint64*>( addr ); break;
	case TK_FLOAT: state.data.f = *reinterpret_cast<const float*>( addr ); break;
	case TK_DOUBLE: state.data.d = *reinterpret_cast<const double*>( addr ); break;
	case TK_ENUM: state.data.u32 = *reinterpret_cast<const uint32*>( addr ); break;
	case TK_INTERFACE: state.data.service = *reinterpret_cast<IService* const*>( addr ); break;
	default:
		assert( !isScalarType( k ) );
	}
}

template<typename T>
inline void putValue( void* ptr, const Any& value  )
{
	*reinterpret_cast<T*>( ptr ) = value.get<const T&>();
}

void Any::put( const Any& value ) const
{
	if( !isValid() || !isOut() )
		CORAL_THROW( IllegalStateException,
			"cannot write to a '" << state << "' variable" );

	Any in = value.asIn();
	TypeKind k = getType()->getKind();
	switch( k )
	{
	case TK_ANY:
		*reinterpret_cast<AnyValue*>( state.data.ptr ) = in;
		break;

	case TK_BOOL:	putValue<bool>( state.data.ptr, in );			break;
	case TK_INT8:	putValue<int8>( state.data.ptr, in );			break;
	case TK_UINT8:	putValue<uint8>( state.data.ptr, in );			break;
	case TK_INT16:	putValue<int16>( state.data.ptr, in );			break;
	case TK_UINT16:	putValue<uint16>( state.data.ptr, in );			break;
	case TK_INT32:	putValue<int32>( state.data.ptr, in );			break;
	case TK_UINT32:	putValue<uint32>( state.data.ptr, in );			break;
	case TK_INT64:	putValue<int64>( state.data.ptr, in );			break;
	case TK_UINT64:	putValue<uint64>( state.data.ptr, in );			break;
	case TK_FLOAT:	putValue<float>( state.data.ptr, in );			break;
	case TK_DOUBLE:	putValue<double>( state.data.ptr, in );			break;
	case TK_STRING:	putValue<std::string>( state.data.ptr, in );	break;

	case TK_ARRAY:
		resize( in.state.size );
		if( !in.state.type )
			break;

		if( in.state.type->getKind() != TK_ARRAY )
			THROW_ILLEGAL_CAST( in.state, state, );

		// are both arrays of the same exact type?
		if( getType() == in.getType() )
		{
			// fast path - straight copy using the element reflector
			IType* elemType = static_cast<IArray*>( getType() )->getElementType();
			elemType->getReflector()->copyValues( in.state.data.ptr,
							getVectorData( state.data.ptr ), in.state.size );
		}
		else
		{
			// slow path - each element must be converted through put()
			for( size_t i = 0; i < in.state.size; ++i )
				at( i ).put( in[i] );
		}
		break;

	case TK_ENUM:
		{
			TypeKind vK = in.isValid() ? in.getType()->getKind() : TK_NONE;
			IEnum* enumType = static_cast<IEnum*>( state.type );
			if( isScalarType( vK ) )
			{
				uint32 id = in.get<uint32>();
				if( id >= enumType->getIdentifiers().getSize() )
					THROW_ILLEGAL_CAST( in.state, state, << ": " << id
									   << " is out of range for the enum" );
				*reinterpret_cast<uint32*>( state.data.ptr ) = id;
			}
			else if( vK == TK_STRING )
			{
				const std::string& str = *in.state.data.str;
				int32 id = enumType->getValueOf( str );
				if( id == -1 )
					THROW_ILLEGAL_CAST( in.state, state,
						<< ": no such identifier '" << str << "' in the enum" );
				*reinterpret_cast<uint32*>( state.data.ptr ) = id;
			}
			else
				THROW_ILLEGAL_CAST( in.state, state, );
		}
		break;

	case TK_STRUCT:
	case TK_NATIVECLASS:
		if( in.getType() != getType() )
			THROW_ILLEGAL_CAST( in.state, state, );
		getType()->getReflector()->copyValues( in.state.data.ptr, state.data.ptr, 1 );
		break;

	case TK_INTERFACE:
		{
			IService* service = NULL;
			if( in.isValid() )
			{
				TypeKind vK = in.getType()->getKind();
				if( vK == TK_INTERFACE && in.getType()->isA( state.type ) )
					service = in.get<IService*>();
				else
					THROW_ILLEGAL_CAST( in.state, state, );
			}
			*reinterpret_cast<RefPtr<IService>*>( state.data.ptr ) = service;
		}
		break;

	default:
		assert( false );
	}
}

void Any::swapArrays( const Any& other ) const
{
	assert( state.type && state.type->getKind() == TK_ARRAY );
	assert( state.type == other.state.type );
	assert( !state.isIn && !other.state.isIn );
	StdVector* myVec = reinterpret_cast<StdVector*>( state.data.ptr );
	StdVector* otherVec = reinterpret_cast<StdVector*>( other.state.data.ptr );
	myVec->swap( *otherVec );
}

void Any::resize( size_t n ) const
{
	assert( state.type && !state.isIn && state.type->getKind() == TK_ARRAY );

	StdVector& v = *reinterpret_cast<StdVector*>( state.data.ptr );
	IType* elemType = static_cast<IArray*>( state.type )->getElementType();
	IReflector* elemReflector = elemType->getReflector();
	size_t elemSize = elemReflector->getSize();
	size_t currentSize = v.size();
	size_t currentN = currentSize / elemSize;
	if( currentN == n )
		return; // nothing to be done

	uint8* addr = &v.front();
	size_t newSize = n * elemSize;

	if( n < currentN )
	{
		// reduce the vector size
		elemReflector->destroyValues( addr + newSize, currentN - n );
		v.resize( newSize );
		return;
	}

	// expand the vector size
	if( v.capacity() < newSize )
	{
		// requires reallocation
		StdVector tmp( newSize );
		uint8* newAddr = &tmp.front();
		elemReflector->createValues( newAddr, n );
		if( currentN > 0 )
		{
			elemReflector->copyValues( addr, newAddr, currentN );
			elemReflector->destroyValues( addr, currentN );
		}
		tmp.swap( v );
	}
	else
	{
		// does not require reallocation
		v.resize( newSize );
		assert( addr == &v.front() );
		elemReflector->createValues( addr + currentSize, n - currentN );
	}
}

Any Any::at( size_t index ) const
{
	assert( state.type && state.type->getKind() == TK_ARRAY );
	assert( index < getCount() );

	IType* elemType = static_cast<IArray*>( state.type )->getElementType();
	co::uint8* addr = state.data.bytes;
	if( !state.isIn )
		addr = getVectorData( addr );
	else if( elemType->getKind() == TK_ANY )
		return reinterpret_cast<Any*>( addr )[index];

	addr += index * elemType->getReflector()->getSize();
	return Any( state.isIn, elemType, addr );
}

bool Any::operator==( const Any& other ) const
{
	if( state.type != other.state.type )
		return false;

	if( state.type == NULL )
		return true;

	if( state.isIn != other.state.isIn )
		return false;

	TypeKind k = state.type->getKind();
	if( state.isIn )
	{
		if( k == TK_ARRAY && state.size != other.state.size )
			return false;

		switch( k )
		{
		case TK_BOOL:	return state.data.b == other.state.data.b;
		case TK_INT8:
		case TK_UINT8:	return state.data.u8 == other.state.data.u8;
		case TK_INT16:
		case TK_UINT16:	return state.data.u16 == other.state.data.u16;
		case TK_INT32:
		case TK_UINT32:	return state.data.u32 == other.state.data.u32;
		case TK_INT64:
		case TK_UINT64:	return state.data.u64 == other.state.data.u64;
		case TK_FLOAT:	return state.data.f == other.state.data.f;
		case TK_DOUBLE:	return state.data.d == other.state.data.d;
		case TK_ENUM:	return state.data.u32 == other.state.data.u32;
		default:
			assert( !isScalarType( k ) );
		}
	}

	return state.data.ptr == other.state.data.ptr;
}

void Any::cast( __any::State& to ) const
{
	assert( state.type && to.type );

	TypeKind k = state.type->getKind();
	TypeKind toK = to.type->getKind();

	// if this contains an 'out any' we may have to dereference it
	if( k == TK_ANY )
	{
		assert( state.isIn == false );
		if( toK != TK_ANY )
		{
			reinterpret_cast<AnyValue*>( state.data.ptr )->getAny().cast( to );
			return;
		}
	}

	// are we casting to an 'in' or 'out' variable?
	if( !to.isIn )
	{
		// casting to an 'out' variable: both types must match
		if( state.isIn == false )
		{
			if( typesMatch( k, toK, state.type, to.type ) )
			{
				to.data.ptr = state.data.ptr;
				return;
			}
		}
	}
	else if( !state.isIn ) // casting from an 'out' to an 'in' variable
	{
		// we must extract an 'in' var from the 'out' var to carry on the cast
		return asIn().cast( to );
	}
	else // casting from an 'in' to an 'in' variable: conversions are possible
	{
		if( k == TK_ARRAY )
		{
			// check if both array element types match
			if( toK == TK_ARRAY )
			{
				IType* fromET = static_cast<IArray*>( state.type )->getElementType();
				IType* toET = static_cast<IArray*>( to.type )->getElementType();
				if( typesMatch( fromET->getKind(), toET->getKind(), fromET, toET ) )
				{
					to.size = state.size;
					to.data = state.data;
					return;
				}
			}
		}
		else if( typesMatch( k, toK, state.type, to.type ) )
		{
			// value types match, no conversion necessary
			to.data = state.data;
			return;
		}
		else // check for possible conversions
		{
			if( toK == TK_INTERFACE ) // converting to an interface
			{
				// null to interface conversion
				if( k == TK_NONE )
				{
					to.data.ptr = NULL;
					return;
				}
			}
			else if( isScalarType( toK ) ) // converting to a scalar
			{
				if( isScalarType( k ) ) // from a scalar
				{
					// scalar to scalar conversions
					castScalar( k, &state.data, toK, &to.data );

					// range check enums
					if( toK == TK_ENUM )
					{
						IEnum* enumType = static_cast<IEnum*>( to.type );
						if( to.data.u32 >= enumType->getIdentifiers().getSize() )
							THROW_ILLEGAL_CAST( state, to, << ": " << to.data.u32
											<< " is out of range for the enum" );
					}

					return;
				}
				else if( k == TK_STRING ) // from a string
				{
					const std::string& str = *reinterpret_cast<std::string*>( state.data.ptr );
					if( toK == TK_ENUM )
					{
						// string to enum conversion
						int32 value = static_cast<IEnum*>( to.type )->getValueOf( str );
						if( value == -1 )
							THROW_ILLEGAL_CAST( state, to, << ": no such identifier '"
											<< str << "' in the enum" );
						to.data.u32 = value;
					}
					else if( co::isIntegerType( toK ) )
					{
						// string to integer conversion
						if( isSignedIntegerType( toK ) )
						{
							long v = strtol( str.c_str(), NULL, 0 );
							castScalar<long>( &v, toK, &to.data );
						}
						else
						{
							unsigned long v = strtoul( str.c_str(), NULL, 0 );
							castScalar<unsigned long>( &v, toK, &to.data );
						}
					}
					else
					{
						// string to double conversion
						double d = strtod( str.c_str(), NULL );
						if( toK == TK_DOUBLE )
							to.data.d = d;
						else if( toK == TK_FLOAT )
							to.data.f = static_cast<float>( d );
						else
							assert( false );
					}
					return;
				}
			}
		}
	}

	THROW_ILLEGAL_CAST( state, to, );
}

/****************************************************************************/
/* co::AnyValue                                                             */
/****************************************************************************/

void AnyValue::clear()
{
	if( isNull() )
		return;

	// sanity check
	assert( _any.isOut() );

	IReflector* reflector = getType()->getReflector();
	reflector->destroyValues( _any.state.data.ptr, 1 );
	free( _any.state.data.ptr );

	_any.clear();
}

void* AnyValue::create( IType* type )
{
	assert( type && isDataType( type->getKind() ) );

	clear();

	IReflector* reflector = type->getReflector();
	uint32 size = reflector->getSize();
	void* ptr = malloc( size );

	try
	{
		reflector->createValues( ptr, 1 );
	}
	catch( ... )
	{
		free( ptr );
		throw;
	}

	_any.state.type = type;
	_any.state.data.ptr = ptr;

	return ptr;
}

void AnyValue::convert( IType* type )
{
	assert( type );

	if( getType() == type )
		return;

	co::AnyValue temp;
	temp.create( type );
	if( isValid() )
		temp.getAny().put( getAny() );
	swap( temp );
}

void AnyValue::copy( const Any& any )
{
	Any in = any.asIn();
	if( in.isNull() )
	{
		clear();
		return;
	}

	create( in.getType() );
	_any.put( in );
}

} // namespace co
