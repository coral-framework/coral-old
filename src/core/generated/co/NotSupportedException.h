/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#ifndef _CO_UNSUPPORTEDOPERATIONEXCEPTION_H_
#define _CO_UNSUPPORTEDOPERATIONEXCEPTION_H_

#include <co/TypeTraits.h>
#include <co/Exception.h>

// co.NotSupportedException Mapping:
namespace co {

class NotSupportedException : public co::Exception
{
public:
	NotSupportedException()
	{;}

	NotSupportedException( const std::string& message )
		: co::Exception( message )
	{;}
};

} // namespace co

namespace co {
template<> struct kindOf<co::NotSupportedException> : public kindOfBase<TK_EXCEPTION> {};
template<> struct nameOf<co::NotSupportedException> { static const char* get() { return "co.NotSupportedException"; } };
template<> struct typeOf<co::NotSupportedException> : public typeOfBase<co::NotSupportedException, ExceptionType> {};
} // namespace co

#endif // _CO_UNSUPPORTEDOPERATIONEXCEPTION_H_
