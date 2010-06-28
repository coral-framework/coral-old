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
	A std::vector of reference pointers to Coral interfaces of type T.
	Offers two extra methods, to maintain sorted arrays: sortedFind() and sortedInsert().
 */
template<class T>
class RefVector : public std::vector< co::RefPtr<T> >
{
public:
	/*!
		Performs a binary search on the sorted range [first,last] using a \c Comparator function.

		Returns \c true if a matching element is found, or \c false if the key is not in the
		sorted range. In either case, the parameter \c pos is set to the index where the element
		should be located in the sorted range.

		\note The \c Comparator function is defined as follows:
			\code int Comparator( const T* element, const Key& key ) \endcode
			- returns \b zero when <tt>element == key</tt>
			- returns \b <tt><0</tt> when <tt>element < key</tt>
			- returns \b <tt>>0</tt> when <tt>element > key</tt>

		\warning Assumes the specified range is already sorted according to the \c Comparator function.
		\sa sortedInsert()
	 */
	//@{
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
	//@}

	/*!
		Inserts a new element in a sorted vector of interfaces.
		Intended to be used in combination with sortedFind().
		See sortedFind() for a description of the \c Comparator function.
		\return \c true on success; or \c false if an existing element with the same key was found.
		\sa sortedFind()
		\note This method is probably more efficient than using std::lower_bound and insert(), because
			it does fewer calls to the comparator and uses co::RefPtr::swap() instead of assignments.
	 */
	//@{
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

	template<typename Key, typename Comparator>
	inline bool sortedInsert( const Key& key, co::RefPtr<T>& value, Comparator compare )
	{
		return sortedInsert<Key, Comparator>( key, value.get(), compare );
	}
	//@}}
};

//! co::ArrayRangeAdaptor to extract an interface-pointer array out of a co::RefVector.
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

} // namespace co

#endif
