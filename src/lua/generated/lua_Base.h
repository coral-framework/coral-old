/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#ifndef _LUA_LUA_BASE_H_
#define _LUA_LUA_BASE_H_

#include <co/ModulePart.h>
#include <co/reserved/ComponentBase.h>

namespace lua {

//! lua.lua has a facet named 'part', of type co.ModulePart.
class lua_co_ModulePart : public co::ModulePart
{
public:
	virtual co::InterfaceType* getInterfaceType();
	virtual const std::string& getInterfaceName();
};

/*!
	Inherit from this class to implement the component 'lua.lua'.
 */
class lua_Base : public co::ComponentBase,
	public lua_co_ModulePart
{
public:
	lua_Base();
	virtual ~lua_Base();

	// co::Interface Methods:
	co::Component* getInterfaceOwner();
	void componentRetain();
	void componentRelease();

	// co::Component Methods:
	co::ComponentType* getComponentType();
	co::Interface* getInterface( co::InterfaceInfo* );
	void setReceptacle( co::InterfaceInfo*, co::Interface* );
};

} // namespace lua

#endif // _LUA_LUA_BASE_H_
