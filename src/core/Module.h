/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#ifndef _MODULE_H_
#define _MODULE_H_

#include "Namespace.h"
#include "Module_Base.h"
#include <co/RefVector.h>
#include <co/IModulePart.h>

namespace co {

/*!
	Implements co.IModule.
 */
class Module : public Module_Base
{
public:
	Module();
	virtual ~Module();

	// internal methods:
	void initialize( const std::string& moduleName );

	/*!
		Adds a constituent part of this module.

		The module's state must be ModuleState_None when this method is called,
		or a LifeCycleException will be raised.
	 */
	void addPart( IModulePart* part );

	// IModule methods:
	ModuleState getState();
	INamespace* getNamespace();
	Range<IModulePart* const> getParts();
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
	INamespace* _namespace;
	RefVector<IModulePart> _parts;
	int32 _rank;
};

} // namespace co

#endif
