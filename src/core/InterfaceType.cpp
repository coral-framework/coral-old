/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#include "InterfaceType.h"
#include "TypeManager.h"
#include <co/Coral.h>
#include <co/System.h>
#include <algorithm>

namespace co {

InterfaceTypeComponent::~InterfaceTypeComponent()
{
	// empty
}

void InterfaceTypeComponent::addSuperInterface( InterfaceType* superItf )
{
	_superInterfaces.push_back( superItf );
}

void InterfaceTypeComponent::addSubInterface( InterfaceType* subItf )
{
	_subInterfaces.push_back( subItf );
}

ArrayRange<InterfaceType* const> InterfaceTypeComponent::getInterfaceAncestors()
{
	updateAncestors();
	return _ancestors;
}

ArrayRange<InterfaceType* const> InterfaceTypeComponent::getSuperInterfaces()
{
	return _superInterfaces;
}

ArrayRange<InterfaceType* const> InterfaceTypeComponent::getSubInterfaces()
{
	return _subInterfaces;
}

const std::string& InterfaceTypeComponent::getCppBlock()
{
	TypeManagerComponent* tm = dynamic_cast<TypeManagerComponent*>( getSystem()->getTypes() );
	assert( tm );
	return tm->getCppBlock( getFullName() );
}

bool InterfaceTypeComponent::isSubTypeOf( InterfaceType* itf )
{
	updateAncestors();
	return itf == this || std::binary_search( _ancestors.begin(), _ancestors.end(), itf );
}

ArrayRange<CompoundType* const> InterfaceTypeComponent::getCompoundTypeAncestors()
{
	updateAncestors();
	return _ancestors;
}

void InterfaceTypeComponent::updateAncestors()
{
	if( !_ancestors.empty() )
		return;

	// add all ancestors from super-interfaces
	size_t count = _superInterfaces.size();
	for( size_t i = 0; i < count; ++i )
	{
		InterfaceType* super = _superInterfaces[i];
		_ancestors.push_back( super );

		ArrayRange<InterfaceType* const> ancestors = super->getInterfaceAncestors();
		for( ; ancestors; ancestors.popFirst() )
			_ancestors.push_back( ancestors.getFirst() );
	}

	// remove duplicates
	std::sort( _ancestors.begin(), _ancestors.end() );
	_ancestors.erase( std::unique( _ancestors.begin(), _ancestors.end() ), _ancestors.end() );
}

CORAL_EXPORT_COMPONENT( InterfaceTypeComponent, InterfaceTypeComponent );

} // namespace co
