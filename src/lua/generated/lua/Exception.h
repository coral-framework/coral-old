/*******************************************************************************
** C++ mapping generated for type 'lua.Exception'
**
** Created: Mon Sep 20 12:13:52 2010
**      by: Coral Compiler version 0.3.0
**
** WARNING! All changes made in this file will be lost when recompiling!
********************************************************************************/

#ifndef _LUA_EXCEPTION_H_
#define _LUA_EXCEPTION_H_

#include <co/TypeTraits.h>
#include <co/UserException.h>

// lua.Exception Mapping:
namespace lua {

class Exception : public co::UserException
{
public:
	Exception()
	{;}

	Exception( const std::string& message )
		: co::UserException( message )
	{;}
};

} // namespace lua

namespace co {
template<> struct kindOf<lua::Exception> : public kindOfBase<TK_EXCEPTION> {};
template<> struct nameOf<lua::Exception> { static const char* get() { return "lua.Exception"; } };
template<> struct typeOf<lua::Exception> : public typeOfBase<lua::Exception, ExceptionType> {};
} // namespace co

#endif // _LUA_EXCEPTION_H_
