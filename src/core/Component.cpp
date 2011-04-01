/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#include "Component.h"
#include "Member.h"
#include <algorithm>

namespace co {

// ------ Helper Comparators ---------------------------------------------------

inline int portComparator( IPort* port, const std::string& name )
{
	return port->getName().compare( name );
}

inline bool sortPortsByKindThenName( const RefPtr<IPort>& a, const RefPtr<IPort>& b )
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

void Component::addPorts( Range<IPort* const> ports )
{
	_ports.reserve( _ports.size() + ports.getSize() );
	for( ; ports; ports.popFirst() )
		_ports.push_back( ports.getFirst() );
}

void Component::sortPorts()
{
	std::sort( _ports.begin(), _ports.end(), sortPortsByKindThenName );

	// notify members about their owner/index
	size_t count = _ports.size();
	for( size_t i = 0; i < count; ++i )
	{
		MemberImpl* mii = dynamic_cast<MemberImpl*>( _ports[i].get() );
		assert( mii );
		mii->setOwner( this, i );

		if( _firstReceptacle == size_t( -1 ) && !_ports[i]->getIsFacet() )
			_firstReceptacle = i;
	}

	if( _firstReceptacle == size_t( -1 ) )
		_firstReceptacle = count;
}

Range<IMember* const> Component::getMembers()
{
	return _ports;
}

IMember* Component::getMember( const std::string& name )
{
	size_t pos;

	// first, look for a facet
	if( _firstReceptacle > 0 && _ports.sortedFind( name, portComparator, 0, _firstReceptacle - 1, pos ) )
		return _ports[pos].get();

	// then look for a receptacle
	size_t numItfs = _ports.size();
	if( _firstReceptacle < numItfs && _ports.sortedFind( name, portComparator, _firstReceptacle, numItfs - 1, pos ) )
		return _ports[pos].get();

	return NULL;
}

Range<IPort* const> Component::getPorts()
{
	return _ports;
}

Range<IPort* const> Component::getFacets()
{
	assert( _firstReceptacle != size_t( -1 ) );

	if( _firstReceptacle < 1 )
		return Range<IPort* const>();

	return Range<IPort* const>( reinterpret_cast<IPort**>(
				&_ports.front() ), _firstReceptacle );
}

Range<IPort* const> Component::getReceptacles()
{
	assert( _firstReceptacle != size_t( -1 ) );

	size_t numItfs = _ports.size();
	if( _firstReceptacle >= numItfs )
		return Range<IPort* const>();

	return Range<IPort* const>( reinterpret_cast<IPort**>(
				&_ports.front() + _firstReceptacle ), numItfs - _firstReceptacle );
}

CORAL_EXPORT_COMPONENT( Component, Component );

} // namespace co
