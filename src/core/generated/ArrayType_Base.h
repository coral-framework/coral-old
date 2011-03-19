/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#ifndef _CO_ARRAYTYPE_BASE_H_
#define _CO_ARRAYTYPE_BASE_H_

#include <co/IArrayType.h>
#include <co/reserved/ComponentBase.h>

namespace co {

//! co.ArrayType has a facet named 'type', of type co.IArrayType.
class ArrayType_co_IArrayType : public co::IArrayType
{
public:
	virtual co::IInterfaceType* getInterfaceType();
	virtual const std::string& getInterfaceName();
};

/*!
	Inherit from this class to implement the component 'co.ArrayType'.
 */
class ArrayType_Base : public co::ComponentBase,
	public ArrayType_co_IArrayType
{
public:
	ArrayType_Base();
	virtual ~ArrayType_Base();

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

#endif // _CO_ARRAYTYPE_BASE_H_
