/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#ifndef _CO_TYPEBUILDERCOMPONENT_BASE_H_
#define _CO_TYPEBUILDERCOMPONENT_BASE_H_

#include <co/TypeBuilder.h>
#include <co/reserved/ComponentBase.h>

namespace co {

//! co.TypeBuilderComponent has a facet named 'typeBuilder', of type co.TypeBuilder.
class TypeBuilderComponent_co_TypeBuilder : public co::TypeBuilder
{
public:
	virtual co::InterfaceType* getInterfaceType();
	virtual const std::string& getInterfaceName();
};

/*!
	Inherit from this class to implement the component 'co.TypeBuilderComponent'.
 */
class TypeBuilderComponent_Base : public co::ComponentBase,
	public TypeBuilderComponent_co_TypeBuilder
{
public:
	TypeBuilderComponent_Base();
	virtual ~TypeBuilderComponent_Base();

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

#endif // _CO_TYPEBUILDERCOMPONENT_BASE_H_
