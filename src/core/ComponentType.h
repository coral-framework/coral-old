/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#ifndef _COMPONENTTYPE_H_
#define _COMPONENTTYPE_H_

#include "Type.h"
#include "ComponentType_Base.h"
#include <co/RefVector.h>
#include <co/IInterfaceInfo.h>

namespace co {

/*!
	Implements co.IComponentType.
 */
class ComponentType : public ComponentType_Base, public TypeImpl
{
public:
	ComponentType();
	virtual ~ComponentType();

	// internal methods:

	//! Appends the given interfaces range to the interfaces list. After calling this
	//! method the interfaces list must be re-sorted using the sortInterfaces() method.
	void addInterfaces( ArrayRange<IInterfaceInfo* const> interfaces );

	//! Sorts the interface list by (isFacet, name) and updates _firstReceptacle.
	void sortInterfaces();

	// ICompoundType methods:
	ArrayRange<IMemberInfo* const> getMembers();
	IMemberInfo* getMember( const std::string& name );

	// IComponentType methods:
	ArrayRange<IInterfaceInfo* const> getInterfaces();
	ArrayRange<IInterfaceInfo* const> getFacets();
	ArrayRange<IInterfaceInfo* const> getReceptacles();

	DELEGATE_CO_TYPE_METHODS( TypeImpl:: );

private:
	typedef RefVector<IInterfaceInfo> InterfacesVector;
	InterfacesVector _interfaces;	// dual sorted vector: facets first, then receptacles.
	size_t _firstReceptacle;		// dividing point: position of the first receptacle in _interfaces.
};

} // namespace co

#endif
