/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#include "AttributeAndMethodContainer.h"
#include "MemberInfo.h"
#include <co/MethodInfo.h>
#include <co/AttributeInfo.h>
#include <co/InterfaceType.h>
#include <algorithm>

namespace co {

// ------ Helper Comparators ---------------------------------------------------

inline int memberInfoComparator( MemberInfo* memberInfo, const std::string& name )
{
	return memberInfo->getName().compare( name );
}

inline bool typeThenNameMemberInfoComparator( const RefPtr<MemberInfo>& m1, const RefPtr<MemberInfo>& m2 )
{
	const AttributeInfo* ai1 = dynamic_cast<const AttributeInfo*>( m1.get() );
	const AttributeInfo* ai2 = dynamic_cast<const AttributeInfo*>( m2.get() );

	if( ( !ai1 && ai2 ) || ( ai1 && !ai2 ) )
		return ai1 != 0;

	return const_cast<MemberInfo*>( m1.get() )->getName() < const_cast<MemberInfo*>( m2.get() )->getName();
}

// ------ AttributeAndMethodContainer ------------------------------------------

AttributeAndMethodContainer::AttributeAndMethodContainer() : _firstMethodPos( -1 )
{
	// empty
}

AttributeAndMethodContainer::~AttributeAndMethodContainer()
{
	// empty
}

void AttributeAndMethodContainer::addMembers( ArrayRange<MemberInfo* const> members )
{
	_members.reserve( _members.size() + members.getSize() );
	for( ; members; members.popFirst() )
		_members.push_back( members.getFirst() );
}

void AttributeAndMethodContainer::sortMembers( CompoundType* owner )
{
	assert( _firstMethodPos == size_t( -1 ) );

	// sort the members	vector
	std::sort( _members.begin(), _members.end(), typeThenNameMemberInfoComparator );

	// notify members about their owner/index
	size_t count = _members.size();
	for( size_t i = 0; i < count; ++i )
	{
		MemberInfoImpl* mii = dynamic_cast<MemberInfoImpl*>( _members[i].get() );
		assert( mii );
		mii->setOwner( owner, i );

		if( _firstMethodPos == size_t( -1 )
				&& dynamic_cast<MethodInfo*>( _members[i].get() ) )
			_firstMethodPos = i;
	}

	if( _firstMethodPos == size_t( -1 ) )
		_firstMethodPos = count;
}

ArrayRange<MemberInfo* const> AttributeAndMethodContainer::getMembers()
{
	return _members;
}

MemberInfo* AttributeAndMethodContainer::getMember( const std::string& name )
{
	size_t pos;

	// first, look for an attribute
	if( _firstMethodPos > 0 && _members.sortedFind( name, memberInfoComparator, 0, _firstMethodPos - 1, pos ) )
		return _members[pos].get();

	// if it's not an attribute, look for a method
	size_t membersSize = _members.size();
	if( _firstMethodPos < membersSize && _members.sortedFind( name, memberInfoComparator, _firstMethodPos, membersSize - 1, pos ) )
		return _members[pos].get();

	// finally, search our ancestors (if any)
	ArrayRange<CompoundType* const> ancestors = getCompoundTypeAncestors();
	for( ; ancestors; ancestors.popFirst() )
	{
		MemberInfo* mi = ancestors.getFirst()->getMember( name );
		if( mi )
			return mi;
	}

	return NULL;
}

ArrayRange<AttributeInfo* const> AttributeAndMethodContainer::getMemberAttributes()
{
	assert( _firstMethodPos != size_t( -1 ) );

	if( _firstMethodPos < 1 )
		return ArrayRange<AttributeInfo* const>();

	// create an array range downcasting MemberInfo* to AttributeInfo*
	return ArrayRange<AttributeInfo* const>(
		reinterpret_cast<AttributeInfo**>( &_members.front() ), _firstMethodPos );
}

ArrayRange<MethodInfo* const> AttributeAndMethodContainer::getMemberMethods()
{
	assert( _firstMethodPos != size_t( -1 ) );

	size_t membersSize = _members.size();
	if( _firstMethodPos >= membersSize )
		return ArrayRange<MethodInfo* const>();

	// create an array range downcasting MemberInfo* to MethodInfo*
	return ArrayRange<MethodInfo* const>(
		reinterpret_cast<MethodInfo**>( &_members.front() + _firstMethodPos ), membersSize - _firstMethodPos );
}

ArrayRange<CompoundType* const> AttributeAndMethodContainer::getCompoundTypeAncestors()
{
	return ArrayRange<CompoundType* const>();
}

} // namespace co
