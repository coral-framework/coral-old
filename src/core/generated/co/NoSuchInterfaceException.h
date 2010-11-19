/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#ifndef _CO_NOSUCHINTERFACEEXCEPTION_H_
#define _CO_NOSUCHINTERFACEEXCEPTION_H_

#include <co/TypeTraits.h>
#include <co/Exception.h>

// co.NoSuchInterfaceException Mapping:
namespace co {

class NoSuchInterfaceException : public co::Exception
{
public:
	NoSuchInterfaceException()
	{;}

	NoSuchInterfaceException( const std::string& message )
		: co::Exception( message )
	{;}
};

} // namespace co

namespace co {
template<> struct kindOf<co::NoSuchInterfaceException> : public kindOfBase<TK_EXCEPTION> {};
template<> struct nameOf<co::NoSuchInterfaceException> { static const char* get() { return "co.NoSuchInterfaceException"; } };
template<> struct typeOf<co::NoSuchInterfaceException> : public typeOfBase<co::NoSuchInterfaceException, ExceptionType> {};
} // namespace co

#endif // _CO_NOSUCHINTERFACEEXCEPTION_H_
