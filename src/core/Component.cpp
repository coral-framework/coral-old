/*
 * Coral - Lightweight C++ Component Framework
 * See Copyright Notice in Coral.h
 */

#include "Component.h"
#include "Port.h"

namespace co {

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

void Component::sortPorts()
{
	std::sort( _ports.begin(), _ports.end(), sortPortsByKindThenName );

	// notify members about their owner/index
	size_t count = _ports.size();
	for( size_t i = 0; i < count; ++i )
	{
		Port* port = static_cast<Port*>( _ports[i].get() );
		port->setOwner( this, i );

		if( _firstReceptacle == size_t( -1 ) && !port->getIsFacet() )
			_firstReceptacle = i;
	}

	if( _firstReceptacle == size_t( -1 ) )
		_firstReceptacle = count;
}

Range<IMember* const> Component::getMembers()
{
	return _ports;
}

inline int nameComparator( const std::string& name, IPort* port )
{
	return name.compare( port->getName() );
}

IMember* Component::getMember( const std::string& name )
{
	size_t pos;

	// first, look for a facet
	if( _firstReceptacle > 0 )
	{
		Range<IPort*> range( _ports.getRange( 0, _firstReceptacle ) );
		if( binarySearch( range, name, nameComparator, pos ) )
			return range[pos];
	}

	// then look for a receptacle
	size_t size = _ports.size();
	if( _firstReceptacle < size )
	{
		Range<IPort*> range( _ports.getRange( _firstReceptacle, size - _firstReceptacle ) );
		if( binarySearch( range, name, nameComparator, pos ) )
			return range[pos];
	}

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
