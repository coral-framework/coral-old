/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#ifndef _CO_SYSTEM_BASE_H_
#define _CO_SYSTEM_BASE_H_

#include <co/ISystem.h>
#include <co/reserved/ComponentBase.h>

namespace co {

//! co.System has a facet named 'system', of type co.ISystem.
class System_co_ISystem : public co::ISystem
{
public:
	virtual co::IInterface* getInterfaceType();
	virtual const std::string& getInterfaceName();
};

/*!
	Inherit from this class to implement the component 'co.System'.
 */
class System_Base : public co::ComponentBase,
	public System_co_ISystem
{
public:
	System_Base();
	virtual ~System_Base();

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

#endif // _CO_SYSTEM_BASE_H_
