/*
 * Coral - A C++ Component Framework.
 * See Copyright Notice in Coral.h
 */

#ifndef _CO_ARRAYTYPECOMPONENT_BASE_H_
#define _CO_ARRAYTYPECOMPONENT_BASE_H_

#include <co/ArrayType.h>
#include <co/reserved/ComponentBase.h>

namespace co {

//! co.ArrayTypeComponent provides an interface named 'type', of type co.ArrayType.
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
	co::Component* getInterfaceOwner();
	void componentRetain();
	void componentRelease();

	// co::Component Methods:
	co::ComponentType* getComponentType();
	co::Interface* getInterface( co::InterfaceInfo* );
	void bindInterface( co::InterfaceInfo*, co::Interface* );
};

} // namespace co

#endif // _CO_ARRAYTYPECOMPONENT_BASE_H_
