/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#ifndef _CO_TYPELOADEXCEPTION_H_
#define _CO_TYPELOADEXCEPTION_H_

#include <co/TypeTraits.h>
#include <co/Exception.h>

// co.TypeLoadException Mapping:
namespace co {

class TypeLoadException : public co::Exception
{
public:
	TypeLoadException()
	{;}

	TypeLoadException( const std::string& message )
		: co::Exception( message )
	{;}
};

} // namespace co

namespace co {
template<> struct kindOf<co::TypeLoadException> : public kindOfBase<TK_EXCEPTION> {};
template<> struct nameOf<co::TypeLoadException> { static const char* get() { return "co.TypeLoadException"; } };
template<> struct typeOf<co::TypeLoadException> : public typeOfBase<co::TypeLoadException, ExceptionType> {};
} // namespace co

#endif // _CO_TYPELOADEXCEPTION_H_
