/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#ifndef _COMPONENTTYPE_H_
#define _COMPONENTTYPE_H_

#include "Type.h"
#include "ComponentTypeComponent_Base.h"
#include <co/RefVector.h>
#include <co/InterfaceInfo.h>

namespace co {

/*!
	Component that implements co.ComponentType.
 */
class ComponentTypeComponent : public ComponentTypeComponent_Base, public TypeImpl
{
public:
	ComponentTypeComponent();
	virtual ~ComponentTypeComponent();

	// internal methods:

	//! Appends the given interfaces range to the interfaces list. After calling this
	//! method the interfaces list must be re-sorted using the sortInterfaces() method.
	void addInterfaces( ArrayRange<InterfaceInfo* const> interfaces );

	//! Sorts the interface list by (isFacet, name) and updates _firstReceptacle.
	void sortInterfaces();

	// CompoundType methods:
	ArrayRange<MemberInfo* const> getMembers();
	MemberInfo* getMember( const std::string& name );

	// ComponentType methods:
	ArrayRange<InterfaceInfo* const> getInterfaces();
	ArrayRange<InterfaceInfo* const> getFacets();
	ArrayRange<InterfaceInfo* const> getReceptacles();

	DELEGATE_CO_TYPE_METHODS( TypeImpl:: );

private:
	typedef RefVector<InterfaceInfo> InterfacesVector;
	InterfacesVector _interfaces;	// dual sorted vector: facets first, then receptacles.
	size_t _firstReceptacle;		// dividing point: position of the first receptacle in _interfaces.
};

} // namespace co

#endif
