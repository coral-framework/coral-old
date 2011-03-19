/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#include "MemberInfo.h"

MemberInfoImpl::MemberInfoImpl() : _owner( 0 ), _index( -1 )
{
	// empty
}

void MemberInfoImpl::setName( const std::string& name )
{
	_name = name;
}

void MemberInfoImpl::setOwner( co::ICompoundType* owner, size_t index )
{
	_owner = owner;

	assert( index < co::MAX_UINT32 );
	_index = static_cast<co::uint32>( index );
}

const std::string& MemberInfoImpl::getName()
{
	return _name;
}

co::ICompoundType* MemberInfoImpl::getOwner()
{
	return _owner;
}

co::uint16 MemberInfoImpl::getIndex()
{
	return _index;
}
