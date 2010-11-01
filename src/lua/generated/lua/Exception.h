/*
 * Coral - A C++ Component Framework.
 * See Copyright Notice in Coral.h
 */

#ifndef _LUA_EXCEPTION_H_
#define _LUA_EXCEPTION_H_

#include <co/TypeTraits.h>
#include <co/Exception.h>

// lua.Exception Mapping:
namespace lua {

class Exception : public co::Exception
{
public:
	Exception()
	{;}

	Exception( const std::string& message )
		: co::Exception( message )
	{;}
};

} // namespace lua

namespace co {
template<> struct kindOf<lua::Exception> : public kindOfBase<TK_EXCEPTION> {};
template<> struct nameOf<lua::Exception> { static const char* get() { return "lua.Exception"; } };
template<> struct typeOf<lua::Exception> : public typeOfBase<lua::Exception, ExceptionType> {};
} // namespace co

#endif // _LUA_EXCEPTION_H_
