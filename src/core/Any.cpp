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

/****************************************************************************/
/* General Utility Functions                                                */
/****************************************************************************/

namespace co {

inline void* getVectorData( const __any::State& s )
{
	std::vector<uint8>& pv = *reinterpret_cast<std::vector<uint8>*>( s.data.ptr );
	return &pv[0];
}

inline uint32 getVectorSize( const __any::State& s )
{
	std::vector<uint8>& pv = *reinterpret_cast<std::vector<uint8>*>( s.data.ptr );
	return pv.size() / s.type->getReflector()->getSize();
}

// avoid incorrect warning about breaking strict-aliasing rules in GCC 4.x
template<typename T>
inline T* reinterpretPtr( const void* ptr )
{
	return reinterpret_cast<T*>( const_cast<void*>( ptr ) );
}

void castScalar( TypeKind fromKind, const void* from, TypeKind toKind, void* to );

} // namespace co

#define THROW_ILLEGAL_CAST( from, to, MORE_MESSAGES ) \
	{ std::stringstream sstream; \
	sstream << "illegal cast from '" << from << "' to '" << to << "'" MORE_MESSAGES; \
	throw co::IllegalCastException( sstream.str() ); }

/****************************************************************************/
/* Global iostream Insertion Operators                                      */
/****************************************************************************/

std::ostream& operator<<( std::ostream& out, const co::__any::State& s )
{
	if( s.kind == co::TK_NONE )
		return out << co::TK_STRINGS[co::TK_NONE];

	assert( s.type != NULL );

	out << ( s.isIn ? "in " : "out " ) << s.type->getFullName();

	if( s.kind == co::TK_ARRAY )
		out << "[]";

	return out;
}

