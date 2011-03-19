/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#ifndef _LUA_ISTATE_H_
#define _LUA_ISTATE_H_

#include <co/TypeTraits.h>
#include <co/Any.h>
#include <co/ArrayRange.h>
#include <vector>
#include <co/Interface.h>

// lua.IState Mapping:
namespace lua {

class IState : public co::Interface
{
public:
	virtual ~IState() {;}

	virtual co::int32 callFunction( const std::string& moduleName, const std::string& functionName, co::ArrayRange<co::Any const> args, co::ArrayRange<co::Any const> results ) = 0;

	virtual bool findScript( const std::string& name, std::string& filename ) = 0;
};

} // namespace lua

namespace co {
template<> struct kindOf<lua::IState> : public kindOfBase<TK_INTERFACE> {};
template<> struct nameOf<lua::IState> { static const char* get() { return "lua.IState"; } };
template<> struct typeOf<lua::IState> : public typeOfBase<lua::IState, IInterfaceType> {};
} // namespace co

#endif // _LUA_ISTATE_H_
