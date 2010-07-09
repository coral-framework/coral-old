/*******************************************************************************
** Base class generated for component 'co.InterfaceTypeComponent'
**
** Created: Fri Jul 02 00:14:15 2010
**      by: Coral Compiler version 0.1.0
**
** WARNING! All changes made in this file will be lost when recompiling!
********************************************************************************/

#ifndef _CO_INTERFACETYPECOMPONENT_BASE_H_
#define _CO_INTERFACETYPECOMPONENT_BASE_H_

#include <co/reserved/ComponentBase.h>
#include <co/InterfaceType.h>

namespace co {

//! co.InterfaceTypeComponent provides an interface named 'type', of type co.InterfaceType
class InterfaceTypeComponent_co_InterfaceType : public co::InterfaceType
{
public:
	virtual co::InterfaceType* getInterfaceType();
	virtual const std::string& getInterfaceName();
};

/*!
	Inherit from this class to implement the component 'co.InterfaceTypeComponent'.
 */
class InterfaceTypeComponent_Base : public co::ComponentBase,
	public InterfaceTypeComponent_co_InterfaceType
{
public:
	InterfaceTypeComponent_Base();
	virtual ~InterfaceTypeComponent_Base();

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

#endif // _CO_INTERFACETYPECOMPONENT_BASE_H_