std::ostream& operator<<( std::ostream& out, const co::Any& a )
{
	const co::__any::State& s = a.getState();

	// type info
	out << "(" << s << ")";

	assert( s.kind != co::TK_EXCEPTION && s.kind != co::TK_COMPONENT );

	// print pointer
	if( !s.isIn || !co::isScalar( s.kind ) )
	{
		if( s.data.ptr == NULL )
		{
			out << "NULL";
		}
		else if( s.kind == co::TK_STRING )
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

/****************************************************************************/
/* co::Any                                                                  */
/****************************************************************************/

namespace co {

void Any::clear()
{
	destroyObject();
	_state = State();
}

uint32 Any::getSize() const
{
	if( _state.kind == TK_ARRAY )
	{
		return _state.isIn ? _state.size : getVectorSize( _state );
	}	
	return _state.type ? _state.type->getReflector()->getSize() : 0;
}

void Any::set( bool isIn, IType* type, const void* ptr, size_t size )
{
	assert( type );

	TypeKind kind = type->getKind();
	assert( isVariable( kind ) );

	if( kind == TK_ARRAY )
		type = static_cast<IArray*>( type )->getElementType();

	_state.type = type;
	_state.size = size;
	_state.kind = kind;
	_state.isIn = isIn;

	if( !isIn || !isScalar( kind ) )
		_state.data.cptr = ptr;
	else
		_state.data.u64 = *reinterpret_cast<const uint64*>( ptr );
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
			if( isIn() )
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
			assert( getKind() == TK_ARRAY && isOut() );
		else
			set( false, paramType, &createArray( static_cast<IArray*>( paramType )->getElementType() ) );
		break;

	case TK_ENUM:
		_state.objectKind = TK_ENUM;
		if( isValid() )
			_object.data.u32 = get<uint32>();
		set( false, paramType, &_object.data.u32 );
		break;

	case TK_STRUCT:
	case TK_NATIVECLASS:
		if( isValid() )
		{
			assert( paramType == _state.type );
			_state.isIn = false;
		}
		else
		{
			createComplexValue( paramType );
		}
		break;

	case TK_INTERFACE:
		_state.objectKind = TK_INTERFACE;
		_object.data.ptr = _state.data.ptr;
		set( false, paramType, &_object.data.ptr );
		break;

	default:
		assert( false );
	}
}

void Any::makeIn()
{
	assert( isOut() );
	if( _state.kind == TK_ANY )
	{
		// ugly hack to retain the object kind
		uint8 objectKind = _state.objectKind;
		_state = get<const Any&>().getState();
		_state.objectKind = objectKind;
	}
	else if( _state.kind == TK_INTERFACE )
	{
		set( true, _state.type, *reinterpret_cast<IService**>( _state.data.ptr ) );
	}
	else if( isScalar( _state.kind ) )
	{
		set( true, _state.type, _state.data.cptr );		
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
	_object.array.reflector->serviceRetain();

	PseudoVector* res = reinterpret_cast<PseudoVector*>( _object.array.vectorArea );
	set( false, co::getArrayOf( elementType ), res );

	if( n == 0 )
	{
		new( _object.array.vectorArea ) PseudoVector();
		return *res;
	}

	switch( elementType->getKind() )
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
		new( _object.array.vectorArea ) PseudoVector( _object.array.reflector->getSize() * n );
		_object.array.reflector->createValues( &res->front(), n );
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
	assert( _state.isIn == other._state.isIn );
	assert( _state.isIn == false );
	assert( _state.objectKind == TK_ARRAY ); // the array must be allocated in this co::Any
	PseudoVector* myVec = reinterpret_cast<PseudoVector*>( _object.array.vectorArea );
	PseudoVector* otherVec = reinterpret_cast<PseudoVector*>( other._state.data.ptr );
	myVec->swap( *otherVec );
}

void* Any::createComplexValue( IType* type )
{
	destroyObject();

	_object.complex.reflector = type->getReflector();
	_object.complex.reflector->serviceRetain();

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

	_object.complex.reflector->createValues( res, 1 );
	set( false, type, res );

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
						size_t elementSize = _object.array.reflector->getSize();
						assert( arraySize % elementSize == 0 );
						_object.array.reflector->destroyValues( &pv->front(), arraySize / elementSize );
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
			_object.array.reflector->serviceRelease();
		}
		break;

	case TK_STRUCT:
		_object.complex.reflector->destroyValues( _object.complex.ptr, 1 );
		_object.complex.reflector->serviceRelease();
		free( _object.complex.ptr );
		break;

	case TK_NATIVECLASS:
		_object.complex.reflector->destroyValues( _object.complex.inplaceArea, 1 );
		_object.complex.reflector->serviceRelease();
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

	if( _state.isIn != other._state.isIn )
		return false;

	if( _state.kind == TK_ARRAY && isIn() && _state.size != other._state.size )
		return false;

	// pointer comparison
	if( isOut() || !isScalar( _state.kind ) )
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

void Any::cast( const State& from, State& to )
{
	assert( from.type && to.type );

	if( to.kind == TK_INTERFACE )
	{
		if( from.kind == TK_INTERFACE && ( to.isIn || !from.isIn )
				&& from.getInterface()->isSubTypeOf( to.getInterface() ) )
		{
			// should we convert from 'out Itf' (RefPtr<Itf>*) to 'in Itf' (Itf*)?
			if( to.isIn && !from.isIn )
				to.data.service = *reinterpret_cast<IService**>( from.data.ptr );
			else
				to.data.ptr = from.data.ptr;
			return;
		}
	}
	else if( to.kind == TK_STRING )
	{
		if( from.kind == TK_STRING && ( to.isIn || !from.isIn ) )
		{
			to.data.ptr = from.data.ptr;
			return;
		}
	}
	else if( isScalar( to.kind ) )
	{
		if( isScalar( from.kind ) )
		{
			if( to.isIn )
			{
				const void* fromPtr = ( from.isIn ? &from.data.ptr : from.data.ptr );
				castScalar( from.kind, fromPtr, to.kind, &to.data.ptr );
				if( to.kind == TK_ENUM )
				{
					size_t numIds = static_cast<IEnum*>( to.type )->getIdentifiers().getSize();
					if( to.data.u32 >= numIds )
						THROW_ILLEGAL_CAST( from, to, << ": " << to.data.u32 <<
							" is out of range for the enum" );
				}
				return;
			}
			else if( !from.isIn && to.type == from.type ) // casting to an 'out scalar' (T&)
			{
				to.data.ptr = from.data.ptr;
				return;
			}
		}
	}
	else if( to.kind == TK_ANY || to.kind == TK_STRUCT || to.kind == TK_NATIVECLASS )
	{
		if( to.kind == from.kind && to.type == from.type && ( to.isIn || !from.isIn ) )
		{
			to.data.ptr = from.data.ptr;
			return;
		}
	}
	else if( to.kind == TK_ARRAY )
	{
		if( from.kind == TK_ARRAY )
		{
			if( to.isIn )
			{
				if( to.type == from.type || ( hasInheritance( to.type->getKind() )
					&& to.type->getKind() == from.type->getKind()
					&& from.getInterface()->isSubTypeOf( to.getInterface() ) ) )
				{
					if( from.isIn )
					{
						to.data.ptr = from.data.ptr;
						to.size = from.size;
					}
					else
					{
						to.data.ptr = getVectorData( from );
						to.size = getVectorSize( from );
					}
					return;
				}
			}
			else if( !from.isIn && to.type == from.type ) // casting to an 'out array' (vector&)
			{
				to.data.ptr = from.data.ptr;
				return;
			}
		}
	}
	else
	{
		assert( false );
	}

	THROW_ILLEGAL_CAST( from, to, );
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
				_object.array.reflector->serviceRetain();
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
							size_t elementSize = _object.array.reflector->getSize();
							assert( arraySize % elementSize == 0 );
							_object.complex.reflector->copyValues( &opv->front(), &pv->front(),
																	arraySize / elementSize );
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
			other._object.complex.reflector->copyValues( other._object.complex.ptr,
					createComplexValue( other._object.complex.reflector->getType() ), 1 );
			break;

		case TK_NATIVECLASS:
			other._object.complex.reflector->copyValues( other._object.complex.inplaceArea,
					createComplexValue( other._object.complex.reflector->getType() ), 1 );
			break;

		default:
			break;
		}
	}

	assert( _state.objectKind == other._state.objectKind );
}

template<typename From, typename To>
inline void castScalar( const void* from, void* to )
{
	*reinterpret_cast<To*>( to ) = static_cast<To>( *reinterpret_cast<const From*>( from ) );
}

template<typename To>
inline void castScalar( TypeKind fromKind, const void* from, void* to )
{
	switch( fromKind )
	{
	case TK_BOOLEAN:	castScalar<bool, To>( from, to );	break;
	case TK_INT8:		castScalar<int8, To>( from, to );	break;
	case TK_UINT8:		castScalar<uint8, To>( from, to );	break;
	case TK_INT16:		castScalar<int16, To>( from, to );	break;
	case TK_UINT16:		castScalar<uint16, To>( from, to );	break;
	case TK_INT32:		castScalar<int32, To>( from, to );	break;
	case TK_UINT32:		castScalar<uint32, To>( from, to );	break;
	case TK_INT64:		castScalar<int64, To>( from, to );	break;
	case TK_UINT64:		castScalar<uint64, To>( from, to );	break;
	case TK_FLOAT:		castScalar<float, To>( from, to );	break;
	case TK_DOUBLE:		castScalar<double, To>( from, to );	break;
	case TK_ENUM:		castScalar<uint32, To>( from, to );	break;
	default:			assert( false );
	}
}

void castScalar( TypeKind fromKind, const void* from, TypeKind toKind, void* to )
{
	assert( isScalar( toKind ) && isScalar( fromKind ) );

	switch( toKind )
	{
	case TK_BOOLEAN:	castScalar<bool>( fromKind, from, to );		break;
	case TK_INT8:		castScalar<int8>( fromKind, from, to );		break;
	case TK_UINT8:		castScalar<uint8>( fromKind, from, to );	break;
	case TK_INT16:		castScalar<int16>( fromKind, from, to );	break;
	case TK_UINT16:		castScalar<uint16>( fromKind, from, to );	break;
	case TK_INT32:		castScalar<int32>( fromKind, from, to );	break;
	case TK_UINT32:		castScalar<uint32>( fromKind, from, to );	break;
	case TK_INT64:		castScalar<int64>( fromKind, from, to );	break;
	case TK_UINT64:		castScalar<uint64>( fromKind, from, to );	break;
	case TK_FLOAT:		castScalar<float>( fromKind, from, to );	break;
	case TK_DOUBLE:		castScalar<double>( fromKind, from, to );	break;
	case TK_ENUM:		castScalar<uint32>( fromKind, from, to );	break;
	default:			assert( false );
	}
}

} // namespace co
