/*
 * Coral - A C++ Component Framework.
 * See Copyright Notice in Coral.h
 */

#ifndef _CO_RESERVED_DEPENDENCYWALKER_H_
#define _CO_RESERVED_DEPENDENCYWALKER_H_

#include <co/Platform.h>
#include <queue>
#include <set>

namespace co {

class Type;

//! Callback interface used by the DependencyWalker class.
class IDependencyVisitor
{
public:
	//! Called for each visited type during a traversal.
	virtual void onTypeVisited( Type* type, uint32 distance ) = 0;
};

/*
	Performs a breadth-first traversal on the dependency graph of a type set.

	A DependencyWalker is created with an initial set of types, all considered to be
	at "distance zero". All types directly reachable from the initial type set (across
	a single "dependecy edge") are at distance one; those only reachable through two
	"dependency edges" are at distance two, and so forth.

	The main walk() method takes a callback interface of type IDependencyVisitor, which
	must be specified to operate on visited types during the traversal. The walk() method
	should only be called once for every DependencyWalker instance.
 */
class CORAL_EXPORT DependencyWalker
{
public:
	//! Constructor for walking the dependencies of a single type.
	DependencyWalker( Type* type ) { enqueue( type, 0 ); }

	/*!
		Constructor that receives the type set through a pair of STL-style iterators.
		The iterator must dereference a co::Type pointer.
	 */
	template<typename Iterator>
	DependencyWalker( Iterator begin, Iterator end )
	{
		while( begin != end )
			enqueue( *begin++, 0 );
	}

	/*!
		Traverses the dependency graph of the provided type set, calling \c visitor's methods as it goes.
		The traversal starts at distance zero, then proceeds in increasing distance order.	
		If \c maxDistance is specified, the traversal excludes types at distances greater than \c maxDistance.
		\warning This method should only be called once during the lifetime of a DependencyWalker.
	 */
	void walk( IDependencyVisitor* visitor, uint32 maxDistance = MAX_UINT32 );

private:
	/*!
		Adds the passed \c type to the list of types to be traversed.
		If the passed type is a co::ArrayType the enqued type is the ElementType of the array.
		The type is ignored if:
			-The type is not a user definable type (e.g. is a basic type).
			-The type was already enqued.
	 */
	void enqueue( Type* type, uint32 distance );

	//! Represents a node in the dependencies graph.
	struct Node
	{
		Type* type;			// the type
		uint32 distance;	// its distance to the initial set of types

		Node( Type* type, uint32 dist ) : type( type ), distance( dist ) {;}
	};

	/*!
		Adds the direct dependencies of the passed \c node to the queue.
		If the passed type is not a co::CompoundType it has no dependencies and it is ignored.
		If the type is an interface i  t adds the superTypes.
		If it has an Attribute, its type is enqueued,
			and for Methods the return type and the parameters types are added.
		If it is an Component all the interfaces are added to the queue.
	 */
	void enqueueDependencies( const Node& node );

private:
	typedef std::set<Type*> TypeSet;
	TypeSet _visited;			// keeps track of visited types
	std::queue<Node> _queue;	// nodes to be visited
};

} // namespace co

#endif // _CO_RESERVED_DEPENDENCYWALKER_H_
