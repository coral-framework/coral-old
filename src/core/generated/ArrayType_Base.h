/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#ifndef _CO_ARRAYTYPE_BASE_H_
#define _CO_ARRAYTYPE_BASE_H_

#include <co/IArray.h>
#include <co/reserved/ComponentBase.h>

namespace co {

//! co.ArrayType has a facet named 'type', of type co.IArray.
class ArrayType_co_IArray : public co::IArray
{
public:
	virtual co::IInterface* getInterfaceType();
	virtual const std::string& getInterfaceName();
};

/*!
	Inherit from this class to implement the component 'co.ArrayType'.
 */
class ArrayType_Base : public co::ComponentBase,
	public ArrayType_co_IArray
{
public:
	ArrayType_Base();
	virtual ~ArrayType_Base();

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

#endif // _CO_ARRAYTYPE_BASE_H_
