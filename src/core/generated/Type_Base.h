/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#ifndef _CO_TYPE_BASE_H_
#define _CO_TYPE_BASE_H_

#include <co/IType.h>
#include <co/reserved/ComponentBase.h>

namespace co {

//! co.Type has a facet named 'type', of type co.IType.
class Type_co_IType : public co::IType
{
public:
	virtual co::IInterface* getInterfaceType();
	virtual const std::string& getInterfaceName();
};

/*!
	Inherit from this class to implement the component 'co.Type'.
 */
class Type_Base : public co::ComponentBase,
	public Type_co_IType
{
public:
	Type_Base();
	virtual ~Type_Base();

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

#endif // _CO_TYPE_BASE_H_
