/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#ifndef _CO_MODULECOMPONENT_BASE_H_
#define _CO_MODULECOMPONENT_BASE_H_

#include <co/Module.h>
#include <co/reserved/ComponentBase.h>

namespace co {

//! co.ModuleComponent has a facet named 'module', of type co.Module.
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
	co::Component* getInterfaceOwner();
	void componentRetain();
	void componentRelease();

	// co::Component Methods:
	co::ComponentType* getComponentType();
	co::Interface* getInterface( co::InterfaceInfo* );
	void bindInterface( co::InterfaceInfo*, co::Interface* );
};

} // namespace co

#endif // _CO_MODULECOMPONENT_BASE_H_
