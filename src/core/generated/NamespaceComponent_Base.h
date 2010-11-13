/*
 * Coral - Lightweight C++ Component Framework.
 * See Copyright Notice in Coral.h
 */

#ifndef _CO_NAMESPACECOMPONENT_BASE_H_
#define _CO_NAMESPACECOMPONENT_BASE_H_

#include <co/Namespace.h>
#include <co/reserved/ComponentBase.h>

namespace co {

//! co.NamespaceComponent has a facet named 'namespace', of type co.Namespace.
class NamespaceComponent_co_Namespace : public co::Namespace
{
public:
	virtual co::InterfaceType* getInterfaceType();
	virtual const std::string& getInterfaceName();
};

/*!
	Inherit from this class to implement the component 'co.NamespaceComponent'.
 */
class NamespaceComponent_Base : public co::ComponentBase,
	public NamespaceComponent_co_Namespace
{
public:
	NamespaceComponent_Base();
	virtual ~NamespaceComponent_Base();

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

#endif // _CO_NAMESPACECOMPONENT_BASE_H_
