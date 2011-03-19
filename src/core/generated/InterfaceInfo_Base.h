/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#ifndef _CO_INTERFACEINFO_BASE_H_
#define _CO_INTERFACEINFO_BASE_H_

#include <co/IInterfaceInfo.h>
#include <co/reserved/ComponentBase.h>

namespace co {

//! co.InterfaceInfo has a facet named 'interfaceInfo', of type co.IInterfaceInfo.
class InterfaceInfo_co_IInterfaceInfo : public co::IInterfaceInfo
{
public:
	virtual co::IInterfaceType* getInterfaceType();
	virtual const std::string& getInterfaceName();
};

/*!
	Inherit from this class to implement the component 'co.InterfaceInfo'.
 */
class InterfaceInfo_Base : public co::ComponentBase,
	public InterfaceInfo_co_IInterfaceInfo
{
public:
	InterfaceInfo_Base();
	virtual ~InterfaceInfo_Base();

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

#endif // _CO_INTERFACEINFO_BASE_H_
