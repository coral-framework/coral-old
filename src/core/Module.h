/*
 * Coral - Lightweight C++ Component Framework
 * See Copyright Notice in Coral.h
 */

#ifndef _MODULE_H_
#define _MODULE_H_

#include "Namespace.h"
#include "Module_Base.h"
#include <co/RefVector.h>
#include <co/IModulePart.h>

namespace co {

// Maps co::ModuleState identifiers to readable strings.
extern const char* MODULE_STATE_STRINGS[];

/*!
	Implements co.IModule.
 */
class Module : public Module_Base
{
public:
	Module();
	virtual ~Module();

	// internal methods:
	void setName( const std::string& moduleName );

	/*!
		Adds a \a modulePart to this module.

		The module's state must be ModuleState_None when this method is called,
		or a IllegalStateException will be raised.
	 */
	void addPart( IModulePart* modulePart );

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
