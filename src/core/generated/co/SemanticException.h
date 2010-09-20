/*
 * Coral - A C++ Component Framework.
 * See Copyright Notice in Coral.h
 */

#ifndef _CO_SEMANTICEXCEPTION_H_
#define _CO_SEMANTICEXCEPTION_H_

#include <co/TypeTraits.h>
#include <co/UserException.h>

// co.SemanticException Mapping:
namespace co {

class SemanticException : public co::UserException
{
public:
	SemanticException()
	{;}

	SemanticException( const std::string& message )
		: co::UserException( message )
	{;}
};

} // namespace co

namespace co {
template<> struct kindOf<co::SemanticException> : public kindOfBase<TK_EXCEPTION> {};
template<> struct nameOf<co::SemanticException> { static const char* get() { return "co.SemanticException"; } };
template<> struct typeOf<co::SemanticException> : public typeOfBase<co::SemanticException, ExceptionType> {};
} // namespace co

#endif // _CO_SEMANTICEXCEPTION_H_
