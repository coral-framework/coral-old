/*
 * Coral - Lightweight C++ Component Framework
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
	virtual co::IInterface* getInterface();
	virtual co::IPort* getFacet();
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

	// co::IService Methods:
	co::IObject* getProvider();
	void serviceRetain();
	void serviceRelease();

	// co::IObject Methods:
	co::IComponent* getComponent();
	co::IService* getServiceAt( co::IPort* );
	void setServiceAt( co::IPort*, co::IService* );
};

} // namespace co

#endif // _CO_TYPEMANAGER_BASE_H_
