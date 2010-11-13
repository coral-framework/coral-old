/*
 * Coral - Lightweight C++ Component Framework.
 * See Copyright Notice in Coral.h
 */

#ifndef _CO_INTERFACEINFOCOMPONENT_BASE_H_
#define _CO_INTERFACEINFOCOMPONENT_BASE_H_

#include <co/InterfaceInfo.h>
#include <co/reserved/ComponentBase.h>

namespace co {

//! co.InterfaceInfoComponent has a facet named 'interfaceInfo', of type co.InterfaceInfo.
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
	co::Component* getInterfaceOwner();
	void componentRetain();
	void componentRelease();

	// co::Component Methods:
	co::ComponentType* getComponentType();
	co::Interface* getInterface( co::InterfaceInfo* );
	void bindInterface( co::InterfaceInfo*, co::Interface* );
};

} // namespace co

#endif // _CO_INTERFACEINFOCOMPONENT_BASE_H_
