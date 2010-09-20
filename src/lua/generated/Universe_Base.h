/*
 * Coral - A C++ Component Framework.
 * See Copyright Notice in Coral.h
 */

#ifndef _LUA_UNIVERSE_BASE_H_
#define _LUA_UNIVERSE_BASE_H_

#include <lua/IState.h>
#include <co/reserved/ComponentBase.h>

namespace lua {

//! lua.Universe provides an interface named 'state', of type lua.IState
class Universe_lua_IState : public lua::IState
{
public:
	virtual co::InterfaceType* getInterfaceType();
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

#endif // _LUA_UNIVERSE_BASE_H_
