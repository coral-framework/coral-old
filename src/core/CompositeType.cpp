/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#include "CompositeType.h"
#include "Member.h"
#include <co/IMethod.h>
#include <co/IField.h>
#include <co/IInterface.h>
#include <algorithm>

namespace co {

//------ Helper Comparators ----------------------------------------------------

inline int memberInfoComparator( IMember* memberInfo, const std::string& name )
{
	return memberInfo->getName().compare( name );
}

inline bool typeThenNameMemberInfoComparator( const RefPtr<IMember>& m1, const RefPtr<IMember>& m2 )
{
	const IField* ai1 = dynamic_cast<const IField*>( m1.get() );
	const IField* ai2 = dynamic_cast<const IField*>( m2.get() );

	if( ( !ai1 && ai2 ) || ( ai1 && !ai2 ) )
		return ai1 != 0;

	return const_cast<IMember*>( m1.get() )->getName() < const_cast<IMember*>( m2.get() )->getName();
}

//------ CompositeTypeImpl -----------------------------------------------------

CompositeTypeImpl::CompositeTypeImpl() : _firstMethodPos( -1 )
{
	// empty
}

CompositeTypeImpl::~CompositeTypeImpl()
{
	// empty
}

void CompositeTypeImpl::addMembers( Range<IMember* const> members )
{
	_members.reserve( _members.size() + members.getSize() );
	for( ; members; members.popFirst() )
		_members.push_back( members.getFirst() );
}

void CompositeTypeImpl::sortMembers( ICompositeType* owner )
{
	assert( _firstMethodPos == size_t( -1 ) );

	// sort the members	vector
	std::sort( _members.begin(), _members.end(), typeThenNameMemberInfoComparator );

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

Range<IMember* const> CompositeTypeImpl::getMembers()
{
	return _members;
}

IMember* CompositeTypeImpl::getMember( const std::string& name )
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
	Range<ICompositeType* const> ancestors = getCompositeTypeAncestors();
	for( ; ancestors; ancestors.popFirst() )
	{
		IMember* mi = ancestors.getFirst()->getMember( name );
		if( mi )
			return mi;
	}

	return NULL;
}

Range<IField* const> CompositeTypeImpl::getFields()
{
	assert( _firstMethodPos != size_t( -1 ) );

	if( _firstMethodPos < 1 )
		return Range<IField* const>();

	// create an array range downcasting IMember* to IField*
	return Range<IField* const>(
		reinterpret_cast<IField**>( &_members.front() ), _firstMethodPos );
}

Range<IMethod* const> CompositeTypeImpl::getMethods()
{
	assert( _firstMethodPos != size_t( -1 ) );

	size_t membersSize = _members.size();
	if( _firstMethodPos >= membersSize )
		return Range<IMethod* const>();

	// create an array range downcasting IMember* to IMethod*
	return Range<IMethod* const>(
		reinterpret_cast<IMethod**>( &_members.front() + _firstMethodPos ), membersSize - _firstMethodPos );
}

Range<ICompositeType* const> CompositeTypeImpl::getCompositeTypeAncestors()
{
	return Range<ICompositeType* const>();
}

} // namespace co
