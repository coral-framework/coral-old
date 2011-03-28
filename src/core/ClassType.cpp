/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#include "ClassType.h"
#include "Member.h"
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
	IMember** start = reinterpret_cast<IMember**>( &_members[0] );
	std::sort( start, start + _members.size(), memberKindComparator );

	// notify members about their owner/index
	size_t count = _members.size();
	for( size_t i = 0; i < count; ++i )
	{
		MemberImpl* mii = dynamic_cast<MemberImpl*>( _members[i].get() );
		assert( mii );
		mii->setOwner( owner, i );

		if( _firstMethodPos == size_t( -1 )
				&& dynamic_cast<IMethod*>( _members[i].get() ) )
			_firstMethodPos = i;
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

	// first, look for an attribute
	if( _firstMethodPos > 0 && _members.sortedFind( name, memberComparator, 0, _firstMethodPos - 1, pos ) )
		return _members[pos].get();

	// if it's not an attribute, look for a method
	size_t membersSize = _members.size();
	if( _firstMethodPos < membersSize && _members.sortedFind( name, memberComparator, _firstMethodPos, membersSize - 1, pos ) )
		return _members[pos].get();

	// finally, search our ancestors (if any)
	Range<ICompositeType* const> ancestors = getCompositeTypeAncestors();
	for( ; ancestors; ancestors.popFirst() )
	{
		IMember* mi = ancestors.getFirst()->getMember( name );
		if( mi )
			return mi;
	}

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

Range<ICompositeType* const> ClassTypeImpl::getCompositeTypeAncestors()
{
	return Range<ICompositeType* const>();
}

} // namespace co
