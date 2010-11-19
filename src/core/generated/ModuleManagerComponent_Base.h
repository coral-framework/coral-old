/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#ifndef _CO_MODULEMANAGERCOMPONENT_BASE_H_
#define _CO_MODULEMANAGERCOMPONENT_BASE_H_

#include <co/ModuleManager.h>
#include <co/reserved/ComponentBase.h>

namespace co {

//! co.ModuleManagerComponent has a facet named 'moduleManager', of type co.ModuleManager.
class ModuleManagerComponent_co_ModuleManager : public co::ModuleManager
{
public:
	virtual co::InterfaceType* getInterfaceType();
	virtual const std::string& getInterfaceName();
};

/*!
	Inherit from this class to implement the component 'co.ModuleManagerComponent'.
 */
class ModuleManagerComponent_Base : public co::ComponentBase,
	public ModuleManagerComponent_co_ModuleManager
{
public:
	ModuleManagerComponent_Base();
	virtual ~ModuleManagerComponent_Base();

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

#endif // _CO_MODULEMANAGERCOMPONENT_BASE_H_
