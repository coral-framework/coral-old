/*******************************************************************************
** Base class generated for component 'co.TypeComponent'
**
** Created: Wed Aug 25 16:31:31 2010
**      by: Coral Compiler version 0.1.0
**
** WARNING! All changes made in this file will be lost when recompiling!
********************************************************************************/

#ifndef _CO_TYPECOMPONENT_BASE_H_
#define _CO_TYPECOMPONENT_BASE_H_

#include <co/reserved/ComponentBase.h>
#include <co/Type.h>

namespace co {

//! co.TypeComponent provides an interface named 'type', of type co.Type
class TypeComponent_co_Type : public co::Type
{
public:
	virtual co::InterfaceType* getInterfaceType();
	virtual const std::string& getInterfaceName();
};

/*!
	Inherit from this class to implement the component 'co.TypeComponent'.
 */
class TypeComponent_Base : public co::ComponentBase,
	public TypeComponent_co_Type
{
public:
	TypeComponent_Base();
	virtual ~TypeComponent_Base();

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

#endif // _CO_TYPECOMPONENT_BASE_H_
