/*******************************************************************************
** Base class generated for component 'co.SystemComponent'
**
** Created: Wed Aug 25 16:31:31 2010
**      by: Coral Compiler version 0.1.0
**
** WARNING! All changes made in this file will be lost when recompiling!
********************************************************************************/

#ifndef _CO_SYSTEMCOMPONENT_BASE_H_
#define _CO_SYSTEMCOMPONENT_BASE_H_

#include <co/reserved/ComponentBase.h>
#include <co/System.h>

namespace co {

//! co.SystemComponent provides an interface named 'system', of type co.System
class SystemComponent_co_System : public co::System
{
public:
	virtual co::InterfaceType* getInterfaceType();
	virtual const std::string& getInterfaceName();
};

/*!
	Inherit from this class to implement the component 'co.SystemComponent'.
 */
class SystemComponent_Base : public co::ComponentBase,
	public SystemComponent_co_System
{
public:
	SystemComponent_Base();
	virtual ~SystemComponent_Base();

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

#endif // _CO_SYSTEMCOMPONENT_BASE_H_
