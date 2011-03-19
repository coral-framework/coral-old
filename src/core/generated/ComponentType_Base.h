/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#ifndef _CO_COMPONENTTYPE_BASE_H_
#define _CO_COMPONENTTYPE_BASE_H_

#include <co/IComponentType.h>
#include <co/reserved/ComponentBase.h>

namespace co {

//! co.ComponentType has a facet named 'type', of type co.IComponentType.
class ComponentType_co_IComponentType : public co::IComponentType
{
public:
	virtual co::IInterfaceType* getInterfaceType();
	virtual const std::string& getInterfaceName();
};

/*!
	Inherit from this class to implement the component 'co.ComponentType'.
 */
class ComponentType_Base : public co::ComponentBase,
	public ComponentType_co_IComponentType
{
public:
	ComponentType_Base();
	virtual ~ComponentType_Base();

	// co::Interface Methods:
	co::IComponent* getInterfaceOwner();
	void componentRetain();
	void componentRelease();

	// co::IComponent Methods:
	co::IComponentType* getComponentType();
	co::Interface* getInterface( co::IInterfaceInfo* );
	void setReceptacle( co::IInterfaceInfo*, co::Interface* );
};

} // namespace co

#endif // _CO_COMPONENTTYPE_BASE_H_
