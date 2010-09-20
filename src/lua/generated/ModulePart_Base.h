/*******************************************************************************
** Base class generated for component 'lua.ModulePart'
**
** Created: Mon Sep 20 12:13:52 2010
**      by: Coral Compiler version 0.3.0
**
** WARNING! All changes made in this file will be lost when recompiling!
********************************************************************************/

#ifndef _LUA_MODULEPART_BASE_H_
#define _LUA_MODULEPART_BASE_H_

#include <co/ModulePart.h>
#include <co/reserved/ComponentBase.h>

namespace lua {

//! lua.ModulePart provides an interface named 'part', of type co.ModulePart
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
	virtual co::Component* getInterfaceOwner();
	virtual void componentRetain();
	virtual void componentRelease();

	// co::Component Methods:
	virtual co::ComponentType* getComponentType();
	virtual co::Interface* getInterface( co::InterfaceInfo* );
	virtual void bindInterface( co::InterfaceInfo*, co::Interface* );
};

} // namespace lua

#endif // _LUA_MODULEPART_BASE_H_
