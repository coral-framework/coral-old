/*
 * Coral - A C++ Component Framework.
 * See Copyright Notice in Coral.h
 */

#ifndef _CO_STRUCTTYPECOMPONENT_BASE_H_
#define _CO_STRUCTTYPECOMPONENT_BASE_H_

#include <co/StructType.h>
#include <co/reserved/ComponentBase.h>

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
