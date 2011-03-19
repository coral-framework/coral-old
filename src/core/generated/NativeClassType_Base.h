/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#ifndef _CO_NATIVECLASSTYPE_BASE_H_
#define _CO_NATIVECLASSTYPE_BASE_H_

#include <co/INativeClassType.h>
#include <co/reserved/ComponentBase.h>

namespace co {

//! co.NativeClassType has a facet named 'type', of type co.INativeClassType.
class NativeClassType_co_INativeClassType : public co::INativeClassType
{
public:
	virtual co::IInterfaceType* getInterfaceType();
	virtual const std::string& getInterfaceName();
};

/*!
	Inherit from this class to implement the component 'co.NativeClassType'.
 */
class NativeClassType_Base : public co::ComponentBase,
	public NativeClassType_co_INativeClassType
{
public:
	NativeClassType_Base();
	virtual ~NativeClassType_Base();

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

#endif // _CO_NATIVECLASSTYPE_BASE_H_
