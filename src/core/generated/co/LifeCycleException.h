/*
 * Coral - A C++ Component Framework.
 * See Copyright Notice in Coral.h
 */

#ifndef _CO_LIFECYCLEEXCEPTION_H_
#define _CO_LIFECYCLEEXCEPTION_H_

#include <co/TypeTraits.h>
#include <co/Exception.h>

// co.LifeCycleException Mapping:
namespace co {

class LifeCycleException : public co::Exception
{
public:
	LifeCycleException()
	{;}

	LifeCycleException( const std::string& message )
		: co::Exception( message )
	{;}
};

} // namespace co

namespace co {
template<> struct kindOf<co::LifeCycleException> : public kindOfBase<TK_EXCEPTION> {};
template<> struct nameOf<co::LifeCycleException> { static const char* get() { return "co.LifeCycleException"; } };
template<> struct typeOf<co::LifeCycleException> : public typeOfBase<co::LifeCycleException, ExceptionType> {};
} // namespace co

#endif // _CO_LIFECYCLEEXCEPTION_H_
