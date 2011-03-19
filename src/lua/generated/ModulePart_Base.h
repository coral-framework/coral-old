/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#ifndef _LUA_MODULEPART_BASE_H_
#define _LUA_MODULEPART_BASE_H_

#include <co/IModulePart.h>
#include <co/reserved/ComponentBase.h>

namespace lua {

//! lua.ModulePart has a facet named 'part', of type co.IModulePart.
class ModulePart_co_IModulePart : public co::IModulePart
{
public:
	virtual co::IInterfaceType* getInterfaceType();
	virtual const std::string& getInterfaceName();
};

/*!
	Inherit from this class to implement the component 'lua.ModulePart'.
 */
class ModulePart_Base : public co::ComponentBase,
	public ModulePart_co_IModulePart
{
public:
	ModulePart_Base();
	virtual ~ModulePart_Base();

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

#endif // _LUA_MODULEPART_BASE_H_
