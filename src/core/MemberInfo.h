/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#ifndef _MEMBERINFO_H_
#define _MEMBERINFO_H_

#include <co/MemberInfo.h>

/*!
	Re-usable implementation of co::MemberInfo.
 */
class MemberInfoImpl
{
public:
	MemberInfoImpl();

	// internal methods:
	void setName( const std::string& name );
	void setOwner( co::CompoundType* owner, size_t index );

	// co::MemberInfo methods:
	const std::string& getName();
	co::CompoundType* getOwner();
	co::uint32 getIndex();

private:
	std::string _name;
	co::CompoundType* _owner;
	co::uint32 _index;
};

#define DELEGATE_CO_MEMBERINFO_METHODS( DELEGATE ) \
	virtual const std::string& getName() { return DELEGATE getName(); } \
	virtual co::CompoundType* getOwner() { return DELEGATE getOwner(); } \
	virtual co::uint32 getIndex() { return DELEGATE getIndex(); }

#endif
