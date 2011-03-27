/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#ifndef _CO_PORT_BASE_H_
#define _CO_PORT_BASE_H_

#include <co/IPort.h>
#include <co/reserved/ComponentBase.h>

namespace co {

//! co.Port has a facet named 'port', of type co.IPort.
class Port_co_IPort : public co::IPort
{
public:
	virtual co::IInterface* getInterfaceType();
	virtual const std::string& getInterfaceName();
};

/*!
	Inherit from this class to implement the component 'co.Port'.
 */
class Port_Base : public co::ComponentBase,
	public Port_co_IPort
{
public:
	Port_Base();
	virtual ~Port_Base();

	// co::IService Methods:
	co::IObject* getInterfaceOwner();
	void componentRetain();
	void componentRelease();

	// co::IObject Methods:
	co::IComponent* getComponentType();
	co::IService* getInterface( co::IPort* );
	void setReceptacle( co::IPort*, co::IService* );
};

} // namespace co

#endif // _CO_PORT_BASE_H_
