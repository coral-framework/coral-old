/*******************************************************************************
** ModuleInstaller class generated for module 'lua'
**
** Created: Mon Sep 20 12:13:52 2010
**      by: Coral Compiler version 0.3.0
**
** WARNING! All changes made in this file will be lost when recompiling!
********************************************************************************/

#include <co/RefPtr.h>
#include <co/Reflector.h>
#include <co/ModulePart.h>

namespace lua {

class ModuleInstaller
{
public:
	inline static ModuleInstaller& instance() { return sm_instance; }

	enum TypeId
	{
		TypeId_Exception,
		TypeId_ILauncher,
		TypeId_IState,
		TypeId_Launcher,
		TypeId_lua,
		TypeId_ModulePart,
		TypeId_ModulePartLoader,
		TypeId_Universe,
		TypeCount
	};

public:
	//! Instantiates Reflectors for the module types that still don't have an assigned Reflector.
	void initialize();

	//! Returns the Reflector that should be assigned to a module type.
	inline co::Reflector* getReflector( TypeId typeId ) { return _reflectors[typeId].get(); }

	//! Sets the Reflector instance that should be assigned to module type.
	//! \warning This method can only be called \b before calling install().
	inline void setReflector( TypeId typeId, co::Reflector* reflector ) { _reflectors[typeId] = reflector; }

	//! Assigns the Reflectors to their corresponding co::Type instances.
	//! This method skips any co::Type instance that already has an assigned Reflector.
	void install();

	//! Removes references to the Reflectors from their corresponding co::Type instances.
	//! This ignores any co::Type instance whose current Reflector was not assigned by this class.
	void uninstall();

private:
	ModuleInstaller();
	~ModuleInstaller();

	co::Reflector* createReflector( TypeId typeId );

private:
	static ModuleInstaller sm_instance;

private:
	co::RefPtr<co::Reflector> _reflectors[TypeCount];
};

// Module's reflector creation functions:
co::Reflector* __createExceptionReflector();
co::Reflector* __createILauncherReflector();
co::Reflector* __createIStateReflector();
co::Reflector* __createLauncherReflector();
co::Reflector* __createluaReflector();
co::Reflector* __createModulePartReflector();
co::Reflector* __createModulePartLoaderReflector();
co::Reflector* __createUniverseReflector();

} // namespace lua
