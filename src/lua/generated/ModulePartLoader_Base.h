/*
 * Coral - A C++ Component Framework.
 * See Copyright Notice in Coral.h
 */

#ifndef _LUA_MODULEPARTLOADER_BASE_H_
#define _LUA_MODULEPARTLOADER_BASE_H_

#include <co/ModulePartLoader.h>
#include <co/reserved/ComponentBase.h>

namespace lua {

//! lua.ModulePartLoader provides an interface named 'loader', of type co.ModulePartLoader
class ModulePartLoader_co_ModulePartLoader : public co::ModulePartLoader
{
public:
	virtual co::InterfaceType* getInterfaceType();
	virtual const std::string& getInterfaceName();
};

/*!
	Inherit from this class to implement the component 'lua.ModulePartLoader'.
 */
class ModulePartLoader_Base : public co::ComponentBase,
	public ModulePartLoader_co_ModulePartLoader
{
public:
	ModulePartLoader_Base();
	virtual ~ModulePartLoader_Base();

	// co::Interface Methods:
	virtual co::Component* getInterfaceOwner();
	virtual void componentRetain();
	virtual void componentRelease();

	// co::Component Methods:
	virtual co::ComponentType* getComponentType();
	virtual co::Interface* getInterface( co::InterfaceInfo* );
	virtual void bindInterface( co::InterfaceInfo*, co::Interface* );
};

} // namespace lua

#endif // _LUA_MODULEPARTLOADER_BASE_H_
