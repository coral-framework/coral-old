/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#ifndef _CO_RESERVED_MODULEPARTBASE_H_
#define _CO_RESERVED_MODULEPARTBASE_H_

#include "ComponentBase.h"
#include <co/RefPtr.h>
#include <co/IModule.h>
#include <co/IModulePart.h>
#include <co/IModulePartLoader.h>

namespace co {

//! ModulePartBase provides an interface named 'part', of type co::IModulePart.
class CORAL_EXPORT ModulePartBase_co_ModulePart : public IModulePart
{
public:
	virtual IInterfaceType* getInterfaceType();
	virtual const std::string& getInterfaceName();
};

/*!
	Auxiliary base class used in generated code to implement the default co::IModulePart of a module.
 */
class CORAL_EXPORT ModulePartBase : public ComponentBase, public ModulePartBase_co_ModulePart
{
public:
	ModulePartBase();
	virtual ~ModulePartBase();

	// co::Interface methods:
    IComponent* getInterfaceOwner();
    void componentRetain();
    void componentRelease();

    // co::IComponent methods:
    IComponentType* getComponentType();
    Interface* getInterface( IInterfaceInfo* );
    void setReceptacle( IInterfaceInfo*, Interface* );

	// co::IModulePart methods:
	void initialize( co::IModule* module );
	void integrate( co::IModule* module );
	void integratePresentation( co::IModule* module );
	void disintegrate( co::IModule* module );
	void dispose( co::IModule* module );
};

} // namespace co

#endif // _CO_RESERVED_MODULEPARTBASE_H_
