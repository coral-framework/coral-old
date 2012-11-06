/*
 * Coral - Lightweight C++ Component Framework
 * See Copyright Notice in Coral.h
 */

#ifndef _CO_SLICE_H_
#define _CO_SLICE_H_

#include <co/TypeTraits.h>

namespace co {

#ifndef DOXYGEN

// Instructs Coral how to take a Slice<T> out of a container C.
template<typename T, typename C>
struct Slicer
{
	static const bool isValid = false;
	static const T* getData( const C& ) { return 0; }
	static size_t getSize( const C& ) { return 0; }
};

// Slicer for statically-allocated arrays.
template<typename T, typename E, size_t size>
struct Slicer<T, E[size]>
{
	static const bool isValid = true;
	static const T* getData( const E array[size] ) { return array; }
	static size_t getSize( const E[size] ) { return size; }
};

// Slicer for std::vectors of values.
template<typename T, typename ET>
struct Slicer<T, std::vector<ET> >
{
	static const bool isValid = true;
	static const T* getData( const std::vector<ET>& v ) { return v.empty() ? NULL : &v[0]; }
	static size_t getSize( const std::vector<ET>& v ) { return v.size(); }
};

// Slicer for std::vectors of raw pointers (allows upcasts).
template<typename T, typename ET>
struct Slicer<T, std::vector<ET*> >
{
	static const bool isValid = true;
	static const T* getData( const std::vector<ET*>& v )
	{
		// ET must be a subtype of T.
		static_assert( std::is_base_of<typename std::remove_pointer<T>::type, ET>::value, "incompatible pointer types" );
		return v.empty() ? NULL : static_cast<const T*>( &v[0] );
	}
	static size_t getSize( const std::vector<ET*>& v ) { return v.size(); }
};

// Slicer for std::vectors of RefPtrs (allows upcasts).
template<typename T, typename ET>
struct Slicer<T, std::vector<co::RefPtr<ET> > >
{
	static const bool isValid = true;
	static const T* getData( const std::vector<co::RefPtr<ET> >& v )
	{
		static_assert( std::is_base_of<typename std::remove_pointer<T>::type, ET>::value, "incompatible pointer types" );
		return v.empty() ? NULL : reinterpret_cast<const T*>( &v[0] );
	}
	static size_t getSize( const std::vector<co::RefPtr<ET> >& v ) { return v.size(); }
};

template<typename T> class TSlice;

#endif // DOXYGEN

/*!
	\brief A const iterator for one-dimensional arrays.
	\tparam T any valid Coral element type.

	The array slice [begin, end) is represented by two pointers.
	The first and the last elements can be obtained by getFirst() and getLast(), respectively.
	A traversal can be made in either direction using popFirst() or popLast().
	It's also possible to index an array element directly using operator[].
	Testing a slice is equivalent to testing whether it isEmpty().

	Here's an example of how to iterate a slice:
	\code
		std::vector<int> vec;
		vec.push_back( 3 );
		vec.push_back( 5 );
		vec.push_back( 8 );
		vec.push_back( 13 );

		for( co::Slice<int> s( vec ); s; s.popFirst() )
		{
			print( "%i", s.getFirst() );
		}
	\endcode
 */
template<typename T>
class Slice
{
public:
	//! Creates a default empty array slice.
	inline Slice() : _begin( 0 ), _end( 0 ) {;}

	/*!
		Creates the slice [begin, end) out of two pointers.
		\param[in] begin pointer to the first element in the slice.
		\param[in] end the address right after the last element in the slice.
	 */
	inline Slice( const T* begin, const T* end )
		: _begin( begin ), _end( end ) { assert( _begin <= _end ); }

	//! Creates a slice from an \a array containing \a size elements.
	inline Slice( const T* array, size_t size )
		: _begin( array ), _end( array + size ) {;}

	//! Creates a slice from a TSlice of the same type.
	inline Slice( const TSlice<T>& t )
		: _begin( t._begin ), _end( t._end ) {;}

	/*!
		Creates a slice spanning the entire contents of a compatible \a container.
		By default supports std::vector's and statically-allocated arrays.
		Specialize the co::Slicer struct to accept other types.
		\tparam C a container class for which a co::Slicer was defined.
	 */
	template<typename C>
	inline Slice( const C& container )
	{
		typedef Slicer<T, C> Slicer;
		static_assert( Slicer::isValid, "invalid container type for co::Slice" );
		_begin = Slicer::getData( container );
		_end = _begin + Slicer::getSize( container );
	}

	//! Non-const copy constructor.
	inline Slice( Slice& other )
		: _begin( other._begin ), _end( other._end ) {;}

	//! Const copy constructor.
	inline Slice( const Slice& other )
		: _begin( other._begin ), _end( other._end ) {;}

	//! Destructor.
	inline ~Slice() {;}

	//! Returns a pointer to the start of the array.
	inline const T* begin() const { return _begin; }

	//! Returns the address right after the last element in the array.
	inline const T* end() const { return _end; }

	//! Returns true if the slice is empty.
	inline bool isEmpty() const { return _begin == _end; }

	//! Returns the number of elements in the slice.
	inline size_t getSize() const { return _end - _begin; }

	//! Testing the slice is equivalent to testing whether it is nonempty.
	inline operator bool() const { return _begin != _end; }

