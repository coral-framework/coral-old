/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#include "ClassType.h"
#include "Field.h"
#include "Method.h"
#include <co/IMethod.h>
#include <co/IField.h>
#include <co/IInterface.h>
#include <algorithm>

namespace co {

//------ Helper Comparators ----------------------------------------------------

inline int memberComparator( IMember* memberInfo, const std::string& name )
{
	return memberInfo->getName().compare( name );
}

inline bool memberKindComparator( IMember* mA, IMember* mB )
{
	bool isFieldA = ( mA->getKind() == MK_FIELD );
	bool isFieldB = ( mB->getKind() == MK_FIELD );

	if( ( !isFieldA && isFieldB ) || ( isFieldA && !isFieldB ) )
		return isFieldA;

	return mA->getName() < mB->getName();
}

//------ ClassTypeImpl -----------------------------------------------------

ClassTypeImpl::ClassTypeImpl() : _firstMethodPos( -1 )
{
	// empty
}

ClassTypeImpl::~ClassTypeImpl()
{
	// empty
}

void ClassTypeImpl::addMembers( Range<IMember* const> members )
{
	_members.reserve( _members.size() + members.getSize() );
	for( ; members; members.popFirst() )
		_members.push_back( members.getFirst() );
}

void ClassTypeImpl::sortMembers( ICompositeType* owner )
{
	assert( _firstMethodPos == size_t( -1 ) );

	// sort the members	vector
	if( !_members.empty() )
	{
		IMember** start = reinterpret_cast<IMember**>( &_members[0] );
		std::sort( start, start + _members.size(), memberKindComparator );
	}

	// notify members about their owner/index
	size_t count = _members.size();
	for( size_t i = 0; i < count; ++i )
	{
		if( _members[i]->getKind() == MK_FIELD )
		{
			static_cast<Field*>( _members[i].get() )->setOwner( owner, i );
		}
		else
		{
			static_cast<Method*>( _members[i].get() )->setOwner( owner, i );

			if( _firstMethodPos == size_t( -1 ) )
				_firstMethodPos = i;
		}
	}

	if( _firstMethodPos == size_t( -1 ) )
		_firstMethodPos = count;
}

Range<IMember* const> ClassTypeImpl::getMembers()
{
	return _members;
}

IMember* ClassTypeImpl::getMember( const std::string& name )
{
	size_t pos;

	// first, look for a field
	if( _firstMethodPos > 0 && _members.sortedFind( name, memberComparator, 0, _firstMethodPos - 1, pos ) )
		return _members[pos].get();

	// if it's not a field, look for a method
	size_t membersSize = _members.size();
	if( _firstMethodPos < membersSize && _members.sortedFind( name, memberComparator, _firstMethodPos, membersSize - 1, pos ) )
		return _members[pos].get();

	return NULL;
}

Range<IField* const> ClassTypeImpl::getFields()
{
	assert( _firstMethodPos != size_t( -1 ) );

	if( _firstMethodPos < 1 )
		return Range<IField* const>();

	// create an array range downcasting IMember* to IField*
	return Range<IField* const>(
		reinterpret_cast<IField**>( &_members.front() ), _firstMethodPos );
}

Range<IMethod* const> ClassTypeImpl::getMethods()
{
	assert( _firstMethodPos != size_t( -1 ) );

	size_t membersSize = _members.size();
	if( _firstMethodPos >= membersSize )
		return Range<IMethod* const>();

	// create an array range downcasting IMember* to IMethod*
	return Range<IMethod* const>(
		reinterpret_cast<IMethod**>( &_members.front() + _firstMethodPos ), membersSize - _firstMethodPos );
}

} // namespace co
