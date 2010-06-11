/*
 * Coral - A C++ Component Framework.
 * See Copyright Notice in Coral.h
 */

#include "AttributeAndMethodContainer.h"
#include "MemberInfo.h"
#include <co/MethodInfo.h>
#include <co/AttributeInfo.h>
#include <co/InterfaceType.h>
#include <algorithm>

// Helper comparators:

int memberInfoComparator( co::MemberInfo* memberInfo, const std::string& name )
{
	return memberInfo->getName().compare( name );
}

bool typeThenNameMemberInfoComparator( const co::RefPtr<co::MemberInfo>& m1, const co::RefPtr<co::MemberInfo>& m2 )
{
	const co::AttributeInfo* ai1 = dynamic_cast<const co::AttributeInfo*>( m1.get() );
	const co::AttributeInfo* ai2 = dynamic_cast<const co::AttributeInfo*>( m2.get() );

	if( ( !ai1 && ai2 ) || ( ai1 && !ai2 ) )
	{
		return ai1 != 0;
	}

	return const_cast<co::MemberInfo*>( m1.get() )->getName() < const_cast<co::MemberInfo*>( m2.get() )->getName();
}

// --- AttributeAndMethodContainer ---

AttributeAndMethodContainer::AttributeAndMethodContainer() : _firstMethodPos( -1 )
{
	// empty
}

void AttributeAndMethodContainer::addMembers( co::ArrayRange<co::MemberInfo* const> members )
{
	_members.reserve( _members.size() + members.getSize() );
	for( ; members; members.popFirst() )
		_members.push_back( members.getFirst() );
}

void AttributeAndMethodContainer::sortMembers( co::CompoundType* owner )
{
	assert( _firstMethodPos == -1 );

	// sort the members	vector
	std::sort( _members.begin(), _members.end(), typeThenNameMemberInfoComparator );

	// notify members about their owner/index
	size_t count = _members.size();
	for( size_t i = 0; i < count; ++i )
	{
		MemberInfoImpl* mii = dynamic_cast<MemberInfoImpl*>( _members[i].get() );
		assert( mii );
		mii->setOwner( owner, i );

		if( _firstMethodPos == -1 && dynamic_cast<co::MethodInfo*>( _members[i].get() ) )
			_firstMethodPos = i;
	}

	if( _firstMethodPos == -1 )
		_firstMethodPos = count;
}

co::ArrayRange<co::MemberInfo* const> AttributeAndMethodContainer::getMembers()
{
	return _members;
}

co::MemberInfo* AttributeAndMethodContainer::getMember( const std::string& name )
{
	int pos;

	// first, look for an attribute
	if( _firstMethodPos > 0 && _members.sortedFind( name, memberInfoComparator, 0, _firstMethodPos - 1, pos ) )
		return _members[pos].get();

	// if it's not an attribute, look for a method
	co::int32 membersSize = _members.size();
	if( _firstMethodPos < membersSize && _members.sortedFind( name, memberInfoComparator, _firstMethodPos, membersSize - 1, pos ) )
		return _members[pos].get();

	// finally, search our ancestors (if any)
	co::ArrayRange<co::CompoundType* const> ancestors = getCompoundTypeAncestors();
	for( ; ancestors; ancestors.popFirst() )
	{
		co::MemberInfo* mi = ancestors.getFirst()->getMember( name );
		if( mi )
			return mi;
	}

	return NULL;
}

co::ArrayRange<co::AttributeInfo* const> AttributeAndMethodContainer::getMemberAttributes()
{
	assert( _firstMethodPos >= 0 );

	if( _firstMethodPos < 1 )
		return co::ArrayRange<co::AttributeInfo* const>();

	// create an array range downcasting MemberInfo* to AttributeInfo*
	return co::ArrayRange<co::AttributeInfo* const>(
		reinterpret_cast<co::AttributeInfo**>( &_members.front() ), _firstMethodPos );
}

co::ArrayRange<co::MethodInfo* const> AttributeAndMethodContainer::getMemberMethods()
{
	assert( _firstMethodPos >= 0 );

	co::int32 membersSize = _members.size();
	if( _firstMethodPos >= membersSize )
		return co::ArrayRange<co::MethodInfo* const>();

	// create an array range downcasting MemberInfo* to MethodInfo*
	return co::ArrayRange<co::MethodInfo* const>(
		reinterpret_cast<co::MethodInfo**>( &_members.front() + _firstMethodPos ), membersSize - _firstMethodPos );
}

co::ArrayRange<co::CompoundType* const> AttributeAndMethodContainer::getCompoundTypeAncestors()
{
	return co::ArrayRange<co::CompoundType* const>();
}
