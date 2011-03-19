/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#ifndef _CO_METHODINFO_BASE_H_
#define _CO_METHODINFO_BASE_H_

#include <co/IMethodInfo.h>
#include <co/reserved/ComponentBase.h>

namespace co {

//! co.MethodInfo has a facet named 'methodInfo', of type co.IMethodInfo.
class MethodInfo_co_IMethodInfo : public co::IMethodInfo
{
public:
	virtual co::IInterfaceType* getInterfaceType();
	virtual const std::string& getInterfaceName();
};

/*!
	Inherit from this class to implement the component 'co.MethodInfo'.
 */
class MethodInfo_Base : public co::ComponentBase,
	public MethodInfo_co_IMethodInfo
{
public:
	MethodInfo_Base();
	virtual ~MethodInfo_Base();

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

#endif // _CO_METHODINFO_BASE_H_
