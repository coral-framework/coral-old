/*
 * Coral - A C++ Component Framework.
 * See Copyright Notice in Coral.h
 */

#ifndef _CO_PARAMETERINFOCOMPONENT_BASE_H_
#define _CO_PARAMETERINFOCOMPONENT_BASE_H_

#include <co/ParameterInfo.h>
#include <co/reserved/ComponentBase.h>

namespace co {

//! co.ParameterInfoComponent provides an interface named 'parameterInfo', of type co.ParameterInfo
class ParameterInfoComponent_co_ParameterInfo : public co::ParameterInfo
{
public:
	virtual co::InterfaceType* getInterfaceType();
	virtual const std::string& getInterfaceName();
};

/*!
	Inherit from this class to implement the component 'co.ParameterInfoComponent'.
 */
class ParameterInfoComponent_Base : public co::ComponentBase,
	public ParameterInfoComponent_co_ParameterInfo
{
public:
	ParameterInfoComponent_Base();
	virtual ~ParameterInfoComponent_Base();

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

#endif // _CO_PARAMETERINFOCOMPONENT_BASE_H_
