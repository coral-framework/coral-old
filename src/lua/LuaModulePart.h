/*
 * Coral - A C++ Component Framework.
 * See Copyright Notice in Coral.h
 */

#ifndef _LUAMODULEPART_H_
#define _LUAMODULEPART_H_

#include "ModulePart_Base.h"

/*!
	A co.ModulePart that wraps a Lua script in a module's dir.
 */
class LuaModulePart : public lua::ModulePart_Base
{
public:
    LuaModulePart();
	virtual ~LuaModulePart();

	// co::ModulePart methods:
	void initialize( co::Module* module );
	void integrate( co::Module* module );
	void integratePresentation( co::Module* module );
	void disintegrate( co::Module* module );
	void dispose( co::Module* module );

private:
	void callScriptMethod( const char* methodName, co::Module* module );
};

#endif
