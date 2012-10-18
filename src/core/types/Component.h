/*
 * Coral - Lightweight C++ Component Framework
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
class Component : public Type<Component_Base>
{
public:
	Component();
	virtual ~Component();

	// internal methods:

	//! Appends the given ports to the port list. After calling this method
	//! the port list must be re-sorted using sortPorts().
	void addPorts( Range<IPort*> ports );

	//! Sorts the port list by (isFacet, name) and updates _firstReceptacle.
	void sortPorts();

	// ICompositeType methods:
	Range<IMember*> getMembers();
	IMember* getMember( const std::string& name );

	// IComponent methods:
	Range<IPort*> getPorts();
	Range<IPort*> getFacets();
	Range<IPort*> getReceptacles();

private:
	RefVector<IPort> _ports; // dual sorted vector: facets first, then receptacles.
	size_t _firstReceptacle; // dividing point: position of the first receptacle in _ports.
};

} // namespace co

#endif
