/*
 * Coral - A C++ Component Framework.
 * See Copyright Notice in Coral.h
 */

#ifndef _CO_MISSINGSERVICEEXCEPTION_H_
#define _CO_MISSINGSERVICEEXCEPTION_H_

#include <co/TypeTraits.h>
#include <co/Exception.h>

// co.MissingServiceException Mapping:
namespace co {

class MissingServiceException : public co::Exception
{
public:
	MissingServiceException()
	{;}

	MissingServiceException( const std::string& message )
		: co::Exception( message )
	{;}
};

} // namespace co

namespace co {
template<> struct kindOf<co::MissingServiceException> : public kindOfBase<TK_EXCEPTION> {};
template<> struct nameOf<co::MissingServiceException> { static const char* get() { return "co.MissingServiceException"; } };
template<> struct typeOf<co::MissingServiceException> : public typeOfBase<co::MissingServiceException, ExceptionType> {};
} // namespace co

#endif // _CO_MISSINGSERVICEEXCEPTION_H_
