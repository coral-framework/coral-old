/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#include "Member.h"

MemberImpl::MemberImpl() : _owner( 0 ), _index( -1 )
{
	// empty
}

void MemberImpl::setName( const std::string& name )
{
	_name = name;
}

void MemberImpl::setOwner( co::ICompositeType* owner, size_t index )
{
	_owner = owner;

	assert( index < co::MAX_UINT32 );
	_index = static_cast<co::uint32>( index );
}

const std::string& MemberImpl::getName()
{
	return _name;
}

co::ICompositeType* MemberImpl::getOwner()
{
	return _owner;
}

co::uint16 MemberImpl::getIndex()
{
	return _index;
}
