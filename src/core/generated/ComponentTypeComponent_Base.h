/*
 * Coral - A C++ Component Framework.
 * See Copyright Notice in Coral.h
 */

#ifndef _CO_COMPONENTTYPECOMPONENT_BASE_H_
#define _CO_COMPONENTTYPECOMPONENT_BASE_H_

#include <co/ComponentType.h>
#include <co/reserved/ComponentBase.h>

namespace co {

//! co.ComponentTypeComponent has a facet named 'type', of type co.ComponentType.
class ComponentTypeComponent_co_ComponentType : public co::ComponentType
{
public:
	virtual co::InterfaceType* getInterfaceType();
	virtual const std::string& getInterfaceName();
};

/*!
	Inherit from this class to implement the component 'co.ComponentTypeComponent'.
 */
class ComponentTypeComponent_Base : public co::ComponentBase,
	public ComponentTypeComponent_co_ComponentType
{
public:
	ComponentTypeComponent_Base();
	virtual ~ComponentTypeComponent_Base();

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

#endif // _CO_COMPONENTTYPECOMPONENT_BASE_H_
