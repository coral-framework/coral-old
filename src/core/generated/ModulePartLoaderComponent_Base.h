/*
 * Coral - A C++ Component Framework.
 * See Copyright Notice in Coral.h
 */

#ifndef _CO_MODULEPARTLOADERCOMPONENT_BASE_H_
#define _CO_MODULEPARTLOADERCOMPONENT_BASE_H_

#include <co/ModulePartLoader.h>
#include <co/reserved/ComponentBase.h>

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
