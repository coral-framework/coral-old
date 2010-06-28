/*******************************************************************************
** Base class generated for component 'co.InterfaceInfoComponent'
**
** Created: Mon Jun 28 12:01:36 2010
**      by: Coral Compiler version 0.1.0
**
** WARNING! All changes made in this file will be lost when recompiling!
********************************************************************************/

#ifndef _CO_INTERFACEINFOCOMPONENT_BASE_H_
#define _CO_INTERFACEINFOCOMPONENT_BASE_H_

#include <co/reserved/ComponentBase.h>
#include <co/InterfaceInfo.h>

namespace co {

//! co.InterfaceInfoComponent provides an interface named 'interfaceInfo', of type co.InterfaceInfo
class InterfaceInfoComponent_co_InterfaceInfo : public co::InterfaceInfo
{
public:
	virtual co::InterfaceType* getInterfaceType();
	virtual const std::string& getInterfaceName();
};

/*!
	Inherit from this class to implement the component 'co.InterfaceInfoComponent'.
 */
class InterfaceInfoComponent_Base : public co::ComponentBase,
	public InterfaceInfoComponent_co_InterfaceInfo
{
public:
	InterfaceInfoComponent_Base();
	virtual ~InterfaceInfoComponent_Base();

	// co::Interface Methods:
	virtual co::Component* getInterfaceOwner();
	virtual void componentRetain();
	virtual void componentRelease();

	// co::Component Methods:
	virtual co::ComponentType* getComponentType();
	virtual co::Interface* getInterface( co::InterfaceInfo* );
	virtual void bindInterface( co::InterfaceInfo*, co::Interface* );
};

} // namespace co

#endif // _CO_INTERFACEINFOCOMPONENT_BASE_H_
