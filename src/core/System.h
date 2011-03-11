/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#ifndef _SYSTEM_H_
#define _SYSTEM_H_

#include "TypeManager.h"
#include "ModuleManager.h"
#include "ServiceManager.h"
#include "SystemComponent_Base.h"
#include <co/RefPtr.h>

namespace co {

/*!
	Component that implements co.System.
 */
class SystemComponent : public SystemComponent_Base
{
public:
	SystemComponent();
	virtual ~SystemComponent();

	// internal methods:
	void initialize();

	// System methods:
	SystemState getState();
	TypeManager* getTypes();
	ModuleManager* getModules();
	ServiceManager* getServices();
	void setupBase( ArrayRange<std::string const> requiredModules );
	void setupPresentation();
	void tearDown();

private:
	SystemState _state;
	RefPtr<TypeManagerComponent> _types;
	RefPtr<ModuleManagerComponent> _modules;
	RefPtr<ServiceManagerComponent> _services;
};

} // namespace co

#endif
