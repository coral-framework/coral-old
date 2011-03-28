/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#ifndef _CO_MEMBER_H_
#define _CO_MEMBER_H_

#include <co/IMember.h>

/*!
	Re-usable implementation of co::IMember.
 */
class MemberImpl
{
public:
	MemberImpl();

	// internal methods:
	void setName( const std::string& name );
	void setOwner( co::ICompositeType* owner, size_t index );

	// co::IMember methods:
	const std::string& getName();
	co::ICompositeType* getOwner();
	co::uint16 getIndex();

private:
	std::string _name;
	co::ICompositeType* _owner;
	co::uint16 _index;
};

#define DELEGATE_co_IMember( DELEGATE ) \
	const std::string& getName() { return DELEGATE getName(); } \
	co::ICompositeType* getOwner() { return DELEGATE getOwner(); } \
	co::uint16 getIndex() { return DELEGATE getIndex(); }

#endif
