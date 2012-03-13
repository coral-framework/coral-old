/*
 * Coral - Lightweight C++ Component Framework
 * See Copyright Notice in Coral.h
 */

#ifndef _CO_CLASSTYPE_H_
#define _CO_CLASSTYPE_H_

#include "Type.h"
#include <co/IMember.h>
#include <co/IClassType.h>
#include <co/IRecordType.h>
#include <co/ICompositeType.h>
#include <co/RefVector.h>

namespace co {

/*!
	Reusable implementation for all class/record types.
 */
class ClassTypeImpl
{
public:
	inline ClassTypeImpl() : _firstMethodPos( -1 )
	{;}

	/*!
		Appends the given members range to the members list. After calling this
		method the members list must be re-sorted by calling sortMembers().
	 */
	void addMembers( Range<IMember*> members );

	//! Sorts the members list by (type, name) and updates _firstMethodPos.
	void sortMembers( ICompositeType* owner );

	// ICompositeType methods:
	IMember* getMember( const std::string& name );

	// IRecordType methods:
	Range<IField*> getFields();

	// IClassType methods:
	Range<IMethod*> getMethods();

protected:
	typedef RefVector<IMember> MembersVector;
	MembersVector _members;	// mixed container: fields first, then methods.
	size_t _firstMethodPos;	// dividing point: position of the first method in _members.
};
	
template<class Base>
class ClassType : public Type<Base>, public ClassTypeImpl
{
public:
	Range<IMember*> getMembers() { return _members; }
	IMember* getMember( const std::string& name ) { return ClassTypeImpl::getMember( name ); }
	Range<IField*> getFields() { return ClassTypeImpl::getFields(); }
	Range<IMethod*> getMethods() { return ClassTypeImpl::getMethods(); }
};

} // namespace co

#endif
