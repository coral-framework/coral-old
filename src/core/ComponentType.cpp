/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#include "ComponentType.h"
#include "MemberInfo.h"
#include <algorithm>

namespace co {

// ------ Helper Comparators ---------------------------------------------------

inline int itfInfoComparator( InterfaceInfo* interfaceInfo, const std::string& name )
{
	return interfaceInfo->getName().compare( name );
}

inline bool sortInterfacesByKindThenName( const RefPtr<InterfaceInfo>& a, const RefPtr<InterfaceInfo>& b )
{
	bool isFacetA = const_cast<InterfaceInfo*>( a.get() )->getIsFacet();
	bool isFacetB = const_cast<InterfaceInfo*>( b.get() )->getIsFacet();

	if( ( !isFacetA && isFacetB ) || ( isFacetA && !isFacetB ) )
	{
		return isFacetA; // facets come first
	}

	return const_cast<InterfaceInfo*>( a.get() )->getName()
			< const_cast<InterfaceInfo*>( b.get() )->getName();
}

// ------ ComponentTypeComponent -----------------------------------------------

ComponentTypeComponent::ComponentTypeComponent() : _firstReceptacle( -1 )
{
	// empty
}

ComponentTypeComponent::~ComponentTypeComponent()
{
	// empty
}

void ComponentTypeComponent::addInterfaces( ArrayRange<InterfaceInfo* const> interfaces )
{
	_interfaces.reserve( _interfaces.size() + interfaces.getSize() );
	for( ; interfaces; interfaces.popFirst() )
		_interfaces.push_back( interfaces.getFirst() );
}

void ComponentTypeComponent::sortInterfaces()
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

ArrayRange<MemberInfo* const> ComponentTypeComponent::getMembers()
{
	return _interfaces;
}

MemberInfo* ComponentTypeComponent::getMember( const std::string& name )
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

ArrayRange<InterfaceInfo* const> ComponentTypeComponent::getInterfaces()
{
	return _interfaces;
}

ArrayRange<InterfaceInfo* const> ComponentTypeComponent::getFacets()
{
	assert( _firstReceptacle != size_t( -1 ) );

	if( _firstReceptacle < 1 )
		return ArrayRange<InterfaceInfo* const>();

	return ArrayRange<InterfaceInfo* const>( reinterpret_cast<InterfaceInfo**>(
				&_interfaces.front() ), _firstReceptacle );
}

ArrayRange<InterfaceInfo* const> ComponentTypeComponent::getReceptacles()
{
	assert( _firstReceptacle != size_t( -1 ) );
	
	size_t numItfs = _interfaces.size();
	if( _firstReceptacle >= numItfs )
		return ArrayRange<InterfaceInfo* const>();
	
	return ArrayRange<InterfaceInfo* const>( reinterpret_cast<InterfaceInfo**>(
				&_interfaces.front() + _firstReceptacle ), numItfs - _firstReceptacle );
}

CORAL_EXPORT_COMPONENT( ComponentTypeComponent, ComponentTypeComponent );

} // namespace co
