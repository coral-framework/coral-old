/*
 * Coral - A C++ Component Framework.
 * See Copyright Notice in Coral.h
 */

#ifndef _CO_METHODBUILDERCOMPONENT_BASE_H_
#define _CO_METHODBUILDERCOMPONENT_BASE_H_

#include <co/MethodBuilder.h>
#include <co/reserved/ComponentBase.h>

namespace co {

//! co.MethodBuilderComponent has a facet named 'methodBuilder', of type co.MethodBuilder.
class MethodBuilderComponent_co_MethodBuilder : public co::MethodBuilder
{
public:
	virtual co::InterfaceType* getInterfaceType();
	virtual const std::string& getInterfaceName();
};

/*!
	Inherit from this class to implement the component 'co.MethodBuilderComponent'.
 */
class MethodBuilderComponent_Base : public co::ComponentBase,
	public MethodBuilderComponent_co_MethodBuilder
{
public:
	MethodBuilderComponent_Base();
	virtual ~MethodBuilderComponent_Base();

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

#endif // _CO_METHODBUILDERCOMPONENT_BASE_H_
