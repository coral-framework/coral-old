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
	virtual co::IInterfaceType* getInterfaceType();
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

#endif // _CO_SYSTEM_BASE_H_
