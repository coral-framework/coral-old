/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#ifndef _CO_REFVECTOR_H_
#define _CO_REFVECTOR_H_

#include <co/RefPtr.h>
#include <co/Range.h>

namespace co {

/*!
	\brief An enhanced std::vector of co::RefPtr's.
	\tparam T a service type (co::IService).

	Supports automatic coercion to co::Range< T* >.
	Also offers extra methods for sorted vectors.
 */
template<class T>
class RefVector : public std::vector< co::RefPtr<T> >
{
public:
	//! Default constructor.
	inline RefVector() : std::vector< co::RefPtr<T> >() {;}

	//! Creates a vector with \a n null references.
	inline RefVector( size_t n ) : std::vector< co::RefPtr<T> >( n ) {;}

	//! Gets a sub-range of this vector.
	inline Range<T*> getRange( size_t first, size_t size )
	{
		return Range<T*>( size == 0 ? NULL : reinterpret_cast<T**>( &( (*this)[0] ) ) + first, size );
	}

	/*!
		Inserts an element in a sorted ref-vector.
		See binarySearch() for a description of the comparison function.
		\return false if and only if there's an existing element with the same key.
	 */
	template<typename Key, typename Comparator>
	bool sortedInsert( const Key& key, T* element, Comparator compare )
	{
		size_t pos, size = this->size();
		if( binarySearch( getRange( 0, size ), key, compare, pos ) )
			return false;

		// insert the new element at the back
		this->push_back( element );

		// slide the new element to the correct position
		for( size_t i = size; i > pos; --i )
			(*this)[i - 1].swap( (*this)[i] );

		return true;
	}

	//! \overload
	template<typename Comparator>
	bool sortedInsert( T* element, Comparator compare )
	{
		return sortedInsert( element, element, compare );
	}

	/*!
		Removes an element from a sorted ref-vector.
		See binarySearch() for a description of the comparison function.
		\return true if an element with the given \a key was removed;
				false is none was found.
	 */
	template<typename Key, typename Comparator>
	bool sortedRemove( const Key& key, Comparator compare )
	{
		size_t pos, size = this->size();
		if( !binarySearch( getRange( 0, size ), key, compare, pos ) )
			return false;

		// slide the removed element to the end
		while( ++pos < size )
			(*this)[pos - 1].swap( (*this)[pos] );

		// pop the removed element
		this->pop_back();

		return true;
	}

	/*!
		Performs a binary search on a sorted ref-vec.
		Returns the element position, or <tt>std::string::npos</tt> if no element is found.
	 */
	template<typename Key, typename Comparator>
	size_t sortedFind( const Key& key, Comparator compare )
	{
		size_t pos;
		return binarySearch( getRange( 0, this->size() ), key, compare, pos )
			? pos : std::string::npos;
	}
};

#ifndef DOXYGEN

// RangeAdaptor to extract an interface-pointer array out of a RefVector.
template<typename T, typename ET>
struct RangeAdaptor<T, RefVector<ET> >
{
	static const bool isValid = true;
	static T* getData( RefVector<ET>& v )
	{
		/*
			A conversion from ET* to T must not need a type cast.
			Generally, this works for single, but not for multiple inheritance.
		 */
		assert( static_cast<T>( (ET*)0xCCCC ) == (T)0xCCCC );
		return v.empty() ? NULL : reinterpret_cast<T*>( &v[0] );
	}
	static size_t getSize( RefVector<ET>& v ) { return v.size(); }
};

/****************************************************************************/
/* All type-traits definitions related to co::RefVector are located below   */
/****************************************************************************/

template<typename T>
struct kindOf<RefVector<T> > : public kindOfBase<TK_ARRAY> {};

template<typename T>
struct nameOf<RefVector<T> > : public nameOfArrayBase<T> {};

template<typename T>
struct typeOf<RefVector<T> > : public typeOfArrayBase<T> {};

#endif // DOXYGEN

} // namespace co

#endif // _CO_REFVECTOR_H_
