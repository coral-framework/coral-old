/*
 * Coral - A C++ Component Framework.
 * See Copyright Notice in Coral.h
 */

#ifndef _CO_UNSUPPORTEDOPERATIONEXCEPTION_H_
#define _CO_UNSUPPORTEDOPERATIONEXCEPTION_H_

#include <co/TypeTraits.h>
#include <co/Exception.h>

// co.UnsupportedOperationException Mapping:
namespace co {

class UnsupportedOperationException : public co::Exception
{
public:
	UnsupportedOperationException()
	{;}

	UnsupportedOperationException( const std::string& message )
		: co::Exception( message )
	{;}
};

} // namespace co

namespace co {
template<> struct kindOf<co::UnsupportedOperationException> : public kindOfBase<TK_EXCEPTION> {};
template<> struct nameOf<co::UnsupportedOperationException> { static const char* get() { return "co.UnsupportedOperationException"; } };
template<> struct typeOf<co::UnsupportedOperationException> : public typeOfBase<co::UnsupportedOperationException, ExceptionType> {};
} // namespace co

#endif // _CO_UNSUPPORTEDOPERATIONEXCEPTION_H_
