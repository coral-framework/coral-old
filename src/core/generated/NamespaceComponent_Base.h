/*******************************************************************************
** Base class generated for component 'co.NamespaceComponent'
**
** Created: Fri Jul 02 00:14:15 2010
**      by: Coral Compiler version 0.1.0
**
** WARNING! All changes made in this file will be lost when recompiling!
********************************************************************************/

#ifndef _CO_NAMESPACECOMPONENT_BASE_H_
#define _CO_NAMESPACECOMPONENT_BASE_H_

#include <co/reserved/ComponentBase.h>
#include <co/Namespace.h>

namespace co {

//! co.NamespaceComponent provides an interface named 'namespace', of type co.Namespace
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
	virtual co::Component* getInterfaceOwner();
	virtual void componentRetain();
	virtual void componentRelease();

	// co::Component Methods:
	virtual co::ComponentType* getComponentType();
	virtual co::Interface* getInterface( co::InterfaceInfo* );
	virtual void bindInterface( co::InterfaceInfo*, co::Interface* );
};

} // namespace co

#endif // _CO_NAMESPACECOMPONENT_BASE_H_
