/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#ifndef _CO_MODULE_BASE_H_
#define _CO_MODULE_BASE_H_

#include <co/IModule.h>
#include <co/reserved/ComponentBase.h>

namespace co {

//! co.Module has a facet named 'module', of type co.IModule.
class Module_co_IModule : public co::IModule
{
public:
	virtual co::IInterfaceType* getInterfaceType();
	virtual const std::string& getInterfaceName();
};

/*!
	Inherit from this class to implement the component 'co.Module'.
 */
class Module_Base : public co::ComponentBase,
	public Module_co_IModule
{
public:
	Module_Base();
	virtual ~Module_Base();

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

#endif // _CO_MODULE_BASE_H_
