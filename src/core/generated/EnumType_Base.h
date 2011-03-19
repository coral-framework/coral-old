/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#ifndef _CO_ENUMTYPE_BASE_H_
#define _CO_ENUMTYPE_BASE_H_

#include <co/IEnumType.h>
#include <co/reserved/ComponentBase.h>

namespace co {

//! co.EnumType has a facet named 'type', of type co.IEnumType.
class EnumType_co_IEnumType : public co::IEnumType
{
public:
	virtual co::IInterfaceType* getInterfaceType();
	virtual const std::string& getInterfaceName();
};

/*!
	Inherit from this class to implement the component 'co.EnumType'.
 */
class EnumType_Base : public co::ComponentBase,
	public EnumType_co_IEnumType
{
public:
	EnumType_Base();
	virtual ~EnumType_Base();

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

#endif // _CO_ENUMTYPE_BASE_H_
