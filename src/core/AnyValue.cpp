/*
 * Coral - A C++ Component Framework.
 * See Copyright Notice in Coral.h
 */

#include "AnyValue.h"
#include <co/Reflector.h>
#include <cstdlib>

namespace co {

void AnyValue::clear()
{
	switch( _kind )
	{
	case TK_NONE:
		return;

	case TK_ANY:
		reinterpret_cast<Any*>( _v.anyArea )->~Any();
		break;

	case TK_STRING:
		reinterpret_cast<std::string*>( _v.stringArea )->~basic_string();
		break;

	case TK_ARRAY:
		{
			// call the destructor of each element
			PseudoVector* pv = reinterpret_cast<PseudoVector*>( _v.array.vectorArea );	
			switch( _v.complex.reflector->getType()->getKind() )
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
					size_t elementSize = _v.complex.reflector->getSize();
					assert( arraySize % elementSize == 0 );

					uint8* beginPtr = &( *pv )[0];
					uint8* endPtr = beginPtr + arraySize;

					for( uint8* p = beginPtr; p < endPtr; p += elementSize )
						_v.complex.reflector->destroyValue( p );
					
					pv->~vector();
				}
				break;

			case TK_INTERFACE:
				{
					// emulate a co::RefVector and decrement references
					std::vector<co::Interface*>& vec = *reinterpret_cast<std::vector<co::Interface*>*>( pv );
					for( std::vector<co::Interface*>::iterator it = vec.begin(); it != vec.end(); ++it )
					{
						co::Interface* itf = *it;
						if( itf )
							itf->componentRelease();
					}
					pv->~vector();
				}
				break;

			default:
				assert( false );
			}
		}
		break;

	case TK_STRUCT:
		_v.complex.reflector->destroyValue( _v.complex.ptr );
		free( _v.complex.ptr );
		break;

	case TK_NATIVECLASS:
		_v.complex.reflector->destroyValue( _v.complex.inplaceArea );
		break;

	default:
		break;
	}

	_kind = TK_NONE;
}

AnyValue::PseudoVector& AnyValue::createArray( Type* elementType, size_t n )
{
	clear();

	_kind = TK_ARRAY;
	_v.array.reflector = elementType->getReflector();
	
	PseudoVector& pv = *reinterpret_cast<PseudoVector*>( _v.array.vectorArea );

	if( n == 0 )
	{
		new( _v.array.vectorArea ) PseudoVector();
		return pv;
	}

	switch( elementType->getKind() )
	{
	case TK_ANY:
		new( _v.array.vectorArea ) std::vector<Any>( n );
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
	case TK_INTERFACE:
	case TK_ENUM:
		new( _v.array.vectorArea ) PseudoVector( _v.complex.reflector->getSize() * n );
		break;

	case TK_STRING:
		new( _v.array.vectorArea ) std::vector<std::string>( n );
		break;

	case TK_STRUCT:
	case TK_NATIVECLASS:
		{
			size_t elementSize = _v.complex.reflector->getSize();
			new( _v.array.vectorArea ) PseudoVector( elementSize * n );

			// call the constructor of each element
			uint8* beginPtr = &pv[0];
			uint8* endPtr = beginPtr + ( elementSize * n );

			for( uint8* p = beginPtr; p < endPtr; p += elementSize )
				_v.array.reflector->createValue( p, elementSize );
		}
		break;

	default:
		assert( false );
	}

	return pv;
}

void* AnyValue::createComplexValue( Type* type )
{
	clear();

	_v.complex.reflector = type->getReflector();
	size_t size = _v.complex.reflector->getSize();

	void* ptr;
	if( size > INPLACE_CAPACITY )
	{
		_kind = TK_STRUCT;
		ptr = _v.complex.ptr = malloc( size );
	}
	else
	{
		_kind = TK_NATIVECLASS;
		ptr = _v.complex.inplaceArea;
	}

	_v.complex.reflector->createValue( ptr, size );

	return ptr;
}

