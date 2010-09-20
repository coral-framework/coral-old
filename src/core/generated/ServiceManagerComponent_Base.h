/*
 * Coral - A C++ Component Framework.
 * See Copyright Notice in Coral.h
 */

#ifndef _CO_SERVICEMANAGERCOMPONENT_BASE_H_
#define _CO_SERVICEMANAGERCOMPONENT_BASE_H_

#include <co/ServiceManager.h>
#include <co/reserved/ComponentBase.h>

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
