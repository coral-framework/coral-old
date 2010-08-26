/*******************************************************************************
** Base class generated for component 'co.ModuleComponent'
**
** Created: Wed Aug 25 16:31:31 2010
**      by: Coral Compiler version 0.1.0
**
** WARNING! All changes made in this file will be lost when recompiling!
********************************************************************************/

#ifndef _CO_MODULECOMPONENT_BASE_H_
#define _CO_MODULECOMPONENT_BASE_H_

#include <co/reserved/ComponentBase.h>
#include <co/Module.h>

namespace co {

//! co.ModuleComponent provides an interface named 'module', of type co.Module
class ModuleComponent_co_Module : public co::Module
{
public:
	virtual co::InterfaceType* getInterfaceType();
	virtual const std::string& getInterfaceName();
};

/*!
	Inherit from this class to implement the component 'co.ModuleComponent'.
 */
class ModuleComponent_Base : public co::ComponentBase,
	public ModuleComponent_co_Module
{
public:
	ModuleComponent_Base();
	virtual ~ModuleComponent_Base();

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

#endif // _CO_MODULECOMPONENT_BASE_H_
