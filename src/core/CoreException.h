/*
 * Coral - A C++ Component Framework.
 * See Copyright Notice in Coral.h
 */

#ifndef _CO_COREEXCEPTION_H_
#define _CO_COREEXCEPTION_H_

#include <co/Exception.h>

namespace co {

/*!
	Thrown to indicate internal Coral/C++ runtime errors.

	This is in contrast to all co::UserException's, which indicate
	external, user-level exceptions.
 */
class CoreException : public Exception
{
public:
	CoreException() : Exception( "" )
	{;}

	CoreException( const std::string& message ) : Exception( message )
	{;}
};

template<> struct kindOf<CoreException> : public kindOfBase<TK_EXCEPTION> {};

} // namespace co

#endif // _CO_COREEXCEPTION_H_
