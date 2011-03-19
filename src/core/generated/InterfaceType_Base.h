/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#ifndef _CO_INTERFACETYPE_BASE_H_
#define _CO_INTERFACETYPE_BASE_H_

#include <co/IInterfaceType.h>
#include <co/reserved/ComponentBase.h>

namespace co {

//! co.InterfaceType has a facet named 'type', of type co.IInterfaceType.
class InterfaceType_co_IInterfaceType : public co::IInterfaceType
{
public:
	virtual co::IInterfaceType* getInterfaceType();
	virtual const std::string& getInterfaceName();
};

/*!
	Inherit from this class to implement the component 'co.InterfaceType'.
 */
class InterfaceType_Base : public co::ComponentBase,
	public InterfaceType_co_IInterfaceType
{
public:
	InterfaceType_Base();
	virtual ~InterfaceType_Base();

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

#endif // _CO_INTERFACETYPE_BASE_H_
