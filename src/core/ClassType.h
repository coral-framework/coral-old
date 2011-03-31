/*
 * Coral - A lightweight C++ component framework
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
class ClassTypeImpl : public TypeImpl
{
public:
	ClassTypeImpl();

	virtual ~ClassTypeImpl();

	/*!
		Appends the given members range to the members list. After calling this
		method the members list must be re-sorted by calling sortMembers().
	 */
	void addMembers( Range<IMember* const> members );

	//! Sorts the members list by (type, name) and updates _firstMethodPos.
	void sortMembers( ICompositeType* owner );

	// CompundType methods:
	Range<IMember* const> getMembers();
	IMember* getMember( const std::string& name );

	// IRecordType methods:
	Range<IField* const> getFields();

	// IClassType methods:
	Range<IMethod* const> getMethods();

protected:
	/*!
		Template method for retrieving the array of ancestors of this CompositeType.
		Used by the getMembers() method. By default an empty array is returned.
	 */
	virtual Range<ICompositeType* const> getCompositeTypeAncestors();

private:
	typedef RefVector<IMember> MembersVector;
	MembersVector _members;	// mixed container: fields first, then methods.
	size_t _firstMethodPos;	// dividing point: position of the first method in _members.
};

#define DELEGATE_co_ICompositeType( DELEGATE ) \
	Range<IMember* const> getMembers() { return DELEGATE getMembers(); } \
	IMember* getMember( const std::string& name ) { return DELEGATE getMember( name ); }

#define DELEGATE_co_IRecordType( DELEGATE ) \
	Range<IField* const> getFields() { return DELEGATE getFields(); }

#define DELEGATE_co_IClassType( DELEGATE ) \
	Range<IMethod* const> getMethods() { return DELEGATE getMethods(); }

} // namespace co

#endif
