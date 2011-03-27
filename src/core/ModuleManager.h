/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#ifndef _MODULEMANAGER_H_
#define _MODULEMANAGER_H_

#include "ModuleManager_Base.h"
#include <co/IModule.h>
#include <co/IModulePartLoader.h>
#include <co/RefVector.h>

namespace co {

// forward declarations:
class Module;

/*!
	Implements co.IModuleManager.
 */
class ModuleManager : public ModuleManager_Base
{
public:
	ModuleManager();
	virtual ~ModuleManager();

	// internal methods:
	void initialize();
	void updateModules( ModuleState toState );

	// IModuleManager methods:
	Range<IModule* const> getModules();
	Range<IModulePartLoader* const> getLoaders();
	bool getBinaryCompatibilityChecking();
	void setBinaryCompatibilityChecking( bool enabled );
	IModule* findModule( const std::string& moduleName );
	void installLoader( IModulePartLoader* loader );
	void uninstallLoader( IModulePartLoader* loader );
	bool isLoadable( const std::string& moduleName );
	IModule* load( const std::string& moduleName );

private:
	Module* createModule( const std::string& moduleName );
	void updateModule( IModule* module, ModuleState toState );
	void verifyModuleIntegrity( IModule* module );
	void syncModuleWithSystemState( IModule* module );

private:
	typedef RefVector<IModule> ModuleList;
	ModuleList _modules;

	typedef RefVector<IModulePartLoader> ModulePartLoaderList;
	ModulePartLoaderList _loaders;
	
	bool _binaryCompatibilityChecking;
};

} // namespace co

#endif
