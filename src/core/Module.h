/*
 * Coral - A C++ Component Framework.
 * See Copyright Notice in Coral.h
 */

#ifndef _MODULE_H_
#define _MODULE_H_

#include "Namespace.h"
#include "ModuleComponent_Base.h"
#include <co/RefVector.h>
#include <co/ModulePart.h>
#include <co/ModulePartLoader.h>

/*!
	Component that implements co.Module.
 */
class Module : public co::ModuleComponent_Base
{
public:
	Module();
	virtual ~Module();

	// internal methods:
	void initialize( const std::string& moduleName );

	/*!
		Adds a ModulePart as a constituent part of this Module.
		The ModulePart's loader is also registered for later use, to unload the parts.

		The module's state must be ModuleState_None when this method is called,
		or a co::LifeCycleException will be raised.
	 */
	void addPart( co::ModulePart* part, co::ModulePartLoader* loader );

	// co::Module methods:
	co::ModuleState getState();
	co::Namespace* getNamespace();
	co::ArrayRange<co::ModulePart* const> getParts();
	void initialize();
	void integrate();
	void integratePresentation();
	void disintegrate();
	void dispose();
	void abort();

private:
	void unloadParts();

private:
	co::ModuleState _state;
	co::Namespace* _namespace;
	co::RefVector<co::ModulePart> _parts;
	co::RefVector<co::ModulePartLoader> _loaders;
};

#endif
