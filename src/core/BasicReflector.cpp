/*
 * Coral - Lightweight C++ Component Framework
 * See Copyright Notice in Coral.h
 */

#include "BasicReflector.h"
#include <co/Any.h>
#include <co/IType.h>
#include <co/NotSupportedException.h>

namespace co {

// ------ BasicReflector -------------------------------------------------------

IType* BasicReflector::getType()
{
	return _type;
}

// ------ PODReflector ---------------------------------------------------------

template<typename T>
class PODReflector : public BasicReflector
{
public:
	PODReflector( IType* t ) : BasicReflector( t ) {;}

	uint32 getSize() { return sizeof(T); }

	void createValues( void* ptr, size_t numValues )
	{
		for( size_t i = 0; i < numValues; ++i )
			reinterpret_cast<T*>( ptr )[i] = 0;
	}

	void copyValues( const void* fromPtr, void* toPtr, size_t numValues )
	{
		for( size_t i = 0; i < numValues; ++i )
			reinterpret_cast<T*>( toPtr )[i] = reinterpret_cast<const T*>( fromPtr )[i];
	}

	void destroyValues( void*, size_t )
	{
		// NOP
	}
};

// ------ ClassReflector -------------------------------------------------------

template<typename T>
class ClassReflector : public BasicReflector
{
public:
	ClassReflector( IType* t ) : BasicReflector( t ) {;}

	uint32 getSize() { return sizeof(T); }

	void createValues( void* ptr, size_t numValues )
	{
		for( size_t i = 0; i < numValues; ++i )
			new( reinterpret_cast<T*>( ptr ) + i ) T;
	}

	void copyValues( const void* fromPtr, void* toPtr, size_t numValues )
	{
		for( size_t i = 0; i < numValues; ++i )
			reinterpret_cast<T*>( toPtr )[i] = reinterpret_cast<const T*>( fromPtr )[i];
	}

	void destroyValues( void* ptr, size_t numValues )
	{
		for( size_t i = 0; i < numValues; ++i )
			callDestructor( reinterpret_cast<T*>( ptr ) + i );
	}
};

// ------ ArrayReflector -------------------------------------------------------

class ArrayReflector : public BasicReflector
{
public:
	typedef std::vector<uint8> PseudoVector;

	ArrayReflector( IType* t ) : BasicReflector( t )
	{
		IType* elemType = static_cast<IArray*>( t )->getElementType();
		_elemReflector = elemType->getReflector();
		_elemSize = _elemReflector->getSize();
	}

	uint32 getSize() { return sizeof(PseudoVector); }

	void createValues( void* ptr, size_t numValues )
	{
		createVector( ptr, numValues );
	}

	void copyValues( const void* fromPtr, void* toPtr, size_t numValues )
	{
		assert( numValues == 1 ); // not meaningful

		destroyVector( toPtr );
		const PseudoVector* fromV = reinterpret_cast<const PseudoVector*>( fromPtr );
		size_t numElements = sizeToElements( fromV->size() );
		PseudoVector* toV = createVector( toPtr, numElements );
		if( numElements > 0 )
			_elemReflector->copyValues( &fromV->front(), &toV->front(), numElements );
	}

	void destroyValues( void* ptr, size_t numValues )
	{
		assert( numValues == 1 ); // not meaningful
		destroyVector( ptr );
	}

private:
	inline size_t sizeToElements( size_t totalSize )
	{
		assert( totalSize % _elemSize == 0 );
		return totalSize / _elemSize;
	}

	PseudoVector* createVector( void* ptr, size_t numElements )
	{
		PseudoVector* vec = new( ptr ) PseudoVector( numElements * _elemSize );
		if( numElements > 0 )
			_elemReflector->createValues( &vec->front(), numElements );
		return vec;
	}

	void destroyVector( void* ptr )
	{
		PseudoVector* vec = reinterpret_cast<PseudoVector*>( ptr );
		size_t totalSize = vec->size();
		if( totalSize > 0 )
		{
			size_t numElements = sizeToElements( totalSize );
			_elemReflector->destroyValues( &vec->front(), numElements );
		}
		
		vec->~vector();
	}

private:
	size_t _elemSize;
	RefPtr<IReflector> _elemReflector;
};

// ------ InternalComponentReflector -------------------------------------------

class InternalComponentReflector : public BasicReflector
{
public:
	InternalComponentReflector( IType* t ) : BasicReflector( t ) {;}
	
	uint32 getSize() { return 0; }

	IObject* newInstance()
	{
		throw NotSupportedException( "cannot instantiate an internal component" );
	}
};

// ------ BasicReflector Factory Method ----------------------------------------

IReflector* BasicReflector::create( IType* t )
{
	assert( t );

	IReflector* r = NULL;
	switch( t->getKind() )
	{
	case TK_ANY:		r = new ClassReflector<AnyValue>( t );		break;
	case TK_BOOL:		r = new PODReflector<bool>( t );			break;
	case TK_INT8:		r = new PODReflector<int8>( t );			break;
	case TK_UINT8:		r = new PODReflector<uint8>( t );			break;
	case TK_INT16:		r = new PODReflector<int16>( t );			break;
	case TK_UINT16:		r = new PODReflector<uint16>( t );			break;
	case TK_INT32:		r = new PODReflector<int32>( t );			break;
	case TK_UINT32:		r = new PODReflector<uint32>( t );			break;
	case TK_INT64:		r = new PODReflector<int64>( t );			break;
	case TK_UINT64:		r = new PODReflector<uint64>( t );			break;
	case TK_FLOAT:		r = new PODReflector<float>( t );			break;
	case TK_DOUBLE:		r = new PODReflector<double>( t );			break;
	case TK_STRING:		r = new ClassReflector<std::string>( t );	break;
	case TK_ARRAY:		r = new ArrayReflector( t );				break;
	case TK_ENUM:		r = new PODReflector<uint32>( t );			break;
	case TK_COMPONENT:	r = new InternalComponentReflector( t );	break;
	default: break;
	}

	assert( r );
	return r;
}

} // namespace co
