/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#ifndef _CO_ILLEGALARGUMENTEXCEPTION_H_
#define _CO_ILLEGALARGUMENTEXCEPTION_H_

#include <co/TypeTraits.h>
#include <co/Exception.h>

// co.IllegalArgumentException Mapping:
namespace co {

class IllegalArgumentException : public co::Exception
{
public:
	IllegalArgumentException()
	{;}

	IllegalArgumentException( const std::string& message )
		: co::Exception( message )
	{;}
};

} // namespace co

namespace co {
template<> struct kindOf<co::IllegalArgumentException> : public kindOfBase<TK_EXCEPTION> {};
template<> struct nameOf<co::IllegalArgumentException> { static const char* get() { return "co.IllegalArgumentException"; } };
template<> struct typeOf<co::IllegalArgumentException> : public typeOfBase<co::IllegalArgumentException, ExceptionType> {};
} // namespace co

#endif // _CO_ILLEGALARGUMENTEXCEPTION_H_
