/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#ifndef _CO_COMPONENT_BASE_H_
#define _CO_COMPONENT_BASE_H_

#include <co/IComponent.h>
#include <co/reserved/ComponentBase.h>

namespace co {

//! co.Component has a facet named 'type', of type co.IComponent.
class Component_co_IComponent : public co::IComponent
{
public:
	virtual co::IInterface* getInterfaceType();
	virtual const std::string& getInterfaceName();
};

/*!
	Inherit from this class to implement the component 'co.Component'.
 */
class Component_Base : public co::ComponentBase,
	public Component_co_IComponent
{
public:
	Component_Base();
	virtual ~Component_Base();

	// co::IService Methods:
	co::IObject* getInterfaceOwner();
	void componentRetain();
	void componentRelease();

	// co::IObject Methods:
	co::IComponent* getComponentType();
	co::IService* getInterface( co::IPort* );
	void setReceptacle( co::IPort*, co::IService* );
};

} // namespace co

#endif // _CO_COMPONENT_BASE_H_
