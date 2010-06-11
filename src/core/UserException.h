/*
 * Coral - A C++ Component Framework.
 * See Copyright Notice in Coral.h
 */

#ifndef _CO_USEREXCEPTION_H_
#define _CO_USEREXCEPTION_H_

#include <co/Exception.h>
#include <string>

namespace co {

class UserException : public Exception
{
public:
	UserException() : Exception( "" )
	{;}

	UserException( const std::string& message ) : Exception( message )
	{;}
};

template<> struct kindOf<UserException> : public kindOfBase<TK_EXCEPTION> {};

} // namespace co

#endif // _CO_USEREXCEPTION_H_
