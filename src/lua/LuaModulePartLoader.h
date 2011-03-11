/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#ifndef _LUAMODULEPARTLOADER_H_
#define _LUAMODULEPARTLOADER_H_

#include "ModulePartLoader_Base.h"
#include "LuaModulePart.h"

namespace lua {

/*!
	A co.ModulePartLoader that allows Coral modules to be written in Lua.
	If a module 'foo' contains a script named 'foo.lua', this component
	will create a lua.ModulePart to wrap the script as a co.ModulePart.
 */
class ModulePartLoader : public ModulePartLoader_Base
{
public:
	ModulePartLoader();
	virtual ~ModulePartLoader();

	// co::ModulePartLoader methods:
	bool canLoadModulePart( const std::string& moduleName );
	co::ModulePart* loadModulePart( const std::string& moduleName );
	
private:
	bool locateModuleLibrary( const std::string& moduleName, std::string* filename = 0 );
};

} // namespace lua

#endif
