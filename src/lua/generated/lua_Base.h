/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#ifndef _LUA_LUA_BASE_H_
#define _LUA_LUA_BASE_H_

#include <co/IModulePart.h>
#include <co/reserved/ComponentBase.h>

namespace lua {

//! lua.lua has a facet named 'part', of type co.IModulePart.
class lua_co_IModulePart : public co::IModulePart
{
public:
	virtual co::IInterfaceType* getInterfaceType();
	virtual const std::string& getInterfaceName();
};

/*!
	Inherit from this class to implement the component 'lua.lua'.
 */
class lua_Base : public co::ComponentBase,
	public lua_co_IModulePart
{
public:
	lua_Base();
	virtual ~lua_Base();

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

#endif // _LUA_LUA_BASE_H_
