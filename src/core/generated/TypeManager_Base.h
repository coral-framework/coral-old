/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#ifndef _CO_TYPEMANAGER_BASE_H_
#define _CO_TYPEMANAGER_BASE_H_

#include <co/ITypeManager.h>
#include <co/reserved/ComponentBase.h>

namespace co {

//! co.TypeManager has a facet named 'typeManager', of type co.ITypeManager.
class TypeManager_co_ITypeManager : public co::ITypeManager
{
public:
	virtual co::IInterfaceType* getInterfaceType();
	virtual const std::string& getInterfaceName();
};

/*!
	Inherit from this class to implement the component 'co.TypeManager'.
 */
class TypeManager_Base : public co::ComponentBase,
	public TypeManager_co_ITypeManager
{
public:
	TypeManager_Base();
	virtual ~TypeManager_Base();

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

#endif // _CO_TYPEMANAGER_BASE_H_
