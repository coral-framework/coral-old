/*
 * Coral - Lightweight C++ Component Framework
 * See Copyright Notice in Coral.h
 */

#ifndef _LUA_IINTERCEPTOR_H_
#define _LUA_IINTERCEPTOR_H_

#include <co/TypeTraits.h>
#include <co/IService.h>
#include <co/Any.h>
#include <co/Range.h>
#include <vector>

// Forward Declarations:
namespace co {
	class IField;
	class IMethod;
} // namespace co
// End Of Forward Declarations

// lua.IInterceptor Mapping:
namespace lua {

class IInterceptor : public co::IService
{
public:
	virtual ~IInterceptor() {;}

	virtual void postGetField( co::IService* service, co::IField* field, const co::Any& value ) = 0;

	virtual void postInvoke( co::IService* service, co::IMethod* method, co::Range<co::Any const> args, const co::Any& returnValue ) = 0;

	virtual void postSetField( co::IService* service, co::IField* field, const co::Any& value ) = 0;
};

} // namespace lua

namespace co {
template<> struct kindOf<lua::IInterceptor> : public kindOfBase<TK_INTERFACE> {};
template<> struct nameOf<lua::IInterceptor> { static const char* get() { return "lua.IInterceptor"; } };
} // namespace co

#endif // _LUA_IINTERCEPTOR_H_
