/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#ifndef _SYSTEM_H_
#define _SYSTEM_H_

#include "TypeManager.h"
#include "ModuleManager.h"
#include "ServiceManager.h"
#include "System_Base.h"
#include <co/RefPtr.h>

namespace co {

/*!
	Implements co.ISystem.
 */
class System : public System_Base
{
public:
	System();
	virtual ~System();

	// internal methods:
	void initialize();

	// ISystem methods:
	SystemState getState();
	ITypeManager* getTypes();
	IModuleManager* getModules();
	IServiceManager* getServices();
	void setupBase( ArrayRange<std::string const> requiredModules );
	void setupPresentation();
	void tearDown();

private:
	SystemState _state;
	RefPtr<TypeManager> _types;
	RefPtr<ModuleManager> _modules;
	RefPtr<ServiceManager> _services;
};

} // namespace co

#endif
