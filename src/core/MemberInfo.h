/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#ifndef _MEMBERINFO_H_
#define _MEMBERINFO_H_

#include <co/IMemberInfo.h>

/*!
	Re-usable implementation of co::IMemberInfo.
 */
class MemberInfoImpl
{
public:
	MemberInfoImpl();

	// internal methods:
	void setName( const std::string& name );
	void setOwner( co::ICompoundType* owner, size_t index );

	// co::IMemberInfo methods:
	const std::string& getName();
	co::ICompoundType* getOwner();
	co::uint16 getIndex();

private:
	std::string _name;
	co::ICompoundType* _owner;
	co::uint16 _index;
};

#define DELEGATE_CO_MEMBERINFO_METHODS( DELEGATE ) \
	virtual const std::string& getName() { return DELEGATE getName(); } \
	virtual co::ICompoundType* getOwner() { return DELEGATE getOwner(); } \
	virtual co::uint16 getIndex() { return DELEGATE getIndex(); }

#endif
