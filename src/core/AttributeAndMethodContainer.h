/*
 * Coral - A C++ Component Framework.
 * See Copyright Notice in Coral.h
 */

#ifndef _ATTRIBUTEANDMETHODCONTAINER_H_
#define _ATTRIBUTEANDMETHODCONTAINER_H_

#include "Type.h"
#include <co/RefVector.h>
#include <co/MemberInfo.h>
#include <co/CompoundType.h>
#include <co/MethodContainer.h>
#include <co/AttributeContainer.h>

/*!
	Re-usable implementation of co::CompoundType, co::AttributeContainer and co::MethodContainer
 */
class AttributeAndMethodContainer
{
public:
	AttributeAndMethodContainer();

	//! Appends the given members range to the members list. After calling this
	//! method the members list must be re-sorted using the sortMembers() method.
	void addMembers( co::ArrayRange<co::MemberInfo* const> members );

	//! Sorts the members list by (type, name) and updates _firstMethodPos.
	void sortMembers( co::CompoundType* owner );

	// co::CompundType methods:
	co::ArrayRange<co::MemberInfo* const> getMembers();
	co::MemberInfo* getMember( const std::string& name );

	// co::AttributeContainer methods:
	co::ArrayRange<co::AttributeInfo* const> getMemberAttributes();

	// co::MethodContainer methods:
	co::ArrayRange<co::MethodInfo* const> getMemberMethods();

protected:
	/*!
		Template method for retrieving the array of ancestors of this CompountType.
		Used by the getMembers() method. By default an empty array is returned.
	 */
	virtual co::ArrayRange<co::CompoundType* const> getCompoundTypeAncestors();

private:
	typedef co::RefVector<co::MemberInfo> MembersVector;
	MembersVector _members;		// mixed container: attributes first, then methods.
	co::int32 _firstMethodPos;	// dividing point: position of the first method in _members.
};

#define DELEGATE_CO_COMPOUNDTYPE_METHODS( DELEGATE ) \
	virtual co::ArrayRange<co::MemberInfo* const> getMembers() { return DELEGATE getMembers(); } \
	virtual co::MemberInfo* getMember( const std::string& name ) { return DELEGATE getMember( name ); }

#define DELEGATE_CO_ATTRIBUTECONTAINER_METHODS( DELEGATE ) \
	virtual co::ArrayRange<co::AttributeInfo* const> getMemberAttributes() { return DELEGATE getMemberAttributes(); }

#define DELEGATE_CO_METHODCONTAINER_METHODS( DELEGATE ) \
	virtual co::ArrayRange<co::MethodInfo* const> getMemberMethods() { return DELEGATE getMemberMethods(); }

#endif
