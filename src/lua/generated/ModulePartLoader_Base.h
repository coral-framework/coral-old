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
	virtual co::IInterface* getInterfaceType();
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

	// co::IService Methods:
	co::IObject* getInterfaceOwner();
	void componentRetain();
	void componentRelease();

	// co::IObject Methods:
	co::IComponent* getComponentType();
	co::IService* getInterface( co::IPort* );
	void setReceptacle( co::IPort*, co::IService* );
};

} // namespace lua

#endif // _LUA_MODULEPARTLOADER_BASE_H_
