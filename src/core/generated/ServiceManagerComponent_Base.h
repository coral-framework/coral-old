/*******************************************************************************
** Base class generated for component 'co.ServiceManagerComponent'
**
** Created: Fri Jul 02 00:14:15 2010
**      by: Coral Compiler version 0.1.0
**
** WARNING! All changes made in this file will be lost when recompiling!
********************************************************************************/

#ifndef _CO_SERVICEMANAGERCOMPONENT_BASE_H_
#define _CO_SERVICEMANAGERCOMPONENT_BASE_H_

#include <co/reserved/ComponentBase.h>
#include <co/ServiceManager.h>

namespace co {

//! co.ServiceManagerComponent provides an interface named 'serviceManager', of type co.ServiceManager
class ServiceManagerComponent_co_ServiceManager : public co::ServiceManager
{
public:
	virtual co::InterfaceType* getInterfaceType();
	virtual const std::string& getInterfaceName();
};

/*!
	Inherit from this class to implement the component 'co.ServiceManagerComponent'.
 */
class ServiceManagerComponent_Base : public co::ComponentBase,
	public ServiceManagerComponent_co_ServiceManager
{
public:
	ServiceManagerComponent_Base();
	virtual ~ServiceManagerComponent_Base();

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

#endif // _CO_SERVICEMANAGERCOMPONENT_BASE_H_
