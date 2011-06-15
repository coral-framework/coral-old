/*
 * Coral - Lightweight C++ Component Framework
 * See Copyright Notice in Coral.h
 */

#ifndef _LUA_ILAUNCHER_H_
#define _LUA_ILAUNCHER_H_

#include <co/TypeTraits.h>
#include <co/IService.h>
#include <vector>
#include <co/Range.h>

// lua.ILauncher Mapping:
namespace lua {

class ILauncher : public co::IService
{
public:
	virtual ~ILauncher() {;}

	virtual co::int32 main( co::Range<std::string const> args ) = 0;
};

} // namespace lua

namespace co {
template<> struct kindOf<lua::ILauncher> : public kindOfBase<TK_INTERFACE> {};
template<> struct nameOf<lua::ILauncher> { static const char* get() { return "lua.ILauncher"; } };
template<> struct typeOf<lua::ILauncher> : public typeOfBase<lua::ILauncher, IInterface> {};
} // namespace co

#endif // _LUA_ILAUNCHER_H_
