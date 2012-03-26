/*
 * Coral - Lightweight C++ Component Framework
 * See Copyright Notice in Coral.h
 */

#ifndef _LUA_ISTATE_H_
#define _LUA_ISTATE_H_

#include <co/TypeTraits.h>
#include <co/Range.h>
#include <co/Any.h>
#include <vector>
#include <co/IService.h>

// Forward Declarations:
namespace lua {
	class IInterceptor;
} // namespace lua
// End Of Forward Declarations

// lua.IState Mapping:
namespace lua {

class IState : public co::IService
{
public:
	virtual ~IState() {;}

	virtual co::Range<lua::IInterceptor* const> getInterceptors() = 0;

	virtual void addInterceptor( lua::IInterceptor* interceptor ) = 0;

	virtual co::int32 callFunction( const std::string& moduleName, const std::string& functionName, co::Range<co::Any const> args, co::Range<co::Any const> results ) = 0;

	virtual void collectGarbage() = 0;

	virtual bool findScript( const std::string& name, std::string& filename ) = 0;

	virtual void removeInterceptor( lua::IInterceptor* interceptor ) = 0;
};

} // namespace lua

namespace co {
template<> struct kindOf<lua::IState> : public kindOfBase<TK_INTERFACE> {};
template<> struct nameOf<lua::IState> { static const char* get() { return "lua.IState"; } };
} // namespace co

#endif // _LUA_ISTATE_H_
