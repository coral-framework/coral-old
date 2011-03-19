/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#ifndef _LUA_MODULEPARTLOADER_BASE_H_
#define _LUA_MODULEPARTLOADER_BASE_H_

#include <co/IModulePartLoader.h>
#include <co/reserved/ComponentBase.h>

namespace lua {

//! lua.ModulePartLoader has a facet named 'loader', of type co.IModulePartLoader.
class ModulePartLoader_co_IModulePartLoader : public co::IModulePartLoader
{
public:
	virtual co::IInterfaceType* getInterfaceType();
	virtual const std::string& getInterfaceName();
};

/*!
	Inherit from this class to implement the component 'lua.ModulePartLoader'.
 */
class ModulePartLoader_Base : public co::ComponentBase,
	public ModulePartLoader_co_IModulePartLoader
{
public:
	ModulePartLoader_Base();
	virtual ~ModulePartLoader_Base();

	// co::Interface Methods:
	co::IComponent* getInterfaceOwner();
	void componentRetain();
	void componentRelease();

	// co::IComponent Methods:
	co::IComponentType* getComponentType();
	co::Interface* getInterface( co::IInterfaceInfo* );
	void setReceptacle( co::IInterfaceInfo*, co::Interface* );
};

} // namespace lua

#endif // _LUA_MODULEPARTLOADER_BASE_H_
