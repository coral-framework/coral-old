/*******************************************************************************
** Base class generated for component 'co.ArrayTypeComponent'
**
** Created: Mon Jun 28 12:01:36 2010
**      by: Coral Compiler version 0.1.0
**
** WARNING! All changes made in this file will be lost when recompiling!
********************************************************************************/

#ifndef _CO_ARRAYTYPECOMPONENT_BASE_H_
#define _CO_ARRAYTYPECOMPONENT_BASE_H_

#include <co/reserved/ComponentBase.h>
#include <co/ArrayType.h>

namespace co {

//! co.ArrayTypeComponent provides an interface named 'type', of type co.ArrayType
class ArrayTypeComponent_co_ArrayType : public co::ArrayType
{
public:
	virtual co::InterfaceType* getInterfaceType();
	virtual const std::string& getInterfaceName();
};

/*!
	Inherit from this class to implement the component 'co.ArrayTypeComponent'.
 */
class ArrayTypeComponent_Base : public co::ComponentBase,
	public ArrayTypeComponent_co_ArrayType
{
public:
	ArrayTypeComponent_Base();
	virtual ~ArrayTypeComponent_Base();

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

#endif // _CO_ARRAYTYPECOMPONENT_BASE_H_
