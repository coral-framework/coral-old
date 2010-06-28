/*
 * Coral - A C++ Component Framework.
 * See Copyright Notice in Coral.h
 */

#include "ComponentType.h"
#include "MemberInfo.h"
#include <algorithm>

// Helper comparators:

int interfaceInfoComparator( co::InterfaceInfo* interfaceInfo, const std::string& name )
{
	return interfaceInfo->getName().compare( name );
}

bool clientThenNameInterfaceInfoComparator( const co::RefPtr<co::InterfaceInfo>& a, const co::RefPtr<co::InterfaceInfo>& b )
{
	bool providedA = const_cast<co::InterfaceInfo*>( a.get() )->getIsProvided();
	bool providedB = const_cast<co::InterfaceInfo*>( b.get() )->getIsProvided();

	if( ( !providedA && providedB ) || ( providedA && !providedB ) )
	{
		return !providedA; // client interfaces come first
	}

	return const_cast<co::InterfaceInfo*>( a.get() )->getName() < const_cast<co::InterfaceInfo*>( b.get() )->getName();
}

// ------ ComponentType -------------------------------------------------------

ComponentType::ComponentType() : _firstProvidedPos( -1 )
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
	std::sort( _interfaces.begin(), _interfaces.end(), clientThenNameInterfaceInfoComparator );

	// notify members about their owner/index
	std::size_t count = _interfaces.size();
	for( std::size_t i = 0; i < count; ++i )
	{
		MemberInfoImpl* mii = dynamic_cast<MemberInfoImpl*>( _interfaces[i].get() );
		assert( mii );
		mii->setOwner( this, i );

		if( _firstProvidedPos == std::size_t( -1 ) && _interfaces[i]->getIsProvided() )
			_firstProvidedPos = i;
	}

	if( _firstProvidedPos == std::size_t( -1 ) )
		_firstProvidedPos = count;
}

co::ArrayRange<co::MemberInfo* const> ComponentType::getMembers()
{
	return _interfaces;
}

co::MemberInfo* ComponentType::getMember( const std::string& name )
{
	std::size_t pos;

	// first, look for a client interface
	if( _firstProvidedPos > 0 && _interfaces.sortedFind( name, interfaceInfoComparator, 0, _firstProvidedPos - 1, pos ) )
		return _interfaces[pos].get();
	
	// if it's not an attribute, look for a method
	std::size_t interfacesSize = _interfaces.size();
	if( _firstProvidedPos < interfacesSize && _interfaces.sortedFind( name, interfaceInfoComparator, _firstProvidedPos, interfacesSize - 1, pos ) )
		return _interfaces[pos].get();
	
	return NULL;
}

co::ArrayRange<co::InterfaceInfo* const> ComponentType::getInterfaces()
{
	return _interfaces;
}

co::ArrayRange<co::InterfaceInfo* const> ComponentType::getRequiredInterfaces()
{
	assert( _firstProvidedPos != std::size_t( -1 ) );
	
	if( _firstProvidedPos < 1 )
		return co::ArrayRange<co::InterfaceInfo* const>();

	return co::ArrayRange<co::InterfaceInfo* const>( reinterpret_cast<co::InterfaceInfo**>( &_interfaces.front() ), _firstProvidedPos );
}

co::ArrayRange<co::InterfaceInfo* const> ComponentType::getProvidedInterfaces()
{
	assert( _firstProvidedPos != std::size_t( -1 ) );

	std::size_t interfacesSize = _interfaces.size();
	if( _firstProvidedPos >= interfacesSize )
		return co::ArrayRange<co::InterfaceInfo* const>();

	return co::ArrayRange<co::InterfaceInfo* const>( reinterpret_cast<co::InterfaceInfo**>( &_interfaces.front() + _firstProvidedPos ), interfacesSize - _firstProvidedPos );
}

CORAL_EXPORT_COMPONENT( ComponentType, ComponentTypeComponent );
