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
	virtual co::IInterface* getInterfaceType();
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

#endif // _CO_MODULE_BASE_H_
