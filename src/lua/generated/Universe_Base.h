/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#ifndef _LUA_UNIVERSE_BASE_H_
#define _LUA_UNIVERSE_BASE_H_

#include <lua/IState.h>
#include <co/reserved/ComponentBase.h>

namespace lua {

//! lua.Universe has a facet named 'state', of type lua.IState.
class Universe_lua_IState : public lua::IState
{
public:
	virtual co::IInterface* getInterfaceType();
	virtual const std::string& getInterfaceName();
};

/*!
	Inherit from this class to implement the component 'lua.Universe'.
 */
class Universe_Base : public co::ComponentBase,
	public Universe_lua_IState
{
public:
	Universe_Base();
	virtual ~Universe_Base();

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

#endif // _LUA_UNIVERSE_BASE_H_
