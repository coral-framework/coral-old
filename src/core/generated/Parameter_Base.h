/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#ifndef _CO_PARAMETER_BASE_H_
#define _CO_PARAMETER_BASE_H_

#include <co/IParameter.h>
#include <co/reserved/ComponentBase.h>

namespace co {

//! co.Parameter has a facet named 'parameter', of type co.IParameter.
class Parameter_co_IParameter : public co::IParameter
{
public:
	virtual co::IInterface* getInterfaceType();
	virtual const std::string& getInterfaceName();
};

/*!
	Inherit from this class to implement the component 'co.Parameter'.
 */
class Parameter_Base : public co::ComponentBase,
	public Parameter_co_IParameter
{
public:
	Parameter_Base();
	virtual ~Parameter_Base();

	// co::IService Methods:
	co::IObject* getInterfaceOwner();
	void componentRetain();
	void componentRelease();

	// co::IObject Methods:
	co::IComponent* getComponentType();
	co::IService* getInterface( co::IPort* );
	void setReceptacle( co::IPort*, co::IService* );
};

} // namespace co

#endif // _CO_PARAMETER_BASE_H_
