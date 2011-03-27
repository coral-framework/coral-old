/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#ifndef _CO_ILLEGALNAMEEXCEPTION_H_
#define _CO_ILLEGALNAMEEXCEPTION_H_

#include <co/TypeTraits.h>
#include <co/Exception.h>

// co.IllegalNameException Mapping:
namespace co {

class IllegalNameException : public co::Exception
{
public:
	IllegalNameException()
	{;}

	IllegalNameException( const std::string& message )
		: co::Exception( message )
	{;}
};

} // namespace co

namespace co {
template<> struct kindOf<co::IllegalNameException> : public kindOfBase<TK_EXCEPTION> {};
template<> struct nameOf<co::IllegalNameException> { static const char* get() { return "co.IllegalNameException"; } };
template<> struct typeOf<co::IllegalNameException> : public typeOfBase<co::IllegalNameException, IException> {};
} // namespace co

#endif // _CO_ILLEGALNAMEEXCEPTION_H_
