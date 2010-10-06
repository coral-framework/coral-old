/*
 * Coral - A C++ Component Framework.
 * See Copyright Notice in Coral.h
 */

#ifndef _CO_SYSTEMCOMPONENT_BASE_H_
#define _CO_SYSTEMCOMPONENT_BASE_H_

#include <co/System.h>
#include <co/reserved/ComponentBase.h>

namespace co {

//! co.SystemComponent provides an interface named 'system', of type co.System.
class SystemComponent_co_System : public co::System
{
public:
	virtual co::InterfaceType* getInterfaceType();
	virtual const std::string& getInterfaceName();
};

/*!
	Inherit from this class to implement the component 'co.SystemComponent'.
 */
class SystemComponent_Base : public co::ComponentBase,
	public SystemComponent_co_System
{
public:
	SystemComponent_Base();
	virtual ~SystemComponent_Base();

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

#endif // _CO_SYSTEMCOMPONENT_BASE_H_
