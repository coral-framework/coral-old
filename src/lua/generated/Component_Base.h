/*
 * Coral - Lightweight C++ Component Framework.
 * See Copyright Notice in Coral.h
 */

#ifndef _LUA_COMPONENT_BASE_H_
#define _LUA_COMPONENT_BASE_H_

#include <co/Reflector.h>
#include <co/DynamicProxyHandler.h>
#include <co/reserved/ComponentBase.h>

namespace lua {

//! lua.Component has a facet named 'dynamicProxyHandler', of type co.DynamicProxyHandler.
class Component_co_DynamicProxyHandler : public co::DynamicProxyHandler
{
public:
	virtual co::InterfaceType* getInterfaceType();
	virtual const std::string& getInterfaceName();
};

//! lua.Component has a facet named 'reflector', of type co.Reflector.
class Component_co_Reflector : public co::Reflector
{
public:
	virtual co::InterfaceType* getInterfaceType();
	virtual const std::string& getInterfaceName();
};

/*!
	Inherit from this class to implement the component 'lua.Component'.
 */
class Component_Base : public co::ComponentBase,
	public Component_co_DynamicProxyHandler,
	public Component_co_Reflector
{
public:
	Component_Base();
	virtual ~Component_Base();

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

#endif // _LUA_COMPONENT_BASE_H_
