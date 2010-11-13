/*
 * Coral - Lightweight C++ Component Framework.
 * See Copyright Notice in Coral.h
 */

#ifndef _LUALAUNCHER_H_
#define _LUALAUNCHER_H_

#include "Launcher_Base.h"

/*!
	A launcher component for use with the 'coral' launcher executable.
 */
class LuaLauncher : public lua::Launcher_Base
{
public:
    LuaLauncher();
	virtual ~LuaLauncher();

	// lua::ILauncher methods:
	co::int32 main( co::ArrayRange<std::string const> args );
};

#endif
