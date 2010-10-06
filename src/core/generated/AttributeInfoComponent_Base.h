/*
 * Coral - A C++ Component Framework.
 * See Copyright Notice in Coral.h
 */

#ifndef _CO_ATTRIBUTEINFOCOMPONENT_BASE_H_
#define _CO_ATTRIBUTEINFOCOMPONENT_BASE_H_

#include <co/AttributeInfo.h>
#include <co/reserved/ComponentBase.h>

namespace co {

//! co.AttributeInfoComponent provides an interface named 'attributeInfo', of type co.AttributeInfo.
class AttributeInfoComponent_co_AttributeInfo : public co::AttributeInfo
{
public:
	virtual co::InterfaceType* getInterfaceType();
	virtual const std::string& getInterfaceName();
};

/*!
	Inherit from this class to implement the component 'co.AttributeInfoComponent'.
 */
class AttributeInfoComponent_Base : public co::ComponentBase,
	public AttributeInfoComponent_co_AttributeInfo
{
public:
	AttributeInfoComponent_Base();
	virtual ~AttributeInfoComponent_Base();

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

#endif // _CO_ATTRIBUTEINFOCOMPONENT_BASE_H_
