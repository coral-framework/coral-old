/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#ifndef _CO_MODULEMANAGER_BASE_H_
#define _CO_MODULEMANAGER_BASE_H_

#include <co/IModuleManager.h>
#include <co/reserved/ComponentBase.h>

namespace co {

//! co.ModuleManager has a facet named 'moduleManager', of type co.IModuleManager.
class ModuleManager_co_IModuleManager : public co::IModuleManager
{
public:
	virtual co::IInterfaceType* getInterfaceType();
	virtual const std::string& getInterfaceName();
};

/*!
	Inherit from this class to implement the component 'co.ModuleManager'.
 */
class ModuleManager_Base : public co::ComponentBase,
	public ModuleManager_co_IModuleManager
{
public:
	ModuleManager_Base();
	virtual ~ModuleManager_Base();

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

#endif // _CO_MODULEMANAGER_BASE_H_
