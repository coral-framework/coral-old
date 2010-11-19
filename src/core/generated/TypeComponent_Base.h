/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#ifndef _CO_TYPECOMPONENT_BASE_H_
#define _CO_TYPECOMPONENT_BASE_H_

#include <co/Type.h>
#include <co/reserved/ComponentBase.h>

namespace co {

//! co.TypeComponent has a facet named 'type', of type co.Type.
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
	co::Component* getInterfaceOwner();
	void componentRetain();
	void componentRelease();

	// co::Component Methods:
	co::ComponentType* getComponentType();
	co::Interface* getInterface( co::InterfaceInfo* );
	void bindInterface( co::InterfaceInfo*, co::Interface* );
};

} // namespace co

#endif // _CO_TYPECOMPONENT_BASE_H_
