/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#ifndef _CO_STRUCT_BASE_H_
#define _CO_STRUCT_BASE_H_

#include <co/IStruct.h>
#include <co/reserved/ComponentBase.h>

namespace co {

//! co.Struct has a facet named 'type', of type co.IStruct.
class Struct_co_IStruct : public co::IStruct
{
public:
	virtual co::IInterface* getInterfaceType();
	virtual const std::string& getInterfaceName();
};

/*!
	Inherit from this class to implement the component 'co.Struct'.
 */
class Struct_Base : public co::ComponentBase,
	public Struct_co_IStruct
{
public:
	Struct_Base();
	virtual ~Struct_Base();

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

#endif // _CO_STRUCT_BASE_H_
