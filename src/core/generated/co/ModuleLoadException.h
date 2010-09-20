/*
 * Coral - A C++ Component Framework.
 * See Copyright Notice in Coral.h
 */

#ifndef _CO_MODULELOADEXCEPTION_H_
#define _CO_MODULELOADEXCEPTION_H_

#include <co/TypeTraits.h>
#include <co/UserException.h>

// co.ModuleLoadException Mapping:
namespace co {

class ModuleLoadException : public co::UserException
{
public:
	ModuleLoadException()
	{;}

	ModuleLoadException( const std::string& message )
		: co::UserException( message )
	{;}
};

} // namespace co

namespace co {
template<> struct kindOf<co::ModuleLoadException> : public kindOfBase<TK_EXCEPTION> {};
template<> struct nameOf<co::ModuleLoadException> { static const char* get() { return "co.ModuleLoadException"; } };
template<> struct typeOf<co::ModuleLoadException> : public typeOfBase<co::ModuleLoadException, ExceptionType> {};
} // namespace co

#endif // _CO_MODULELOADEXCEPTION_H_
