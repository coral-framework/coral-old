/*
 * Coral - Lightweight C++ Component Framework.
 * See Copyright Notice in Coral.h
 */

#ifndef _MODULEMANAGER_H_
#define _MODULEMANAGER_H_

#include "ModuleManagerComponent_Base.h"
#include <co/Module.h>
#include <co/ModulePartLoader.h>
#include <co/RefVector.h>

// forward declarations:
class Module;

/*!
	Component that implements co.ModuleManager.
 */
class ModuleManager : public co::ModuleManagerComponent_Base
{
public:
	ModuleManager();
	virtual ~ModuleManager();

	// internal methods:
	void initialize();
	void updateModules( co::ModuleState toState );

	// co::ModuleManager methods:
	co::ArrayRange<co::Module* const> getModules();
	co::ArrayRange<co::ModulePartLoader* const> getLoaders();
	bool getBinaryCompatibilityChecking();
	void setBinaryCompatibilityChecking( bool enabled );
	co::Module* findModule( const std::string& moduleName );
	void installLoader( co::ModulePartLoader* loader );
	void uninstallLoader( co::ModulePartLoader* loader );
	bool isLoadable( const std::string& moduleName );
	co::Module* load( const std::string& moduleName );

private:
	Module* createModule( const std::string& moduleName );
	void updateModule( co::Module* module, co::ModuleState toState );
	void verifyModuleIntegrity( co::Module* module );
	void syncModuleWithSystemState( co::Module* module );

private:
	typedef co::RefVector<co::Module> ModuleList;
	ModuleList _modules;

	typedef co::RefVector<co::ModulePartLoader> ModulePartLoaderList;
	ModulePartLoaderList _loaders;
	
	bool _binaryCompatibilityChecking;
};

#endif
