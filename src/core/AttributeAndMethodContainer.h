/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#ifndef _ATTRIBUTEANDMETHODCONTAINER_H_
#define _ATTRIBUTEANDMETHODCONTAINER_H_

#include "Type.h"
#include <co/RefVector.h>
#include <co/IMemberInfo.h>
#include <co/ICompoundType.h>
#include <co/IMethodContainer.h>
#include <co/IAttributeContainer.h>

namespace co {

/*!
	Re-usable implementation of ICompoundType, IAttributeContainer and IMethodContainer
 */
class AttributeAndMethodContainer
{
public:
	AttributeAndMethodContainer();

	virtual ~AttributeAndMethodContainer();

	//! Appends the given members range to the members list. After calling this
	//! method the members list must be re-sorted using the sortMembers() method.
	void addMembers( ArrayRange<IMemberInfo* const> members );

	//! Sorts the members list by (type, name) and updates _firstMethodPos.
	void sortMembers( ICompoundType* owner );

	// CompundType methods:
	ArrayRange<IMemberInfo* const> getMembers();
	IMemberInfo* getMember( const std::string& name );

	// IAttributeContainer methods:
	ArrayRange<IAttributeInfo* const> getMemberAttributes();

	// IMethodContainer methods:
	ArrayRange<IMethodInfo* const> getMemberMethods();

protected:
	/*!
		Template method for retrieving the array of ancestors of this CompountType.
		Used by the getMembers() method. By default an empty array is returned.
	 */
	virtual ArrayRange<ICompoundType* const> getCompoundTypeAncestors();

private:
	typedef RefVector<IMemberInfo> MembersVector;
	MembersVector _members;	// mixed container: attributes first, then methods.
	size_t _firstMethodPos;	// dividing point: position of the first method in _members.
};

#define DELEGATE_CO_COMPOUNDTYPE_METHODS( DELEGATE ) \
	virtual ArrayRange<IMemberInfo* const> getMembers() { return DELEGATE getMembers(); } \
	virtual IMemberInfo* getMember( const std::string& name ) { return DELEGATE getMember( name ); }

#define DELEGATE_CO_ATTRIBUTECONTAINER_METHODS( DELEGATE ) \
	virtual ArrayRange<IAttributeInfo* const> getMemberAttributes() { return DELEGATE getMemberAttributes(); }

#define DELEGATE_CO_METHODCONTAINER_METHODS( DELEGATE ) \
	virtual ArrayRange<IMethodInfo* const> getMemberMethods() { return DELEGATE getMemberMethods(); }

} // namespace co

#endif
