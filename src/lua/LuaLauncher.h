/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#ifndef _LUALAUNCHER_H_
#define _LUALAUNCHER_H_

#include "Launcher_Base.h"

namespace lua {

/*!
	A launcher component for use with the 'coral' launcher executable.
 */
class Launcher : public Launcher_Base
{
public:
    Launcher();
	virtual ~Launcher();

	// lua::ILauncher methods:
	co::int32 main( co::ArrayRange<std::string const> args );
};

} // namespace lua

#endif
