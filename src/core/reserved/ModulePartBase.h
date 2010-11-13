/*
 * Coral - Lightweight C++ Component Framework.
 * See Copyright Notice in Coral.h
 */

#ifndef _CO_RESERVED_MODULEPARTBASE_H_
#define _CO_RESERVED_MODULEPARTBASE_H_

#include "ComponentBase.h"
#include <co/RefPtr.h>
#include <co/Module.h>
#include <co/ModulePart.h>
#include <co/ModulePartLoader.h>

namespace co {

//! ModulePartBase provides an interface named 'part', of type co::ModulePart.
class CORAL_EXPORT ModulePartBase_co_ModulePart : public ModulePart
{
public:
	virtual InterfaceType* getInterfaceType();
	virtual const std::string& getInterfaceName();
};

/*!
	Auxiliary base class used in generated code to implement the default co::ModulePart of a module.
 */
class CORAL_EXPORT ModulePartBase : public ComponentBase, public ModulePartBase_co_ModulePart
{
public:
	ModulePartBase();
	virtual ~ModulePartBase();

	// co::Interface methods:
    Component* getInterfaceOwner();
    void componentRetain();
    void componentRelease();

    // co::Component methods:
    ComponentType* getComponentType();
    Interface* getInterface( InterfaceInfo* );
    void bindInterface( InterfaceInfo*, Interface* );

	// co::ModulePart methods:
	void initialize( co::Module* module );
	void integrate( co::Module* module );
	void integratePresentation( co::Module* module );
	void disintegrate( co::Module* module );
	void dispose( co::Module* module );
};

} // namespace co

#endif // _CO_RESERVED_MODULEPARTBASE_H_
