/*
 * Coral - Lightweight C++ Component Framework.
 * See Copyright Notice in Coral.h
 */

#ifndef _CO_ILLEGALCASTEXCEPTION_H_
#define _CO_ILLEGALCASTEXCEPTION_H_

#include <co/TypeTraits.h>
#include <co/Exception.h>

// co.IllegalCastException Mapping:
namespace co {

class IllegalCastException : public co::Exception
{
public:
	IllegalCastException()
	{;}

	IllegalCastException( const std::string& message )
		: co::Exception( message )
	{;}
};

} // namespace co

namespace co {
template<> struct kindOf<co::IllegalCastException> : public kindOfBase<TK_EXCEPTION> {};
template<> struct nameOf<co::IllegalCastException> { static const char* get() { return "co.IllegalCastException"; } };
template<> struct typeOf<co::IllegalCastException> : public typeOfBase<co::IllegalCastException, ExceptionType> {};
} // namespace co

#endif // _CO_ILLEGALCASTEXCEPTION_H_
