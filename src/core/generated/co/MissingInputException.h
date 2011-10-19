/*
 * Coral - Lightweight C++ Component Framework
 * See Copyright Notice in Coral.h
 */

#ifndef _CO_MISSINGINPUTEXCEPTION_H_
#define _CO_MISSINGINPUTEXCEPTION_H_

#include <co/TypeTraits.h>
#include <co/Exception.h>

// co.MissingInputException Mapping:
namespace co {

class MissingInputException : public co::Exception
{
public:
	MissingInputException()
	{;}

	MissingInputException( const std::string& message )
		: co::Exception( message )
	{;}

	virtual ~MissingInputException() throw()
	{;}

	inline const char* getTypeName() const { return "co.MissingInputException"; }
};

} // namespace co

namespace co {
template<> struct kindOf<co::MissingInputException> : public kindOfBase<TK_EXCEPTION> {};
template<> struct nameOf<co::MissingInputException> { static const char* get() { return "co.MissingInputException"; } };
} // namespace co

#endif // _CO_MISSINGINPUTEXCEPTION_H_
