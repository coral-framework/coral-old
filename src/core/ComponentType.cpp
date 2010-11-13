/*
 * Coral - Lightweight C++ Component Framework.
 * See Copyright Notice in Coral.h
 */

#include "ComponentType.h"
#include "MemberInfo.h"
#include <algorithm>

// Helper comparators:

int itfInfoComparator( co::InterfaceInfo* interfaceInfo, const std::string& name )
{
	return interfaceInfo->getName().compare( name );
}

bool sortInterfacesByKindThenName( const co::RefPtr<co::InterfaceInfo>& a, const co::RefPtr<co::InterfaceInfo>& b )
{
	bool isFacetA = const_cast<co::InterfaceInfo*>( a.get() )->getIsFacet();
	bool isFacetB = const_cast<co::InterfaceInfo*>( b.get() )->getIsFacet();

	if( ( !isFacetA && isFacetB ) || ( isFacetA && !isFacetB ) )
	{
		return isFacetA; // facets come first
	}

	return const_cast<co::InterfaceInfo*>( a.get() )->getName() < const_cast<co::InterfaceInfo*>( b.get() )->getName();
}

// ------ ComponentType -------------------------------------------------------

ComponentType::ComponentType() : _firstReceptacle( -1 )
{
	// empty
}

ComponentType::~ComponentType()
{
	// empty
}

void ComponentType::addInterfaces( co::ArrayRange<co::InterfaceInfo* const> interfaces )
{
	_interfaces.reserve( _interfaces.size() + interfaces.getSize() );
	for( ; interfaces; interfaces.popFirst() )
		_interfaces.push_back( interfaces.getFirst() );
}

void ComponentType::sortInterfaces()
{
	std::sort( _interfaces.begin(), _interfaces.end(), sortInterfacesByKindThenName );

	// notify members about their owner/index
	std::size_t count = _interfaces.size();
	for( std::size_t i = 0; i < count; ++i )
	{
		MemberInfoImpl* mii = dynamic_cast<MemberInfoImpl*>( _interfaces[i].get() );
		assert( mii );
		mii->setOwner( this, i );

		if( _firstReceptacle == std::size_t( -1 ) && !_interfaces[i]->getIsFacet() )
			_firstReceptacle = i;
	}

	if( _firstReceptacle == std::size_t( -1 ) )
		_firstReceptacle = count;
}

co::ArrayRange<co::MemberInfo* const> ComponentType::getMembers()
{
	return _interfaces;
}

co::MemberInfo* ComponentType::getMember( const std::string& name )
{
	std::size_t pos;

	// first, look for a facet
	if( _firstReceptacle > 0 && _interfaces.sortedFind( name, itfInfoComparator, 0, _firstReceptacle - 1, pos ) )
		return _interfaces[pos].get();

	// then look for a receptacle
	std::size_t numItfs = _interfaces.size();
	if( _firstReceptacle < numItfs && _interfaces.sortedFind( name, itfInfoComparator, _firstReceptacle, numItfs - 1, pos ) )
		return _interfaces[pos].get();

	return NULL;
}

co::ArrayRange<co::InterfaceInfo* const> ComponentType::getInterfaces()
{
	return _interfaces;
}

co::ArrayRange<co::InterfaceInfo* const> ComponentType::getFacets()
{
	assert( _firstReceptacle != std::size_t( -1 ) );

	if( _firstReceptacle < 1 )
		return co::ArrayRange<co::InterfaceInfo* const>();

	return co::ArrayRange<co::InterfaceInfo* const>( reinterpret_cast<co::InterfaceInfo**>(
				&_interfaces.front() ), _firstReceptacle );
}

co::ArrayRange<co::InterfaceInfo* const> ComponentType::getReceptacles()
{
	assert( _firstReceptacle != std::size_t( -1 ) );
	
	std::size_t numItfs = _interfaces.size();
	if( _firstReceptacle >= numItfs )
		return co::ArrayRange<co::InterfaceInfo* const>();
	
	return co::ArrayRange<co::InterfaceInfo* const>( reinterpret_cast<co::InterfaceInfo**>(
				&_interfaces.front() + _firstReceptacle ), numItfs - _firstReceptacle );
}

CORAL_EXPORT_COMPONENT( ComponentType, ComponentTypeComponent );
