/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#ifndef _CO_INTERFACETYPECOMPONENT_BASE_H_
#define _CO_INTERFACETYPECOMPONENT_BASE_H_

#include <co/InterfaceType.h>
#include <co/reserved/ComponentBase.h>

namespace co {

//! co.InterfaceTypeComponent has a facet named 'type', of type co.InterfaceType.
class InterfaceTypeComponent_co_InterfaceType : public co::InterfaceType
{
public:
	virtual co::InterfaceType* getInterfaceType();
	virtual const std::string& getInterfaceName();
};

/*!
	Inherit from this class to implement the component 'co.InterfaceTypeComponent'.
 */
class InterfaceTypeComponent_Base : public co::ComponentBase,
	public InterfaceTypeComponent_co_InterfaceType
{
public:
	InterfaceTypeComponent_Base();
	virtual ~InterfaceTypeComponent_Base();

	// co::Interface Methods:
	co::Component* getInterfaceOwner();
	void componentRetain();
	void componentRelease();

	// co::Component Methods:
	co::ComponentType* getComponentType();
	co::Interface* getInterface( co::InterfaceInfo* );
	void bindInterface( co::InterfaceInfo*, co::Interface* );
};

} // namespace co

#endif // _CO_INTERFACETYPECOMPONENT_BASE_H_
