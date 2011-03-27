/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#include "Component.h"
#include "Member.h"
#include <algorithm>

namespace co {

// ------ Helper Comparators ---------------------------------------------------

inline int itfInfoComparator( IPort* port, const std::string& name )
{
	return port->getName().compare( name );
}

inline bool sortInterfacesByKindThenName( const RefPtr<IPort>& a, const RefPtr<IPort>& b )
{
	bool isFacetA = const_cast<IPort*>( a.get() )->getIsFacet();
	bool isFacetB = const_cast<IPort*>( b.get() )->getIsFacet();

	if( ( !isFacetA && isFacetB ) || ( isFacetA && !isFacetB ) )
	{
		return isFacetA; // facets come first
	}

	return const_cast<IPort*>( a.get() )->getName()
			< const_cast<IPort*>( b.get() )->getName();
}

// ------ Component -----------------------------------------------

Component::Component() : _firstReceptacle( -1 )
{
	// empty
}

Component::~Component()
{
	// empty
}

void Component::addInterfaces( Range<IPort* const> interfaces )
{
	_interfaces.reserve( _interfaces.size() + interfaces.getSize() );
	for( ; interfaces; interfaces.popFirst() )
		_interfaces.push_back( interfaces.getFirst() );
}

void Component::sortInterfaces()
{
	std::sort( _interfaces.begin(), _interfaces.end(), sortInterfacesByKindThenName );

	// notify members about their owner/index
	size_t count = _interfaces.size();
	for( size_t i = 0; i < count; ++i )
	{
		MemberImpl* mii = dynamic_cast<MemberImpl*>( _interfaces[i].get() );
		assert( mii );
		mii->setOwner( this, i );

		if( _firstReceptacle == size_t( -1 ) && !_interfaces[i]->getIsFacet() )
			_firstReceptacle = i;
	}

	if( _firstReceptacle == size_t( -1 ) )
		_firstReceptacle = count;
}

Range<IMember* const> Component::getMembers()
{
	return _interfaces;
}

IMember* Component::getMember( const std::string& name )
{
	size_t pos;

	// first, look for a facet
	if( _firstReceptacle > 0 && _interfaces.sortedFind( name, itfInfoComparator, 0, _firstReceptacle - 1, pos ) )
		return _interfaces[pos].get();

	// then look for a receptacle
	size_t numItfs = _interfaces.size();
	if( _firstReceptacle < numItfs && _interfaces.sortedFind( name, itfInfoComparator, _firstReceptacle, numItfs - 1, pos ) )
		return _interfaces[pos].get();

	return NULL;
}

Range<IPort* const> Component::getPorts()
{
	return _interfaces;
}

Range<IPort* const> Component::getFacets()
{
	assert( _firstReceptacle != size_t( -1 ) );

	if( _firstReceptacle < 1 )
		return Range<IPort* const>();

	return Range<IPort* const>( reinterpret_cast<IPort**>(
				&_interfaces.front() ), _firstReceptacle );
}

Range<IPort* const> Component::getReceptacles()
{
	assert( _firstReceptacle != size_t( -1 ) );

	size_t numItfs = _interfaces.size();
	if( _firstReceptacle >= numItfs )
		return Range<IPort* const>();

	return Range<IPort* const>( reinterpret_cast<IPort**>(
				&_interfaces.front() + _firstReceptacle ), numItfs - _firstReceptacle );
}

CORAL_EXPORT_COMPONENT( Component, Component );

} // namespace co
