/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#ifndef _LUAMODULEPART_H_
#define _LUAMODULEPART_H_

#include "ModulePart_Base.h"

namespace lua {

/*!
	A co.IModulePart that wraps a Lua script in a module's dir.
 */
class ModulePart : public ModulePart_Base
{
public:
    ModulePart();
	virtual ~ModulePart();

	// co::IModulePart methods:
	void initialize( co::IModule* module );
	void integrate( co::IModule* module );
	void integratePresentation( co::IModule* module );
	void disintegrate( co::IModule* module );
	void dispose( co::IModule* module );

private:
	void callScriptMethod( const char* methodName, co::IModule* module );
};

} // namespace lua

#endif
