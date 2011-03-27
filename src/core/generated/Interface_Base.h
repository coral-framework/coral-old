/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#ifndef _CO_INTERFACE_BASE_H_
#define _CO_INTERFACE_BASE_H_

#include <co/IInterface.h>
#include <co/reserved/ComponentBase.h>

namespace co {

//! co.Interface has a facet named 'type', of type co.IInterface.
class Interface_co_IInterface : public co::IInterface
{
public:
	virtual co::IInterface* getInterfaceType();
	virtual const std::string& getInterfaceName();
};

/*!
	Inherit from this class to implement the component 'co.Interface'.
 */
class Interface_Base : public co::ComponentBase,
	public Interface_co_IInterface
{
public:
	Interface_Base();
	virtual ~Interface_Base();

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

#endif // _CO_INTERFACE_BASE_H_
