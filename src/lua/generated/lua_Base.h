/*******************************************************************************
** Base class generated for component 'lua.lua'
**
** Created: Mon Sep 20 12:13:52 2010
**      by: Coral Compiler version 0.3.0
**
** WARNING! All changes made in this file will be lost when recompiling!
********************************************************************************/

#ifndef _LUA_LUA_BASE_H_
#define _LUA_LUA_BASE_H_

#include <co/ModulePart.h>
#include <co/reserved/ComponentBase.h>

namespace lua {

//! lua.lua provides an interface named 'part', of type co.ModulePart
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
	virtual co::Component* getInterfaceOwner();
	virtual void componentRetain();
	virtual void componentRelease();

	// co::Component Methods:
	virtual co::ComponentType* getComponentType();
	virtual co::Interface* getInterface( co::InterfaceInfo* );
	virtual void bindInterface( co::InterfaceInfo*, co::Interface* );
};

} // namespace lua

#endif // _LUA_LUA_BASE_H_
