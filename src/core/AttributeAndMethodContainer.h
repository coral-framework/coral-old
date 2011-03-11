/*
 * Coral - A lightweight C++ component framework
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

namespace co {

/*!
	Re-usable implementation of CompoundType, AttributeContainer and MethodContainer
 */
class AttributeAndMethodContainer
{
public:
	AttributeAndMethodContainer();

	virtual ~AttributeAndMethodContainer();

	//! Appends the given members range to the members list. After calling this
	//! method the members list must be re-sorted using the sortMembers() method.
	void addMembers( ArrayRange<MemberInfo* const> members );

	//! Sorts the members list by (type, name) and updates _firstMethodPos.
	void sortMembers( CompoundType* owner );

	// CompundType methods:
	ArrayRange<MemberInfo* const> getMembers();
	MemberInfo* getMember( const std::string& name );

	// AttributeContainer methods:
	ArrayRange<AttributeInfo* const> getMemberAttributes();

	// MethodContainer methods:
	ArrayRange<MethodInfo* const> getMemberMethods();

protected:
	/*!
		Template method for retrieving the array of ancestors of this CompountType.
		Used by the getMembers() method. By default an empty array is returned.
	 */
	virtual ArrayRange<CompoundType* const> getCompoundTypeAncestors();

private:
	typedef RefVector<MemberInfo> MembersVector;
	MembersVector _members;	// mixed container: attributes first, then methods.
	size_t _firstMethodPos;	// dividing point: position of the first method in _members.
};

#define DELEGATE_CO_COMPOUNDTYPE_METHODS( DELEGATE ) \
	virtual ArrayRange<MemberInfo* const> getMembers() { return DELEGATE getMembers(); } \
	virtual MemberInfo* getMember( const std::string& name ) { return DELEGATE getMember( name ); }

#define DELEGATE_CO_ATTRIBUTECONTAINER_METHODS( DELEGATE ) \
	virtual ArrayRange<AttributeInfo* const> getMemberAttributes() { return DELEGATE getMemberAttributes(); }

#define DELEGATE_CO_METHODCONTAINER_METHODS( DELEGATE ) \
	virtual ArrayRange<MethodInfo* const> getMemberMethods() { return DELEGATE getMemberMethods(); }

} // namespace co

#endif
