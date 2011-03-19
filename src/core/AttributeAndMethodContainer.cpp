/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#include "AttributeAndMethodContainer.h"
#include "MemberInfo.h"
#include <co/IMethodInfo.h>
#include <co/IAttributeInfo.h>
#include <co/IInterfaceType.h>
#include <algorithm>

namespace co {

// ------ Helper Comparators ---------------------------------------------------

inline int memberInfoComparator( IMemberInfo* memberInfo, const std::string& name )
{
	return memberInfo->getName().compare( name );
}

inline bool typeThenNameMemberInfoComparator( const RefPtr<IMemberInfo>& m1, const RefPtr<IMemberInfo>& m2 )
{
	const IAttributeInfo* ai1 = dynamic_cast<const IAttributeInfo*>( m1.get() );
	const IAttributeInfo* ai2 = dynamic_cast<const IAttributeInfo*>( m2.get() );

	if( ( !ai1 && ai2 ) || ( ai1 && !ai2 ) )
		return ai1 != 0;

	return const_cast<IMemberInfo*>( m1.get() )->getName() < const_cast<IMemberInfo*>( m2.get() )->getName();
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

void AttributeAndMethodContainer::addMembers( ArrayRange<IMemberInfo* const> members )
{
	_members.reserve( _members.size() + members.getSize() );
	for( ; members; members.popFirst() )
		_members.push_back( members.getFirst() );
}

void AttributeAndMethodContainer::sortMembers( ICompoundType* owner )
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
				&& dynamic_cast<IMethodInfo*>( _members[i].get() ) )
			_firstMethodPos = i;
	}

	if( _firstMethodPos == size_t( -1 ) )
		_firstMethodPos = count;
}

ArrayRange<IMemberInfo* const> AttributeAndMethodContainer::getMembers()
{
	return _members;
}

IMemberInfo* AttributeAndMethodContainer::getMember( const std::string& name )
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
	ArrayRange<ICompoundType* const> ancestors = getCompoundTypeAncestors();
	for( ; ancestors; ancestors.popFirst() )
	{
		IMemberInfo* mi = ancestors.getFirst()->getMember( name );
		if( mi )
			return mi;
	}

	return NULL;
}

ArrayRange<IAttributeInfo* const> AttributeAndMethodContainer::getMemberAttributes()
{
	assert( _firstMethodPos != size_t( -1 ) );

	if( _firstMethodPos < 1 )
		return ArrayRange<IAttributeInfo* const>();

	// create an array range downcasting IMemberInfo* to IAttributeInfo*
	return ArrayRange<IAttributeInfo* const>(
		reinterpret_cast<IAttributeInfo**>( &_members.front() ), _firstMethodPos );
}

ArrayRange<IMethodInfo* const> AttributeAndMethodContainer::getMemberMethods()
{
	assert( _firstMethodPos != size_t( -1 ) );

	size_t membersSize = _members.size();
	if( _firstMethodPos >= membersSize )
		return ArrayRange<IMethodInfo* const>();

	// create an array range downcasting IMemberInfo* to IMethodInfo*
	return ArrayRange<IMethodInfo* const>(
		reinterpret_cast<IMethodInfo**>( &_members.front() + _firstMethodPos ), membersSize - _firstMethodPos );
}

ArrayRange<ICompoundType* const> AttributeAndMethodContainer::getCompoundTypeAncestors()
{
	return ArrayRange<ICompoundType* const>();
}

} // namespace co
