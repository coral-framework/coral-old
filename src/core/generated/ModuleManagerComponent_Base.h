/*******************************************************************************
** Base class generated for component 'co.ModuleManagerComponent'
**
** Created: Fri Jul 02 00:14:15 2010
**      by: Coral Compiler version 0.1.0
**
** WARNING! All changes made in this file will be lost when recompiling!
********************************************************************************/

#ifndef _CO_MODULEMANAGERCOMPONENT_BASE_H_
#define _CO_MODULEMANAGERCOMPONENT_BASE_H_

#include <co/reserved/ComponentBase.h>
#include <co/ModuleManager.h>

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
