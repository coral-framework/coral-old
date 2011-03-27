/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#ifndef _LUA_LAUNCHER_BASE_H_
#define _LUA_LAUNCHER_BASE_H_

#include <lua/ILauncher.h>
#include <co/reserved/ComponentBase.h>

namespace lua {

//! lua.Launcher has a facet named 'launcher', of type lua.ILauncher.
class Launcher_lua_ILauncher : public lua::ILauncher
{
public:
	virtual co::IInterface* getInterfaceType();
	virtual const std::string& getInterfaceName();
};

/*!
	Inherit from this class to implement the component 'lua.Launcher'.
 */
class Launcher_Base : public co::ComponentBase,
	public Launcher_lua_ILauncher
{
public:
	Launcher_Base();
	virtual ~Launcher_Base();

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

#endif // _LUA_LAUNCHER_BASE_H_
