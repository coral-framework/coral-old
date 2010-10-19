/*
 * Coral - A C++ Component Framework.
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
	co::Component* getInterfaceOwner();
	void componentRetain();
	void componentRelease();

	// co::Component Methods:
	co::ComponentType* getComponentType();
	co::Interface* getInterface( co::InterfaceInfo* );
	void bindInterface( co::InterfaceInfo*, co::Interface* );
};

} // namespace lua

#endif // _LUA_UNIVERSE_BASE_H_
