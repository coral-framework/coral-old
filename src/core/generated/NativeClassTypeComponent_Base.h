/*
 * Coral - A C++ Component Framework.
 * See Copyright Notice in Coral.h
 */

#ifndef _CO_NATIVECLASSTYPECOMPONENT_BASE_H_
#define _CO_NATIVECLASSTYPECOMPONENT_BASE_H_

#include <co/NativeClassType.h>
#include <co/reserved/ComponentBase.h>

namespace co {

//! co.NativeClassTypeComponent provides an interface named 'type', of type co.NativeClassType.
class NativeClassTypeComponent_co_NativeClassType : public co::NativeClassType
{
public:
	virtual co::InterfaceType* getInterfaceType();
	virtual const std::string& getInterfaceName();
};

/*!
	Inherit from this class to implement the component 'co.NativeClassTypeComponent'.
 */
class NativeClassTypeComponent_Base : public co::ComponentBase,
	public NativeClassTypeComponent_co_NativeClassType
{
public:
	NativeClassTypeComponent_Base();
	virtual ~NativeClassTypeComponent_Base();

	// co::Interface Methods:
	co::Component* getInterfaceOwner();
	void componentRetain();
	void componentRelease();

	// co::Component Methods:
	co::ComponentType* getComponentType();
	co::Interface* getInterface( co::InterfaceInfo* );
	void bindInterface( co::InterfaceInfo*, co::Interface* );
};

} // namespace co

#endif // _CO_NATIVECLASSTYPECOMPONENT_BASE_H_
