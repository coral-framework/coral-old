/*******************************************************************************
** Base class generated for component 'co.MethodBuilderComponent'
**
** Created: Fri Jul 02 00:14:15 2010
**      by: Coral Compiler version 0.1.0
**
** WARNING! All changes made in this file will be lost when recompiling!
********************************************************************************/

#ifndef _CO_METHODBUILDERCOMPONENT_BASE_H_
#define _CO_METHODBUILDERCOMPONENT_BASE_H_

#include <co/reserved/ComponentBase.h>
#include <co/MethodBuilder.h>

namespace co {

//! co.MethodBuilderComponent provides an interface named 'methodBuilder', of type co.MethodBuilder
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
	virtual co::Component* getInterfaceOwner();
	virtual void componentRetain();
	virtual void componentRelease();

	// co::Component Methods:
	virtual co::ComponentType* getComponentType();
	virtual co::Interface* getInterface( co::InterfaceInfo* );
	virtual void bindInterface( co::InterfaceInfo*, co::Interface* );
};

} // namespace co

#endif // _CO_METHODBUILDERCOMPONENT_BASE_H_
