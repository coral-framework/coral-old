/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#ifndef _CO_NATIVECLASS_BASE_H_
#define _CO_NATIVECLASS_BASE_H_

#include <co/INativeClass.h>
#include <co/reserved/ComponentBase.h>

namespace co {

//! co.NativeClass has a facet named 'type', of type co.INativeClass.
class NativeClass_co_INativeClass : public co::INativeClass
{
public:
	virtual co::IInterface* getInterfaceType();
	virtual const std::string& getInterfaceName();
};

/*!
	Inherit from this class to implement the component 'co.NativeClass'.
 */
class NativeClass_Base : public co::ComponentBase,
	public NativeClass_co_INativeClass
{
public:
	NativeClass_Base();
	virtual ~NativeClass_Base();

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

#endif // _CO_NATIVECLASS_BASE_H_
