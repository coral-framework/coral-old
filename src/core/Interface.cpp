/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#include "Interface.h"
#include "TypeManager.h"
#include <co/Coral.h>
#include <co/ISystem.h>

namespace co {

Interface::Interface() : _baseType( NULL ), _numSuperTypes( 0 ), _superTypes( NULL )
{
	// empty
}

Interface::~Interface()
{
	delete[] _superTypes;
}

void Interface::setBaseType( IInterface* base )
{
	_baseType = base;
}

void Interface::addSubType( IInterface* sub )
{
	_subTypes.push_back( sub );
}

void Interface::updateSuperTypes()
{
	assert( _numSuperTypes == 0 && !_superTypes );

	if( !_baseType )
	{
		assert( getFullName() == "co.IService" );
		return;
	}

	IInterface* base = _baseType;
	while( base )
	{
		++_numSuperTypes;
		base = base->getBaseType();
	}

	_superTypes = new IInterface*[_numSuperTypes];

	base = _baseType;
	for( size_t i = 0; i < _numSuperTypes; ++i )
	{
		_superTypes[i] = base;
		base = base->getBaseType();		
	}

	assert( _superTypes[_numSuperTypes - 1]->getFullName() == "co.IService" );
}

Range<IMember* const> Interface::getMembers()
{
	return ClassTypeImpl::getMembers();
}

IMember* Interface::getMember( const std::string& name )
{
	IMember* res = ClassTypeImpl::getMember( name );

	if( !res && _baseType )
		res = _baseType->getMember( name );

	return res;
}

IInterface* Interface::getBaseType()
{
	return _baseType;
}

Range<IInterface* const> Interface::getSuperTypes()
{
	return Range<IInterface* const>( _superTypes, _numSuperTypes );
}

Range<IInterface* const> Interface::getSubTypes()
{
	return _subTypes;
}

const std::string& Interface::getCppBlock()
{
	TypeManager* tm = static_cast<TypeManager*>( getSystem()->getTypes() );
	return tm->getCppBlock( getFullName() );
}

bool Interface::isSubTypeOf( IInterface* type )
{
	if( type == this )
		return true;

	for( size_t i = 0; i < _numSuperTypes; ++i )
		if( _superTypes[i] == type )
			return true;

	return false;
}

CORAL_EXPORT_COMPONENT( Interface, Interface );

} // namespace co
