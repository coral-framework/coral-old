/*
 * Coral - A C++ Component Framework.
 * See Copyright Notice in Coral.h
 */

#ifndef _CO_REFVECTOR_H_
#define _CO_REFVECTOR_H_

#include <co/RefPtr.h>
#include <co/ArrayRange.h>

namespace co {

/*!
	\brief An enhanced std::vector of co::RefPtr's.
 
	\tparam T an interface type (co::Interface).
 
	Supports automatic coercion to co::ArrayRange< T* >.
	Also offers extra methods for sorted arrays: sortedInsert() and sortedFind().
 */
template<class T>
class RefVector : public std::vector< co::RefPtr<T> >
{
public:
	//! Default constructor.
	inline RefVector() : std::vector< co::RefPtr<T> >() {;}

	//! Creates a vector with \a n null pointers.
	inline RefVector( size_t n ) : std::vector< co::RefPtr<T> >( n ) {;}

	/*!
		Performs a binary search on the sorted range [first,last] using a \c Comparator function.

		Returns \c true if a matching element is found, or \c false if the key is not in the
		sorted range. In either case, the parameter \a pos is set to the index where the element
		should be located in the sorted range.

		\note The \c Comparator function is defined as follows:
			\code int Comparator( const T* element, const Key& key ) \endcode
			- returns \b zero when <tt>element == key</tt>
			- returns \b <tt><0</tt> when <tt>element < key</tt>
			- returns \b <tt>>0</tt> when <tt>element > key</tt>

		\warning Assumes the specified range is already sorted according to the \c Comparator function.
		\sa sortedInsert()
	 */
	template<class Key, class Comparator>
	inline bool sortedFind( const Key& key, Comparator compare, std::size_t first, std::size_t last, std::size_t& pos )
	{
		assert( last < this->size() );
		while( first <= last )
		{
			pos = ( first + last ) / 2;
			int comp = compare( (*this)[pos].get(), key );
			if( comp < 0 )
				first = pos + 1;	// repeat search in right half
			else if( comp > 0 )
			{
				if( pos == 0 )
					break;
				last = pos - 1;		// repeat search in left half
			}
			else
				return true;
		}
		pos = first;
		return false;
	}

	/*!
		\overload
		This overload operates on the whole co::RefVector, in the range [0, n - 1].
	 */
	template<class Key, class Comparator>
	inline bool sortedFind( const Key& key, Comparator compare, std::size_t& pos )
	{
		if( this->empty() )
		{
			pos = 0;
			return false;
		}
		return sortedFind( key, compare, 0, this->size() - 1, pos );
	}

	/*!
		Inserts a new element in a sorted vector of interfaces. Intended for use in combination
		with sortedFind(). See sortedFind() for a description of the \c Comparator function.
		\return \c true on success; or \c false if an existing element with the same key was found.
		\sa sortedFind()
		\note This method is supposed to be slightly more efficient than using std::lower_bound
				and insert(), mainly because it uses co::RefPtr<T>::swap().
	 */
	template<typename Key, typename Comparator>
	inline bool sortedInsert( const Key& key, T* element, Comparator compare )
	{
		std::size_t pos;
		if( sortedFind( key, compare, pos ) )
			return false;

		// insert the new element at the back
		this->push_back( element );

		// slide the new element to the correct position
		for( std::size_t i = this->size() - 1; i > pos; --i )
			(*this)[i - 1].swap( (*this)[i] );

		return true;
	}

	//! \overload
	template<typename Key, typename Comparator>
	inline bool sortedInsert( const Key& key, co::RefPtr<T>& element, Comparator compare )
	{
		return sortedInsert<Key, Comparator>( key, element.get(), compare );
	}
};

#ifndef DOXYGEN

// ArrayRangeAdaptor to extract an interface-pointer array out of a RefVector.
template<typename T, typename ET>
struct ArrayRangeAdaptor<T, RefVector<ET> >
{
	static const bool isValid = true;
	static T* getData( RefVector<ET>& v )
	{
		// ET must be castable to T
		static const T s_castabilityTest = (ET*)0;
		CORAL_UNUSED( s_castabilityTest );
		return v.empty() ? NULL : reinterpret_cast<T*>( &v[0] );
	}
	static std::size_t getSize( RefVector<ET>& v ) { return v.size(); }
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

#endif
