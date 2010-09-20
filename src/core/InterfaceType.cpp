/*
 * Coral - A C++ Component Framework.
 * See Copyright Notice in Coral.h
 */

#include "InterfaceType.h"
#include "TypeManager.h"
#include <co/Coral.h>
#include <co/System.h>
#include <algorithm>

InterfaceType::~InterfaceType()
{
	// empty
}

void InterfaceType::addSuperInterface( co::InterfaceType* superItf )
{
	_superInterfaces.push_back( superItf );
}

void InterfaceType::addSubInterface( co::InterfaceType* subItf )
{
	_subInterfaces.push_back( subItf );
}

co::ArrayRange<co::InterfaceType* const> InterfaceType::getInterfaceAncestors()
{
	updateAncestors();
	return _ancestors;
}

co::ArrayRange<co::InterfaceType* const> InterfaceType::getSuperInterfaces()
{
	return _superInterfaces;
}

co::ArrayRange<co::InterfaceType* const> InterfaceType::getSubInterfaces()
{
	return _subInterfaces;
}

const std::string& InterfaceType::getCppBlock()
{
	TypeManager* tm = dynamic_cast<TypeManager*>( co::getSystem()->getTypes() );
	assert( tm );
	return tm->getCppBlock( getFullName() );
}

bool InterfaceType::isSubTypeOf( co::InterfaceType* itf )
{
	updateAncestors();
	return itf == this || std::binary_search( _ancestors.begin(), _ancestors.end(), itf );
}

co::ArrayRange<co::CompoundType* const> InterfaceType::getCompoundTypeAncestors()
{
	updateAncestors();
	return _ancestors;
}

void InterfaceType::updateAncestors()
{
	if( !_ancestors.empty() )
		return;

	// add all ancestors from super-interfaces
	std::size_t count = _superInterfaces.size();
	for( std::size_t i = 0; i < count; ++i )
	{
		co::InterfaceType* super = _superInterfaces[i];
		_ancestors.push_back( super );

		co::ArrayRange<co::InterfaceType* const> ancestors = super->getInterfaceAncestors();
		for( ; ancestors; ancestors.popFirst() )
			_ancestors.push_back( ancestors.getFirst() );
	}

	// remove duplicates
	std::sort( _ancestors.begin(), _ancestors.end() );
	_ancestors.erase( std::unique( _ancestors.begin(), _ancestors.end() ), _ancestors.end() );
}

CORAL_EXPORT_COMPONENT( InterfaceType, InterfaceTypeComponent );
