/*******************************************************************************
** Base class generated for component 'co.StructTypeComponent'
**
** Created: Mon Jun 28 12:01:36 2010
**      by: Coral Compiler version 0.1.0
**
** WARNING! All changes made in this file will be lost when recompiling!
********************************************************************************/

#ifndef _CO_STRUCTTYPECOMPONENT_BASE_H_
#define _CO_STRUCTTYPECOMPONENT_BASE_H_

#include <co/reserved/ComponentBase.h>
#include <co/StructType.h>

namespace co {

//! co.StructTypeComponent provides an interface named 'type', of type co.StructType
class StructTypeComponent_co_StructType : public co::StructType
{
public:
	virtual co::InterfaceType* getInterfaceType();
	virtual const std::string& getInterfaceName();
};

/*!
	Inherit from this class to implement the component 'co.StructTypeComponent'.
 */
class StructTypeComponent_Base : public co::ComponentBase,
	public StructTypeComponent_co_StructType
{
public:
	StructTypeComponent_Base();
	virtual ~StructTypeComponent_Base();

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

#endif // _CO_STRUCTTYPECOMPONENT_BASE_H_