void AnyValue::makeOut( Type* paramType, Any& arg )
{
	TypeKind paramKind = paramType->getKind();
	switch( paramKind )
	{
	case TK_ANY:
		// if we had an auxiliary AnyValue we could accept any value for
		// an 'out any' parameter; currenty we only accept real Any's.
		createAny();
		if( arg.isValid() )
			getAny() = arg.get<const Any&>();
		arg.set<Any&>( getAny() );
		break;

	case TK_BOOLEAN:
		_kind = TK_BOOLEAN;
		if( arg.isValid() )
			_v.data.b = arg.get<bool>();
		arg.set<bool&>( _v.data.b );
		break;

	case TK_INT8:
		_kind = TK_INT8;
		if( arg.isValid() )
			_v.data.i8 = arg.get<int8>();
		arg.set<int8&>( _v.data.i8 );
		break;

	case TK_UINT8:
		_kind = TK_UINT8;
		if( arg.isValid() )
			_v.data.u8 = arg.get<uint8>();
		arg.set<uint8&>( _v.data.u8 );
		break;

	case TK_INT16:
		_kind = TK_INT16;
		if( arg.isValid() )
			_v.data.i16 = arg.get<int16>();
		arg.set<int16&>( _v.data.i16 );
		break;

	case TK_UINT16:
		_kind = TK_UINT16;
		if( arg.isValid() )
			_v.data.u16 = arg.get<uint16>();
		arg.set<uint16&>( _v.data.u16 );
		break;

	case TK_INT32:
		_kind = TK_INT32;
		if( arg.isValid() )
			_v.data.i32 = arg.get<int32>();
		arg.set<int32&>( _v.data.i32 );
		break;

	case TK_UINT32:
		_kind = TK_UINT32;
		if( arg.isValid() )
			_v.data.u32 = arg.get<uint32>();
		arg.set<uint32&>( _v.data.u32 );
		break;

	case TK_INT64:
		_kind = TK_INT64;
		if( arg.isValid() )
			_v.data.i64 = arg.get<int64>();
		arg.set<int64&>( _v.data.i64 );
		break;

	case TK_UINT64:
		_kind = TK_UINT64;
		if( arg.isValid() )
			_v.data.u64 = arg.get<uint64>();
		arg.set<uint64&>( _v.data.u64 );
		break;

	case TK_FLOAT:
		_kind = TK_FLOAT;
		if( arg.isValid() )
			_v.data.f = arg.get<float>();
		arg.set<float&>( _v.data.f );
		break;

	case TK_DOUBLE:
		_kind = TK_DOUBLE;
		if( arg.isValid() )
			_v.data.d = arg.get<double>();
		arg.set<double&>( _v.data.d );
		break;

	case TK_STRING:
		if( arg.isValid() )
			assert( arg.getKind() == TK_STRING && arg.isConst() == false );
		else
			arg.set<std::string&>( createString() );
		break;

	case TK_ARRAY:
		if( arg.isValid() )
			assert( arg.getKind() == TK_ARRAY && arg.isConst() == false );
		else
		{
			co::ArrayType* arrayType = dynamic_cast<co::ArrayType*>( paramType );
			assert( arrayType );

			co::Type* elementType = arrayType->getElementType();
			co::TypeKind elementKind = elementType->getKind();

			co::AnyValue::PseudoVector& pv = createArray( elementType );
			arg.setArray(
				( elementKind == co::TK_INTERFACE ? co::Any::AK_RefVector : co::Any::AK_StdVector ), elementType,
				( elementKind == co::TK_INTERFACE ? co::Any::VarIsPointer : co::Any::VarIsValue ), &pv );
		}
		break;

	case TK_ENUM:
		_kind = TK_ENUM;
		if( arg.isValid() )
			_v.data.u32 = arg.get<uint32>();
		arg.set<uint32&>( _v.data.u32 );
		break;

	case TK_STRUCT:
	case TK_NATIVECLASS:
		if( arg.isValid() )
		{
			assert( paramType == arg.getType() );
			assert( !arg.isPointer() );
			arg.setVariable( paramType, Any::VarIsReference, arg.getState().data.ptr );
		}
		else
		{
			void* ptr = createComplexValue( paramType );
			arg.setVariable( paramType, Any::VarIsReference, ptr );
		}
		break;

	case TK_INTERFACE:
		_kind = TK_INTERFACE;
		_v.data.ptr = arg.getState().data.ptr;
		arg.setVariable( paramType, ( Any::VarIsPointer | Any::VarIsReference ), &_v.data.ptr );
		break;

	default:
		assert( false );
	}
}

void AnyValue::makeIn( Any& arg )
{
	switch( arg.getKind() )
	{
	case TK_ANY:
		arg = getAny();
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
		assert( arg.isReference() && !arg.isPointer() );
		arg.setBasic( arg.getKind(), Any::VarIsValue, arg.getState().data.ptr );
		break;

	case TK_STRING:
		// empty
		break;

	case TK_ARRAY:
		// TODO
		break;

	case TK_ENUM:
		assert( arg.isReference() && !arg.isPointer() );
		arg.setVariable( arg.getType(), Any::VarIsValue, arg.getState().data.ptr );
		break;

	case TK_STRUCT:
	case TK_NATIVECLASS:
		// empty
		break;

	case TK_INTERFACE:
		arg.setInterface( reinterpret_cast<Interface*>( _v.data.ptr ), arg.getInterfaceType() );
		break;

	default:
		assert( false );
	}
}

} // namespace co
