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
	virtual co::IInterface* getInterfaceType();
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

#endif // _LUA_LUA_BASE_H_
