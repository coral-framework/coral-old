/*
 * Coral - A C++ Component Framework.
 * See Copyright Notice in Coral.h
 */

#ifndef _SYSTEM_H_
#define _SYSTEM_H_

#include "TypeManager.h"
#include "ModuleManager.h"
#include "SystemComponent_Base.h"
#include <co/RefPtr.h>

/*!
	Component that implements co.System.
 */
class System : public co::SystemComponent_Base
{
public:
	System();
	virtual ~System();

	// internal methods:
	void initialize();

	// co::System methods:
	co::SystemState getState();
	co::TypeManager* getTypes();
	co::ModuleManager* getModules();
	void setupBase( co::ArrayRange<std::string const> requiredModules );
	void setupPresentation();
	void tearDown();

private:
	co::SystemState _state;
	co::RefPtr<TypeManager> _types;
	co::RefPtr<ModuleManager> _modules;
};

#endif
