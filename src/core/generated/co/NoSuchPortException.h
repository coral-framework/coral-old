/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#ifndef _CO_NOSUCHPORTEXCEPTION_H_
#define _CO_NOSUCHPORTEXCEPTION_H_

#include <co/TypeTraits.h>
#include <co/Exception.h>

// co.NoSuchPortException Mapping:
namespace co {

class NoSuchPortException : public co::Exception
{
public:
	NoSuchPortException()
	{;}

	NoSuchPortException( const std::string& message )
		: co::Exception( message )
	{;}

	virtual ~NoSuchPortException() throw()
	{;}

	inline const char* getTypeName() const { return "co.NoSuchPortException"; }
};

} // namespace co

namespace co {
template<> struct kindOf<co::NoSuchPortException> : public kindOfBase<TK_EXCEPTION> {};
template<> struct nameOf<co::NoSuchPortException> { static const char* get() { return "co.NoSuchPortException"; } };
template<> struct typeOf<co::NoSuchPortException> : public typeOfBase<co::NoSuchPortException, IException> {};
} // namespace co

#endif // _CO_NOSUCHPORTEXCEPTION_H_
