/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#ifndef _CO_ENUMTYPECOMPONENT_BASE_H_
#define _CO_ENUMTYPECOMPONENT_BASE_H_

#include <co/EnumType.h>
#include <co/reserved/ComponentBase.h>

namespace co {

//! co.EnumTypeComponent has a facet named 'type', of type co.EnumType.
class EnumTypeComponent_co_EnumType : public co::EnumType
{
public:
	virtual co::InterfaceType* getInterfaceType();
	virtual const std::string& getInterfaceName();
};

/*!
	Inherit from this class to implement the component 'co.EnumTypeComponent'.
 */
class EnumTypeComponent_Base : public co::ComponentBase,
	public EnumTypeComponent_co_EnumType
{
public:
	EnumTypeComponent_Base();
	virtual ~EnumTypeComponent_Base();

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

#endif // _CO_ENUMTYPECOMPONENT_BASE_H_
