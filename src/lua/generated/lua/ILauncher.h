/*
 * Coral - A C++ Component Framework.
 * See Copyright Notice in Coral.h
 */

#ifndef _LUA_ILAUNCHER_H_
#define _LUA_ILAUNCHER_H_

#include <co/TypeTraits.h>
#include <co/ArrayRange.h>
#include <vector>
#include <co/Interface.h>

// lua.ILauncher Mapping:
namespace lua {

class ILauncher : public co::Interface
{
public:
	virtual ~ILauncher() {;}

	virtual co::int32 main( co::ArrayRange<std::string const> args ) = 0;
};

} // namespace lua

namespace co {
template<> struct kindOf<lua::ILauncher> : public kindOfBase<TK_INTERFACE> {};
template<> struct nameOf<lua::ILauncher> { static const char* get() { return "lua.ILauncher"; } };
template<> struct typeOf<lua::ILauncher> : public typeOfBase<lua::ILauncher, InterfaceType> {};
} // namespace co

#endif // _LUA_ILAUNCHER_H_
