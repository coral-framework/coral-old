/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#include "ComponentType.h"
#include "MemberInfo.h"
#include <algorithm>

namespace co {

// ------ Helper Comparators ---------------------------------------------------

inline int itfInfoComparator( IInterfaceInfo* interfaceInfo, const std::string& name )
{
	return interfaceInfo->getName().compare( name );
}

inline bool sortInterfacesByKindThenName( const RefPtr<IInterfaceInfo>& a, const RefPtr<IInterfaceInfo>& b )
{
	bool isFacetA = const_cast<IInterfaceInfo*>( a.get() )->getIsFacet();
	bool isFacetB = const_cast<IInterfaceInfo*>( b.get() )->getIsFacet();

	if( ( !isFacetA && isFacetB ) || ( isFacetA && !isFacetB ) )
	{
		return isFacetA; // facets come first
	}

	return const_cast<IInterfaceInfo*>( a.get() )->getName()
			< const_cast<IInterfaceInfo*>( b.get() )->getName();
}

// ------ ComponentType -----------------------------------------------

ComponentType::ComponentType() : _firstReceptacle( -1 )
{
	// empty
}

ComponentType::~ComponentType()
{
	// empty
}

void ComponentType::addInterfaces( ArrayRange<IInterfaceInfo* const> interfaces )
{
	_interfaces.reserve( _interfaces.size() + interfaces.getSize() );
	for( ; interfaces; interfaces.popFirst() )
		_interfaces.push_back( interfaces.getFirst() );
}

void ComponentType::sortInterfaces()
{
	std::sort( _interfaces.begin(), _interfaces.end(), sortInterfacesByKindThenName );

	// notify members about their owner/index
	size_t count = _interfaces.size();
	for( size_t i = 0; i < count; ++i )
	{
		MemberInfoImpl* mii = dynamic_cast<MemberInfoImpl*>( _interfaces[i].get() );
		assert( mii );
		mii->setOwner( this, i );

		if( _firstReceptacle == size_t( -1 ) && !_interfaces[i]->getIsFacet() )
			_firstReceptacle = i;
	}

	if( _firstReceptacle == size_t( -1 ) )
		_firstReceptacle = count;
}

ArrayRange<IMemberInfo* const> ComponentType::getMembers()
{
	return _interfaces;
}

IMemberInfo* ComponentType::getMember( const std::string& name )
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

ArrayRange<IInterfaceInfo* const> ComponentType::getInterfaces()
{
	return _interfaces;
}

ArrayRange<IInterfaceInfo* const> ComponentType::getFacets()
{
	assert( _firstReceptacle != size_t( -1 ) );

	if( _firstReceptacle < 1 )
		return ArrayRange<IInterfaceInfo* const>();

	return ArrayRange<IInterfaceInfo* const>( reinterpret_cast<IInterfaceInfo**>(
				&_interfaces.front() ), _firstReceptacle );
}

ArrayRange<IInterfaceInfo* const> ComponentType::getReceptacles()
{
	assert( _firstReceptacle != size_t( -1 ) );

	size_t numItfs = _interfaces.size();
	if( _firstReceptacle >= numItfs )
		return ArrayRange<IInterfaceInfo* const>();

	return ArrayRange<IInterfaceInfo* const>( reinterpret_cast<IInterfaceInfo**>(
				&_interfaces.front() + _firstReceptacle ), numItfs - _firstReceptacle );
}

CORAL_EXPORT_COMPONENT( ComponentType, ComponentType );

} // namespace co
