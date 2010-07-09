/*
 * Coral - A C++ Component Framework.
 * See Copyright Notice in Coral.h
 */

#include "DependencyWalker.h"
#include <co/TypeKind.h>
#include <co/ArrayType.h>
#include <co/MethodInfo.h>
#include <co/ExceptionType.h>
#include <co/InterfaceType.h>
#include <co/AttributeInfo.h>
#include <co/ParameterInfo.h>
#include <co/ComponentType.h>
#include <co/InterfaceInfo.h>

namespace co {

void DependencyWalker::walk( IDependencyVisitor* visitor, uint32 maxDistance )
{
	// walk() cannot be called twice
	assert( _queue.size() == _visited.size() );

	// a visitor must be provided
	assert( visitor );

	for( ; !_queue.empty(); _queue.pop() )
	{
		const Node& currentNode = _queue.front();

		visitor->onTypeVisited( currentNode.type, currentNode.distance );

		if( currentNode.distance < maxDistance )
			enqueueDependencies( currentNode );
	}
}

void DependencyWalker::enqueue( Type* type, uint32 distance )
{
	// if the type is an array, enqueue its element type instead
	if( type->getKind() == co::TK_ARRAY )
		type = dynamic_cast<co::ArrayType*>( type )->getElementType();

	// prune basic types before checking for them in the set
	if( type->getKind() < co::TK_ARRAY )
		return;

	// the remaining types can be checked against the dependency set
	TypeSet::iterator it = _visited.find( type );
	if( it != _visited.end() )
		return;

	// the type was not visited yet and should be added to the queue
	_visited.insert( type );
	_queue.push( Node( type, distance ) );
}

void DependencyWalker::enqueueDependencies( const Node& node )
{
	// only compound types have dependencies
	if( dynamic_cast<co::CompoundType*>( node.type ) == NULL )
		return;

	uint32 nextDistance = node.distance + 1;

	co::InterfaceType* interface = dynamic_cast<co::InterfaceType*>( node.type );
	if( interface )
	{
		co::ArrayRange<co::InterfaceType* const> superTypes = interface->getSuperInterfaces();
		for( ; superTypes; superTypes.popFirst() )
			enqueue( superTypes.getFirst(), nextDistance );
	}

	co::AttributeContainer* attribContainer = dynamic_cast<co::AttributeContainer*>( node.type );
	if( attribContainer )
	{
		co::ArrayRange<co::AttributeInfo* const> attributeList = attribContainer->getMemberAttributes();
		for ( ; attributeList; attributeList.popFirst() )
			enqueue( attributeList.getFirst()->getType(), nextDistance );
	}

	co::MethodContainer* methodContainer = dynamic_cast<co::MethodContainer*>( node.type );
	if( methodContainer )
	{
		co::ArrayRange<co::MethodInfo* const> methodList = methodContainer->getMemberMethods();
		for( ; methodList; methodList.popFirst() )
		{
			co::MethodInfo* info = methodList.getFirst();
			if( info->getReturnType() )
				enqueue( info->getReturnType(), nextDistance );

			co::ArrayRange<co::ParameterInfo* const> parameterList = info->getParameters();
			for( ; parameterList; parameterList.popFirst())
				enqueue( parameterList.getFirst()->getType(), nextDistance );

			co::ArrayRange<co::ExceptionType* const> exceptionList = info->getExceptions();
			for( ; exceptionList; exceptionList.popFirst())
				enqueue( exceptionList.getFirst(), nextDistance );
		}
	}

	co::ComponentType* componentType = dynamic_cast<co::ComponentType*>( node.type );
	if( componentType )
	{
		co::ArrayRange<co::InterfaceInfo* const> interfaceList = componentType->getInterfaces();
		for ( ; interfaceList; interfaceList.popFirst() )
			enqueue( interfaceList.getFirst()->getType(), nextDistance );
	}
}
	
} // namespace co
