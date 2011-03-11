/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#ifndef _CO_ARRAYRANGE_H_
#define _CO_ARRAYRANGE_H_

#include <co/Platform.h>
#include <co/TypeTraits.h>
#include <vector>

namespace co {

#ifndef DOXYGEN

// Instructs an ArrayRange how to extract an array out of a custom container.
template<typename T, typename C>
struct ArrayRangeAdaptor
{
	static const bool isValid = false;
	static T* getData( C& ) { return 0; }
	static size_t getSize( C& ) { return 0; }
};

// We should always be able to create ArrayRanges out of const containers.
template<typename T, typename C>
struct ArrayRangeAdaptor<T, const C>
{
	typedef ArrayRangeAdaptor<T, C> NCA;
	static const bool isValid = NCA::isValid;
	static T* getData( const C& container ) { return NCA::getData( const_cast<C&>( container ) ); }
	static size_t getSize( const C& container ) { return NCA::getSize( const_cast<C&>( container ) ); }
};

// Specialization for std::vectors of values.
template<typename T, typename ET>
struct ArrayRangeAdaptor<T, std::vector<ET> >
{
	static const bool isValid = true;
	static T* getData( std::vector<ET>& v ) { return v.empty() ? NULL : &v[0]; }
	static size_t getSize( std::vector<ET>& v ) { return v.size(); }
};

// Specialization for std::vectors of pointers (allows coercions).
template<typename T, typename ET>
struct ArrayRangeAdaptor<T, std::vector<ET*> >
{
	static const bool isValid = true;
	static T* getData( std::vector<ET*>& v )
	{
		/*
			A conversion from ET* to T must not need a type cast.
			Generally, this works for single, but not for multiple inheritance.
		 */
		assert( static_cast<T>( (ET*)0xCCCC ) == (T)0xCCCC );
		return v.empty() ? NULL : reinterpret_cast<T*>( &v[0] );
	}
	static size_t getSize( std::vector<ET*>& v ) { return v.size(); }
};

#endif // DOXYGEN

#ifdef CORAL_CC_MSVC
#pragma warning (push)
#pragma warning (disable: 4521) // disable "multiple copy constructors specified"
#endif

/*!
	\brief An advanced iterator for contiguous arrays of elements.

	\tparam T any valid type for C++ arrays.
 
	The range [start, end) is represented by two pointers.
	The first and the last elements can be obtained by getFirst() and getLast(), respectively.
	A traversal can be made in either direction using popFirst() or popLast().
	It's also possible to index an array element directly using operator[].
	Testing an ArrayRange object is equivalent to testing whether it isEmpty().
 
	Here's an example of how to iterate over a range:
	\code
		std::vector<int> myArray;
		myArray.push_back( 3 );
		myArray.push_back( 5 );
		myArray.push_back( 8 );
		myArray.push_back( 13 );

		for( co::ArrayRange<int> r( myArray ); r; r.popFirst() )
		{
			print( "%i", r.getFirst() );
		}
	\endcode
 */
template<typename T>
class ArrayRange
{
public:
	//! Creates an empty array range.
	ArrayRange() : _start( 0 ), _end( 0 )
	{;}

	/*!
		Creates the range [start, end) out of two pointers.
		\param[in] start pointer to the first element in the range.
		\param[in] end pointer to the location right after the last element in the range.
	 */
	ArrayRange( T* start, T* end ) : _start( start ), _end( end )
	{
		assert( _start <= _end );
	}

	//! Creates a range starting at 'array' and containing 'size' elements.
	ArrayRange( T* array, size_t size ) : _start( array ), _end( array + size )
	{;}

	//! Non-const copy constructor.
	ArrayRange( ArrayRange& other ) : _start( other._start ), _end( other._end ) {;}

	//! Const copy constructor.
	ArrayRange( const ArrayRange& other ) : _start( other._start ), _end( other._end ) {;}

	/*!
		Creates a range spanning the entire contents of an array-like container.
		By default, the only accepted container type is std::vector, but you may
		specialize the co::ArrayRangeAdaptor struct to accept other container types.
		\tparam C a container class for which a co::ArrayRangeAdaptor was defined.
	 */
	template<typename C>
	ArrayRange( C& container )
	{
		typedef ::co::ArrayRangeAdaptor<T, C> Adaptor;
		CORAL_STATIC_CHECK( Adaptor::isValid, cannot_build_this_ArrayRange_out_of_that_type );
		_start = Adaptor::getData( container );
		_end = _start + Adaptor::getSize( container );
	}

	//! Destructor.
	~ArrayRange()
	{;}

	//! Returns true if the range is empty.
	inline bool isEmpty() const { return _start == _end; }

	//! Returns the number of elements in the range.
	inline size_t getSize() const { return _end - _start; }

	//! Testing the range is equivalent to testing if it's not empty.
	inline operator bool() const { return _start != _end; }

	//! Returns the first element in the range.
	inline T& getFirst() { return *_start; }

	//! Returns the first element in the range (const version).
	inline const T& getFirst() const { return *_start; }

	//! Returns the last element in the range.
	inline T& getLast() { return *( _end - 1 ); }

	//! Returns the last element in the range (const version).
	inline const T& getLast() const { return *( _end - 1 ); }

	//! Removes the first element from the range, if it's not empty.
	inline void popFirst()
	{
		if( _start < _end )
			++_start;
	}

	//! Removes the last element from the range, if it's not empty.
	inline void popLast()
	{
		if( _start < _end )
			--_end;
	}

	/*!
		Assigns the range to a std::vector-compatible container.
	 */
	template<typename C>
	inline void assignTo( C& container ) const
	{
		size_t size = getSize();
		if( container.capacity() < size )
		{
			container.clear();
			container.reserve( size );
			for( size_t i = 0; i < size; ++i )
				container.push_back( _start[i] );
		}
		else
		{
			container.resize( size );
			for( size_t i = 0; i < size; ++i )
				container[i] = _start[i];
		}
	}

	//! Unchecked random access to range elements.
	inline T& operator[]( int index ) { return *( _start + index ); }

	//! Unchecked random access to range elements (const version).
	inline const T& operator[]( int index ) const { return *( _start + index ); }

private:
	T* _start;
	T* _end;
};

#ifdef CORAL_CC_MSVC
#pragma warning (pop)
#endif

#ifndef DOXYGEN

/****************************************************************************/
/* All type-traits definitions related to co::ArrayRange are located below  */
/****************************************************************************/

template<typename T>
struct kindOf<ArrayRange<T> > : public kindOfBase<TK_ARRAY> {};

template<typename T>
struct nameOf<ArrayRange<T> > : public nameOfArrayBase<T> {};

template<typename T>
struct typeOf<ArrayRange<T> > : public typeOfArrayBase<T> {};

#endif // DOXYGEN

} // namespace co

#endif // _CO_ARRAYRANGE_H_
