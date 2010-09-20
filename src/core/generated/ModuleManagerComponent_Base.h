/*
 * Coral - A C++ Component Framework.
 * See Copyright Notice in Coral.h
 */

#ifndef _CO_MODULEMANAGERCOMPONENT_BASE_H_
#define _CO_MODULEMANAGERCOMPONENT_BASE_H_

#include <co/ModuleManager.h>
#include <co/reserved/ComponentBase.h>

namespace co {

//! co.ModuleManagerComponent provides an interface named 'moduleManager', of type co.ModuleManager
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
	virtual co::Component* getInterfaceOwner();
	virtual void componentRetain();
	virtual void componentRelease();

	// co::Component Methods:
	virtual co::ComponentType* getComponentType();
	virtual co::Interface* getInterface( co::InterfaceInfo* );
	virtual void bindInterface( co::InterfaceInfo*, co::Interface* );
};

} // namespace co

#endif // _CO_MODULEMANAGERCOMPONENT_BASE_H_
