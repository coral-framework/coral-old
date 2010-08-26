/*******************************************************************************
** Base class generated for component 'co.ComponentTypeComponent'
**
** Created: Wed Aug 25 16:31:31 2010
**      by: Coral Compiler version 0.1.0
**
** WARNING! All changes made in this file will be lost when recompiling!
********************************************************************************/

#ifndef _CO_COMPONENTTYPECOMPONENT_BASE_H_
#define _CO_COMPONENTTYPECOMPONENT_BASE_H_

#include <co/reserved/ComponentBase.h>
#include <co/ComponentType.h>

namespace co {

//! co.ComponentTypeComponent provides an interface named 'type', of type co.ComponentType
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
	virtual co::Component* getInterfaceOwner();
	virtual void componentRetain();
	virtual void componentRelease();

	// co::Component Methods:
	virtual co::ComponentType* getComponentType();
	virtual co::Interface* getInterface( co::InterfaceInfo* );
	virtual void bindInterface( co::InterfaceInfo*, co::Interface* );
};

} // namespace co

#endif // _CO_COMPONENTTYPECOMPONENT_BASE_H_
