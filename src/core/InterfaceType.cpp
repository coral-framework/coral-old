/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#include "InterfaceType.h"
#include "TypeManager.h"
#include <co/Coral.h>
#include <co/ISystem.h>
#include <algorithm>

namespace co {

InterfaceType::~InterfaceType()
{
	// empty
}

void InterfaceType::addSuperInterface( IInterfaceType* superItf )
{
	_superInterfaces.push_back( superItf );
}

void InterfaceType::addSubInterface( IInterfaceType* subItf )
{
	_subInterfaces.push_back( subItf );
}

ArrayRange<IInterfaceType* const> InterfaceType::getInterfaceAncestors()
{
	updateAncestors();
	return _ancestors;
}

ArrayRange<IInterfaceType* const> InterfaceType::getSuperInterfaces()
{
	return _superInterfaces;
}

ArrayRange<IInterfaceType* const> InterfaceType::getSubInterfaces()
{
	return _subInterfaces;
}

const std::string& InterfaceType::getCppBlock()
{
	TypeManager* tm = dynamic_cast<TypeManager*>( getSystem()->getTypes() );
	assert( tm );
	return tm->getCppBlock( getFullName() );
}

bool InterfaceType::isSubTypeOf( IInterfaceType* itf )
{
	updateAncestors();
	return itf == this || std::binary_search( _ancestors.begin(), _ancestors.end(), itf );
}

ArrayRange<ICompoundType* const> InterfaceType::getCompoundTypeAncestors()
{
	updateAncestors();
	return _ancestors;
}

void InterfaceType::updateAncestors()
{
	if( !_ancestors.empty() )
		return;

	// add all ancestors from super-interfaces
	size_t count = _superInterfaces.size();
	for( size_t i = 0; i < count; ++i )
	{
		IInterfaceType* super = _superInterfaces[i];
		_ancestors.push_back( super );

		ArrayRange<IInterfaceType* const> ancestors = super->getInterfaceAncestors();
		for( ; ancestors; ancestors.popFirst() )
			_ancestors.push_back( ancestors.getFirst() );
	}

	// remove duplicates
	std::sort( _ancestors.begin(), _ancestors.end() );
	_ancestors.erase( std::unique( _ancestors.begin(), _ancestors.end() ), _ancestors.end() );
}

CORAL_EXPORT_COMPONENT( InterfaceType, InterfaceType );

} // namespace co
