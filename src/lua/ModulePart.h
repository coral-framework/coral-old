/*
 * Coral - A C++ Component Framework.
 * See Copyright Notice in Coral.h
 */

#ifndef _MODULEPART_H_
#define _MODULEPART_H_

#include "lua_Base.h"
#include "LuaModulePartLoader.h"
#include <co/RefPtr.h>

/*!
	The Lua module's co.ModulePart.
 */
class ModulePart : public lua::lua_Base
{
public:
    ModulePart();
	virtual ~ModulePart();

	// co::ModulePart methods:
	void initialize( co::Module* module );
	void integrate( co::Module* module );
	void integratePresentation( co::Module* module );
	void disintegrate( co::Module* module );
	void dispose( co::Module* module );

private:
	co::RefPtr<LuaModulePartLoader> _luaModulePartLoader;
};

#endif
