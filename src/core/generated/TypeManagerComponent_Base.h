/*
 * Coral - A C++ Component Framework.
 * See Copyright Notice in Coral.h
 */

#ifndef _CO_TYPEMANAGERCOMPONENT_BASE_H_
#define _CO_TYPEMANAGERCOMPONENT_BASE_H_

#include <co/TypeManager.h>
#include <co/reserved/ComponentBase.h>

namespace co {

//! co.TypeManagerComponent provides an interface named 'typeManager', of type co.TypeManager
class TypeManagerComponent_co_TypeManager : public co::TypeManager
{
public:
	virtual co::InterfaceType* getInterfaceType();
	virtual const std::string& getInterfaceName();
};

/*!
	Inherit from this class to implement the component 'co.TypeManagerComponent'.
 */
class TypeManagerComponent_Base : public co::ComponentBase,
	public TypeManagerComponent_co_TypeManager
{
public:
	TypeManagerComponent_Base();
	virtual ~TypeManagerComponent_Base();

	// co::Interface Methods:
	virtual co::Component* getInterfaceOwner();
	virtual void componentRetain();
	virtual void componentRelease();

	// co::Component Methods:
	virtual co::ComponentType* getComponentType();
	virtual co::Interface* getInterface( co::InterfaceInfo* );
	virtual void bindInterface( co::InterfaceInfo*, co::Interface* );
};

} // namespace co

#endif // _CO_TYPEMANAGERCOMPONENT_BASE_H_
