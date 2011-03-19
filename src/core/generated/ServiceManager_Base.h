/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#ifndef _CO_SERVICEMANAGER_BASE_H_
#define _CO_SERVICEMANAGER_BASE_H_

#include <co/IServiceManager.h>
#include <co/reserved/ComponentBase.h>

namespace co {

//! co.ServiceManager has a facet named 'serviceManager', of type co.IServiceManager.
class ServiceManager_co_IServiceManager : public co::IServiceManager
{
public:
	virtual co::IInterfaceType* getInterfaceType();
	virtual const std::string& getInterfaceName();
};

/*!
	Inherit from this class to implement the component 'co.ServiceManager'.
 */
class ServiceManager_Base : public co::ComponentBase,
	public ServiceManager_co_IServiceManager
{
public:
	ServiceManager_Base();
	virtual ~ServiceManager_Base();

	// co::Interface Methods:
	co::IComponent* getInterfaceOwner();
	void componentRetain();
	void componentRelease();

	// co::IComponent Methods:
	co::IComponentType* getComponentType();
	co::Interface* getInterface( co::IInterfaceInfo* );
	void setReceptacle( co::IInterfaceInfo*, co::Interface* );
};

} // namespace co

#endif // _CO_SERVICEMANAGER_BASE_H_
