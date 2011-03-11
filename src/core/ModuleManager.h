/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#ifndef _MODULEMANAGER_H_
#define _MODULEMANAGER_H_

#include "ModuleManagerComponent_Base.h"
#include <co/Module.h>
#include <co/ModulePartLoader.h>
#include <co/RefVector.h>

namespace co {

// forward declarations:
class ModuleComponent;

/*!
	Component that implements co.ModuleManager.
 */
class ModuleManagerComponent : public ModuleManagerComponent_Base
{
public:
	ModuleManagerComponent();
	virtual ~ModuleManagerComponent();

	// internal methods:
	void initialize();
	void updateModules( ModuleState toState );

	// ModuleManager methods:
	ArrayRange<Module* const> getModules();
	ArrayRange<ModulePartLoader* const> getLoaders();
	bool getBinaryCompatibilityChecking();
	void setBinaryCompatibilityChecking( bool enabled );
	Module* findModule( const std::string& moduleName );
	void installLoader( ModulePartLoader* loader );
	void uninstallLoader( ModulePartLoader* loader );
	bool isLoadable( const std::string& moduleName );
	Module* load( const std::string& moduleName );

private:
	ModuleComponent* createModule( const std::string& moduleName );
	void updateModule( Module* module, ModuleState toState );
	void verifyModuleIntegrity( Module* module );
	void syncModuleWithSystemState( Module* module );

private:
	typedef RefVector<Module> ModuleList;
	ModuleList _modules;

	typedef RefVector<ModulePartLoader> ModulePartLoaderList;
	ModulePartLoaderList _loaders;
	
	bool _binaryCompatibilityChecking;
};

} // namespace co

#endif
