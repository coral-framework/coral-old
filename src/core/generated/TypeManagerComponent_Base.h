/*******************************************************************************
** Base class generated for component 'co.TypeManagerComponent'
**
** Created: Fri Jul 02 00:14:15 2010
**      by: Coral Compiler version 0.1.0
**
** WARNING! All changes made in this file will be lost when recompiling!
********************************************************************************/

#ifndef _CO_TYPEMANAGERCOMPONENT_BASE_H_
#define _CO_TYPEMANAGERCOMPONENT_BASE_H_

#include <co/reserved/ComponentBase.h>
#include <co/TypeManager.h>

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
