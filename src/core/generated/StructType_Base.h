/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#ifndef _CO_STRUCTTYPE_BASE_H_
#define _CO_STRUCTTYPE_BASE_H_

#include <co/IStructType.h>
#include <co/reserved/ComponentBase.h>

namespace co {

//! co.StructType has a facet named 'type', of type co.IStructType.
class StructType_co_IStructType : public co::IStructType
{
public:
	virtual co::IInterfaceType* getInterfaceType();
	virtual const std::string& getInterfaceName();
};

/*!
	Inherit from this class to implement the component 'co.StructType'.
 */
class StructType_Base : public co::ComponentBase,
	public StructType_co_IStructType
{
public:
	StructType_Base();
	virtual ~StructType_Base();

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

#endif // _CO_STRUCTTYPE_BASE_H_
