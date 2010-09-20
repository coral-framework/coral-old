/*
 * Coral - A C++ Component Framework.
 * See Copyright Notice in Coral.h
 */

#ifndef _CO_METHODINFOCOMPONENT_BASE_H_
#define _CO_METHODINFOCOMPONENT_BASE_H_

#include <co/MethodInfo.h>
#include <co/reserved/ComponentBase.h>

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
