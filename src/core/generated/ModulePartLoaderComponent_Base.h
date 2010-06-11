/*******************************************************************************
** Base class generated for component 'co.ModulePartLoaderComponent'
**
** Created: Thu Jun 10 20:53:30 2010
**      by: Coral Compiler version 0.1.0
**
** WARNING! All changes made in this file will be lost when recompiling!
********************************************************************************/

#ifndef _CO_MODULEPARTLOADERCOMPONENT_BASE_H_
#define _CO_MODULEPARTLOADERCOMPONENT_BASE_H_

#include <co/reserved/ComponentBase.h>
#include <co/ModulePartLoader.h>

namespace co {

//! co.ModulePartLoaderComponent provides an interface named 'loader', of type co.ModulePartLoader
class ModulePartLoaderComponent_co_ModulePartLoader : public co::ModulePartLoader
{
public:
	virtual co::InterfaceType* getInterfaceType();
	virtual const std::string& getInterfaceName();
};

/*!
	Inherit from this class to implement the component 'co.ModulePartLoaderComponent'.
 */
class ModulePartLoaderComponent_Base : public co::ComponentBase,
	public ModulePartLoaderComponent_co_ModulePartLoader
{
public:
	ModulePartLoaderComponent_Base();
	virtual ~ModulePartLoaderComponent_Base();

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

#endif // _CO_MODULEPARTLOADERCOMPONENT_BASE_H_
