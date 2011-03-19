/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#ifndef _CO_ATTRIBUTEINFO_BASE_H_
#define _CO_ATTRIBUTEINFO_BASE_H_

#include <co/IAttributeInfo.h>
#include <co/reserved/ComponentBase.h>

namespace co {

//! co.AttributeInfo has a facet named 'attributeInfo', of type co.IAttributeInfo.
class AttributeInfo_co_IAttributeInfo : public co::IAttributeInfo
{
public:
	virtual co::IInterfaceType* getInterfaceType();
	virtual const std::string& getInterfaceName();
};

/*!
	Inherit from this class to implement the component 'co.AttributeInfo'.
 */
class AttributeInfo_Base : public co::ComponentBase,
	public AttributeInfo_co_IAttributeInfo
{
public:
	AttributeInfo_Base();
	virtual ~AttributeInfo_Base();

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

#endif // _CO_ATTRIBUTEINFO_BASE_H_
