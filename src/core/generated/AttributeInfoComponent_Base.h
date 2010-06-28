/*******************************************************************************
** Base class generated for component 'co.AttributeInfoComponent'
**
** Created: Mon Jun 28 12:01:36 2010
**      by: Coral Compiler version 0.1.0
**
** WARNING! All changes made in this file will be lost when recompiling!
********************************************************************************/

#ifndef _CO_ATTRIBUTEINFOCOMPONENT_BASE_H_
#define _CO_ATTRIBUTEINFOCOMPONENT_BASE_H_

#include <co/reserved/ComponentBase.h>
#include <co/AttributeInfo.h>

namespace co {

//! co.AttributeInfoComponent provides an interface named 'attributeInfo', of type co.AttributeInfo
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
	virtual co::Component* getInterfaceOwner();
	virtual void componentRetain();
	virtual void componentRelease();

	// co::Component Methods:
	virtual co::ComponentType* getComponentType();
	virtual co::Interface* getInterface( co::InterfaceInfo* );
	virtual void bindInterface( co::InterfaceInfo*, co::Interface* );
};

} // namespace co

#endif // _CO_ATTRIBUTEINFOCOMPONENT_BASE_H_
