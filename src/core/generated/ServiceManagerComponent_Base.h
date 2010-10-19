/*
 * Coral - A C++ Component Framework.
 * See Copyright Notice in Coral.h
 */

#ifndef _CO_SERVICEMANAGERCOMPONENT_BASE_H_
#define _CO_SERVICEMANAGERCOMPONENT_BASE_H_

#include <co/ServiceManager.h>
#include <co/reserved/ComponentBase.h>

namespace co {

//! co.ServiceManagerComponent has a facet named 'serviceManager', of type co.ServiceManager.
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
	co::Component* getInterfaceOwner();
	void componentRetain();
	void componentRelease();

	// co::Component Methods:
	co::ComponentType* getComponentType();
	co::Interface* getInterface( co::InterfaceInfo* );
	void bindInterface( co::InterfaceInfo*, co::Interface* );
};

} // namespace co

#endif // _CO_SERVICEMANAGERCOMPONENT_BASE_H_