	//! Returns the first element in the slice.
	inline const T& getFirst() const { return *_begin; }

	//! Returns the last element in the slice.
	inline const T& getLast() const { return *( _end - 1 ); }

	//! Removes the first element from the slice, if it is nonempty.
	inline void popFirst() { assert( !isEmpty() ); ++_begin; }

	//! Removes the last element from the slice, if it is nonempty.
	inline void popLast() { assert( !isEmpty() ); --_end; }

	//! Unchecked random access to slice elements.
	inline const T& operator[]( size_t index ) const { return _begin[index]; }

protected:
	const T* _begin;
	const T* _end;
};

//! Interface for temporary objects that can be passed in a TSlice.
struct ITemporary
{
	virtual ~ITemporary() {;}
};

//! Wrapper for instantiating temporary objects. \sa ITemporary
template<typename T>
struct Temporary : public ITemporary
{
	T value;
};

//! Ensures derived classes cannot be copied.
class NonCopyable
{
protected:
	NonCopyable() {;}
	~NonCopyable() {;}

private:
	NonCopyable( const NonCopyable& );
	NonCopyable& operator=( const NonCopyable& );
};

/*!
	\brief A Slice that can hold a temporary object.
	\tparam T any valid Coral element type.
 */
template<typename T>
class TSlice : public Slice<T>, public NonCopyable
{
public:
	//typedef typename ArrayOf<T, kindOf<T>::kind>::VectorType VectorType;

public:
	//! Creates a default empty array slice.
	inline TSlice() : _temp( nullptr ) {;}

	/*!
		Creates the slice [begin, end) out of two pointers.
		\param[in] begin pointer to the first element in the slice.
		\param[in] end the address right after the last element in the slice.
	 */
	inline TSlice( const T* begin, const T* end )
		: Slice<T>( begin, end ), _temp( nullptr ) {;}

	//! Creates a slice from an \a array containing \a size elements.
	inline TSlice( const T* array, size_t size )
		: Slice<T>( array, size ), _temp( nullptr ) {;}

	/*!
		Creates a slice spanning the entire contents of a \a container.
		By default supports std::vector and statically-allocated arrays.
		Specialize co::Slicer to add support for other container types.
		\tparam C a container type supported by co::Slicer.
	 */
	//@{
	template<typename C>
	inline TSlice( C& container )
		: Slice<T>( container ), _temp( nullptr ) {;}

	//! Overload that implicitly takes ownership of a temporary container.
	template<typename C>
	inline TSlice( C&& container )
		: Slice<T>( container ), _temp( nullptr )
	{
		co::Temporary<C>* temp = new Temporary<C>;
		temp->value = std::move( container );
		_temp = temp;
	}

	//! Overload that explicitly takes ownership of a temporary object.
	template<typename C>
	inline TSlice( C& container, ITemporary* temp )
		: Slice<T>( container ), _temp( temp ) {;}
	//@}

	//! Non-const copy constructor.
	inline TSlice( TSlice& ) { /* Error - NonCopyable */ }

	//! Const copy constructor.
	inline TSlice( const TSlice& ) { /* Error - NonCopyable */ }

	//! Move constructor.
	inline TSlice( TSlice&& other )
		: Slice<T>( other._begin, other._end ), _temp( other._temp )
	{
		other._temp = nullptr;
	}

	//! Destructor.
	inline ~TSlice() { if( _temp ) delete _temp; }

	//! Returns our temporary object, if any.
	inline ITemporary* getTemporary() { return _temp; }

private:
	ITemporary* _temp;
};


/******************************************************************************/
/* Simple template library for co::Slice                                      */
/******************************************************************************/

/*!
	Assigns the contents of a \a slice to a std::vector-compatible \a container.
 */
template<typename T, typename C>
void assign( Slice<T> slice, C& container )
{
	size_t size = slice.getSize();
	container.resize( size );
	for( size_t i = 0; i < size; ++i )
		container[i] = slice[i];
}

/*!
	\brief Binary search on a sorted \a slice using a comparison function.

	Returns \c true if a matching element is found, or \c false if the key is
	not in the sorted slice. In either case, the parameter \a pos is set to the
	index where the element should be located in the sorted slice.

	The search relies on a key-based comparison function, which is more
	flexible (and often more efficient) than element-based STL comparators.

	The comparison function is defined is follows:
		\code int compare( const Key& key, const T& element ) \endcode
		- returns \b zero when <tt>key == element</tt>
		- returns \b <tt><0</tt> when <tt>key < element</tt>
		- returns \b <tt>>0</tt> when <tt>key > element</tt>
 */
template<typename T, typename Key, typename Comparator>
bool binarySearch( Slice<T> slice, const Key& key, Comparator compare, size_t& pos )
{
	pos = slice.getSize();
	if( pos == 0 )
		return false;

	size_t first = 0, last = pos - 1;
	while( 1 )
	{
		pos = ( first + last ) / 2;
		int comp = compare( key, slice[pos] );
		if( comp > 0 )
		{
			// repeat search in right half
			first = pos + 1;
			if( pos == last ) break;
		}
		else if( comp < 0 )
		{
			// repeat search in left half
			if( pos == 0 ) break;
			last = pos - 1;
		}
		else return true;
	}
	pos = first;
	return false;
}

} // namespace co

#endif // _CO_SLICE_H_
