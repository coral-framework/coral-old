/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#ifndef _COMPONENTTYPE_H_
#define _COMPONENTTYPE_H_

#include "Type.h"
#include "Component_Base.h"
#include <co/RefVector.h>
#include <co/IPort.h>

namespace co {

/*!
	Implements co.IComponent.
 */
class Component : public Component_Base, public TypeImpl
{
public:
	Component();
	virtual ~Component();

	// internal methods:

	//! Appends the given interfaces range to the interfaces list. After calling this
	//! method the interfaces list must be re-sorted using the sortInterfaces() method.
	void addInterfaces( Range<IPort* const> interfaces );

	//! Sorts the interface list by (isFacet, name) and updates _firstReceptacle.
	void sortInterfaces();

	// ICompositeType methods:
	Range<IMember* const> getMembers();
	IMember* getMember( const std::string& name );

	// IComponent methods:
	Range<IPort* const> getPorts();
	Range<IPort* const> getFacets();
	Range<IPort* const> getReceptacles();

	DELEGATE_co_IType( TypeImpl:: );

private:
	typedef RefVector<IPort> InterfacesVector;
	InterfacesVector _interfaces;	// dual sorted vector: facets first, then receptacles.
	size_t _firstReceptacle;		// dividing point: position of the first receptacle in _interfaces.
};

} // namespace co

#endif
