/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#include "Interface.h"
#include "TypeManager.h"
#include <co/Coral.h>
#include <co/ISystem.h>
#include <algorithm>

namespace co {

Interface::~Interface()
{
	// empty
}

void Interface::addSuperInterface( IInterface* superItf )
{
	_superInterfaces.push_back( superItf );
}

void Interface::addSubInterface( IInterface* subItf )
{
	_subInterfaces.push_back( subItf );
}

Range<IInterface* const> Interface::getInterfaceAncestors()
{
	updateAncestors();
	return _ancestors;
}

Range<IInterface* const> Interface::getSuperInterfaces()
{
	return _superInterfaces;
}

Range<IInterface* const> Interface::getSubInterfaces()
{
	return _subInterfaces;
}

const std::string& Interface::getCppBlock()
{
	TypeManager* tm = dynamic_cast<TypeManager*>( getSystem()->getTypes() );
	assert( tm );
	return tm->getCppBlock( getFullName() );
}

bool Interface::isSubTypeOf( IInterface* itf )
{
	updateAncestors();
	return itf == this || std::binary_search( _ancestors.begin(), _ancestors.end(), itf );
}

Range<ICompositeType* const> Interface::getCompositeTypeAncestors()
{
	updateAncestors();
	return _ancestors;
}

void Interface::updateAncestors()
{
	if( !_ancestors.empty() )
		return;

	// add all ancestors from super-interfaces
	size_t count = _superInterfaces.size();
	for( size_t i = 0; i < count; ++i )
	{
		IInterface* super = _superInterfaces[i];
		_ancestors.push_back( super );

		Range<IInterface* const> ancestors = super->getInterfaceAncestors();
		for( ; ancestors; ancestors.popFirst() )
			_ancestors.push_back( ancestors.getFirst() );
	}

	// remove duplicates
	std::sort( _ancestors.begin(), _ancestors.end() );
	_ancestors.erase( std::unique( _ancestors.begin(), _ancestors.end() ), _ancestors.end() );
}

CORAL_EXPORT_COMPONENT( Interface, Interface );

} // namespace co
