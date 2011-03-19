/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#ifndef _LUA_COMPONENT_BASE_H_
#define _LUA_COMPONENT_BASE_H_

#include <co/IReflector.h>
#include <co/IDynamicProxyHandler.h>
#include <co/reserved/ComponentBase.h>

namespace lua {

//! lua.Component has a facet named 'dynamicProxyHandler', of type co.IDynamicProxyHandler.
class Component_co_IDynamicProxyHandler : public co::IDynamicProxyHandler
{
public:
	virtual co::IInterfaceType* getInterfaceType();
	virtual const std::string& getInterfaceName();
};

//! lua.Component has a facet named 'reflector', of type co.IReflector.
class Component_co_IReflector : public co::IReflector
{
public:
	virtual co::IInterfaceType* getInterfaceType();
	virtual const std::string& getInterfaceName();
};

/*!
	Inherit from this class to implement the component 'lua.Component'.
 */
class Component_Base : public co::ComponentBase,
	public Component_co_IDynamicProxyHandler,
	public Component_co_IReflector
{
public:
	Component_Base();
	virtual ~Component_Base();

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

#endif // _LUA_COMPONENT_BASE_H_
