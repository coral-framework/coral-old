/*
 * Coral - Lightweight C++ Component Framework
 * See Copyright Notice in Coral.h
 */

#ifndef _CO_RANGE_H_
#define _CO_RANGE_H_

#include <co/Platform.h>
#include <co/TypeTraits.h>
#include <vector>

namespace co {

#ifndef DOXYGEN

// Instructs a Range how to extract an array out of a custom container.
template<typename T, typename C>
struct RangeAdaptor
{
	static const bool isValid = false;
	static T* getData( C& ) { return 0; }
	static size_t getSize( C& ) { return 0; }
};

// We should always be able to create Ranges out of const containers.
template<typename T, typename C>
struct RangeAdaptor<T, const C>
{
	typedef RangeAdaptor<T, C> NCA;
	static const bool isValid = NCA::isValid;
	static const T* getData( const C& container ) { return NCA::getData( const_cast<C&>( container ) ); }
	static size_t getSize( const C& container ) { return NCA::getSize( const_cast<C&>( container ) ); }
};

// Specialization for std::vectors of values.
template<typename T, typename ET>
struct RangeAdaptor<T, std::vector<ET> >
{
	static const bool isValid = true;
	static T* getData( std::vector<ET>& v ) { return v.empty() ? NULL : &v[0]; }
	static size_t getSize( std::vector<ET>& v ) { return v.size(); }
};

// Specialization for std::vectors of pointers (allows coercions).
template<typename T, typename ET>
struct RangeAdaptor<T, std::vector<ET*> >
{
	static const bool isValid = true;
	static T* getData( std::vector<ET*>& v )
	{
		// ET must be a subtype of T.
		static_assert( ( traits::isSubTypeOf<ET, typename traits::removePointer<T>::Type>::value ), "incompatible pointer types" );
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
	\brief A generic iterator for one-dimensional arrays.
	\tparam T any acceptable element type for a C++ array.

	The range [start, end) is represented by two pointers.
	The first and the last elements can be obtained by getFirst() and getLast(), respectively.
	A traversal can be made in either direction using popFirst() or popLast().
	It's also possible to index an array element directly using operator[].
	Testing a range is equivalent to testing whether it isEmpty().

	Here's an example of how to iterate over a range:
	\code
		std::vector<int> myArray;
		myArray.push_back( 3 );
		myArray.push_back( 5 );
		myArray.push_back( 8 );
		myArray.push_back( 13 );

		for( co::Range<int> r( myArray ); r; r.popFirst() )
		{
			print( "%i", r.getFirst() );
		}
	\endcode

	\par Reference
		The concept of \e range as an alternative to \e iterators was proposed by Andrei
		Alexandrescu in his talk <em>"Iterators Must Go"</em>. Since arrays are Coral's
		only native collection type, we provide only this very simple Range class.
 */
template<typename T>
class Range
{
public:
	//! Creates a default empty array range.
	Range() : _start( 0 ), _end( 0 )
	{;}

	/*!
		Creates the range [start, end) out of two pointers.
		\param[in] start pointer to the first element in the range.
		\param[in] end the address right after the last element in the range.
	 */
	Range( T* start, T* end ) : _start( start ), _end( end )
	{
		assert( _start <= _end );
	}

	//! Creates a range starting at \a array and containing \a size elements.
	Range( T* array, size_t size ) : _start( array ), _end( array + size )
	{;}

	//! Non-const copy constructor.
	Range( Range& other ) : _start( other._start ), _end( other._end ) {;}

	//! Const copy constructor.
	Range( const Range& other ) : _start( other._start ), _end( other._end ) {;}

	/*!
		Creates a range spanning the entire contents of an array-like container.
		By default, the only accepted container type is std::vector, but you may
		specialize the co::RangeAdaptor struct to accept other container types.
		\tparam C a container class for which a co::RangeAdaptor was defined.
	 */
	template<typename C>
	Range( C& container )
	{
		typedef RangeAdaptor<T, C> Adaptor;
		static_assert( Adaptor::isValid, "invalid container type for co::Range" );
		_start = Adaptor::getData( container );
		_end = _start + Adaptor::getSize( container );
	}

	//! Destructor.
	inline ~Range() {;}

	//! Returns a pointer to the start of the array.
	inline T* getStart() const { return _start; }

	//! Returns the address right after the last element in the array.
	inline T* getEnd() const { return _end; }

	//! Returns true if the range is empty.
	inline bool isEmpty() const { return _start == _end; }

	//! Returns the number of elements in the range.
	inline size_t getSize() const { return _end - _start; }

	//! Testing the range is equivalent to testing whether it is nonempty.
	inline operator bool() const { return _start != _end; }

	//! Returns the first element in the range.
	inline T& getFirst() { return *_start; }

	//! Returns the first element in the range (const version).
	inline const T& getFirst() const { return *_start; }

	//! Returns the last element in the range.
	inline T& getLast() { return *( _end - 1 ); }

	//! Returns the last element in the range (const version).
	inline const T& getLast() const { return *( _end - 1 ); }

	//! Removes the first element from the range, if it is nonempty.
	inline void popFirst()
	{
		if( _start < _end )
			++_start;
	}

	//! Removes the last element from the range, if it is nonempty.
	inline void popLast()
	{
		if( _start < _end )
			--_end;
	}

	//! Unchecked random access to range elements.
	inline T& operator[]( size_t index ) { return *( _start + index ); }

	//! Unchecked random access to range elements (const version).
	inline const T& operator[]( size_t index ) const { return *( _start + index ); }

private:
	T* _start;
	T* _end;
};

#ifdef CORAL_CC_MSVC
#pragma warning (pop)
#endif

/******************************************************************************/
/* Simple template library based on co::Range                                 */
/******************************************************************************/

/*!
	Copies the elements in \a source into the given \a destination range.
 */
template<typename T>
void copy( Range<T> source, Range<T> destination )
{
	size_t size = source.getSize();
	assert( size <= destination.getSize() );
	for( size_t i = 0; i < size; ++i )
		destination[i] = source[i];
}

/*!
	Assigns the contents of a \a source range to a
	std::vector-compatible \a container.
 */
template<typename T, typename C>
void assign( Range<T> source, C& container )
{
	size_t size = source.getSize();
	container.resize( size );
	for( size_t i = 0; i < size; ++i )
		container[i] = source[i];
}

/*!
	\brief Performs a binary search on a sorted \a range using a comparison function.
 
	Returns \c true if a matching element is found, or \c false if the key is
	not in the sorted range. In either case, the parameter \a pos is set to the
	index where the element should be located in the sorted range.

	The search relies on a key-based comparison function, which is more
	flexible (and often more efficient) than element-based STL comparators.

	The comparison function is defined is follows:
		\code int compare( const Key& key, const T& element ) \endcode
		- returns \b zero when <tt>key == element</tt>
		- returns \b <tt><0</tt> when <tt>key < element</tt>
		- returns \b <tt>>0</tt> when <tt>key > element</tt>

	\warning The \a range must already be sorted according to the comparison function.
 */
template<typename T, typename Key, typename Comparator>
bool binarySearch( Range<T> range, const Key& key, Comparator compare, size_t& pos )
{
	pos = range.getSize();
	if( pos == 0 )
		return false;

	size_t first = 0, last = pos - 1;
	while( 1 )
	{
		pos = ( first + last ) / 2;
		int comp = compare( key, range[pos] );
		if( comp > 0 )
		{
			// repeat search in right half
			first = pos + 1;
			if( pos == last )
				break;
		}		
		else if( comp < 0 )
		{
			// repeat search in left half
			if( pos == 0 )
				break;
			last = pos - 1;
		}
		else return true;
	}
	pos = first;
	return false;
}

} // namespace co

#endif // _CO_RANGE_H_
