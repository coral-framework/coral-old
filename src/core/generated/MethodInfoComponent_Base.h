/*******************************************************************************
** Base class generated for component 'co.MethodInfoComponent'
**
** Created: Mon Jun 28 12:01:36 2010
**      by: Coral Compiler version 0.1.0
**
** WARNING! All changes made in this file will be lost when recompiling!
********************************************************************************/

#ifndef _CO_METHODINFOCOMPONENT_BASE_H_
#define _CO_METHODINFOCOMPONENT_BASE_H_

#include <co/reserved/ComponentBase.h>
#include <co/MethodInfo.h>

namespace co {

//! co.MethodInfoComponent provides an interface named 'methodInfo', of type co.MethodInfo
class MethodInfoComponent_co_MethodInfo : public co::MethodInfo
{
public:
	virtual co::InterfaceType* getInterfaceType();
	virtual const std::string& getInterfaceName();
};

/*!
	Inherit from this class to implement the component 'co.MethodInfoComponent'.
 */
class MethodInfoComponent_Base : public co::ComponentBase,
	public MethodInfoComponent_co_MethodInfo
{
public:
	MethodInfoComponent_Base();
	virtual ~MethodInfoComponent_Base();

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

#endif // _CO_METHODINFOCOMPONENT_BASE_H_
