/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#ifndef _MODULE_H_
#define _MODULE_H_

#include "Namespace.h"
#include "ModuleComponent_Base.h"
#include <co/RefVector.h>
#include <co/ModulePart.h>

namespace co {

/*!
	Component that implements co.Module.
 */
class ModuleComponent : public ModuleComponent_Base
{
public:
	ModuleComponent();
	virtual ~ModuleComponent();

	// internal methods:
	void initialize( const std::string& moduleName );

	/*!
		Adds a ModulePart as a constituent part of this Module.

		The module's state must be ModuleState_None when this method is called,
		or a LifeCycleException will be raised.
	 */
	void addPart( ModulePart* part );

	// Module methods:
	ModuleState getState();
	Namespace* getNamespace();
	ArrayRange<ModulePart* const> getParts();
	int32 getRank();
	void setRank( int32 rank );
	void initialize();
	void integrate();
	void integratePresentation();
	void disintegrate();
	void dispose();
	void abort();

private:
	ModuleState _state;
	Namespace* _namespace;
	RefVector<ModulePart> _parts;
	int32 _rank;
};

} // namespace co

#endif
