/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#ifndef _LUA_MODULEPART_BASE_H_
#define _LUA_MODULEPART_BASE_H_

#include <co/ModulePart.h>
#include <co/reserved/ComponentBase.h>

namespace lua {

//! lua.ModulePart has a facet named 'part', of type co.ModulePart.
class ModulePart_co_ModulePart : public co::ModulePart
{
public:
	virtual co::InterfaceType* getInterfaceType();
	virtual const std::string& getInterfaceName();
};

/*!
	Inherit from this class to implement the component 'lua.ModulePart'.
 */
class ModulePart_Base : public co::ComponentBase,
	public ModulePart_co_ModulePart
{
public:
	ModulePart_Base();
	virtual ~ModulePart_Base();

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

#endif // _LUA_MODULEPART_BASE_H_
