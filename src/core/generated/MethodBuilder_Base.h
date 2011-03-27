/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#ifndef _CO_METHODBUILDER_BASE_H_
#define _CO_METHODBUILDER_BASE_H_

#include <co/IMethodBuilder.h>
#include <co/reserved/ComponentBase.h>

namespace co {

//! co.MethodBuilder has a facet named 'methodBuilder', of type co.IMethodBuilder.
class MethodBuilder_co_IMethodBuilder : public co::IMethodBuilder
{
public:
	virtual co::IInterface* getInterfaceType();
	virtual const std::string& getInterfaceName();
};

/*!
	Inherit from this class to implement the component 'co.MethodBuilder'.
 */
class MethodBuilder_Base : public co::ComponentBase,
	public MethodBuilder_co_IMethodBuilder
{
public:
	MethodBuilder_Base();
	virtual ~MethodBuilder_Base();

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

#endif // _CO_METHODBUILDER_BASE_H_
