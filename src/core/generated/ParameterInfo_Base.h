/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#ifndef _CO_PARAMETERINFO_BASE_H_
#define _CO_PARAMETERINFO_BASE_H_

#include <co/IParameterInfo.h>
#include <co/reserved/ComponentBase.h>

namespace co {

//! co.ParameterInfo has a facet named 'parameterInfo', of type co.IParameterInfo.
class ParameterInfo_co_IParameterInfo : public co::IParameterInfo
{
public:
	virtual co::IInterfaceType* getInterfaceType();
	virtual const std::string& getInterfaceName();
};

/*!
	Inherit from this class to implement the component 'co.ParameterInfo'.
 */
class ParameterInfo_Base : public co::ComponentBase,
	public ParameterInfo_co_IParameterInfo
{
public:
	ParameterInfo_Base();
	virtual ~ParameterInfo_Base();

	// co::Interface Methods:
	co::IComponent* getInterfaceOwner();
	void componentRetain();
	void componentRelease();

	// co::IComponent Methods:
	co::IComponentType* getComponentType();
	co::Interface* getInterface( co::IInterfaceInfo* );
	void setReceptacle( co::IInterfaceInfo*, co::Interface* );
};

} // namespace co

#endif // _CO_PARAMETERINFO_BASE_H_
