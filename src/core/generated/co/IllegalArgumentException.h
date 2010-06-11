/*******************************************************************************
** C++ mapping generated for type 'co.IllegalArgumentException'
**
** Created: Thu Jun 10 20:53:30 2010
**      by: Coral Compiler version 0.1.0
**
** WARNING! All changes made in this file will be lost when recompiling!
********************************************************************************/

#ifndef _CO_ILLEGALARGUMENTEXCEPTION_H_
#define _CO_ILLEGALARGUMENTEXCEPTION_H_

#include <co/TypeTraits.h>
#include <co/UserException.h>

// co.IllegalArgumentException Mapping:
namespace co {

class IllegalArgumentException : public co::UserException
{
public:
	IllegalArgumentException()
	{;}
	
	IllegalArgumentException( const std::string& message )
		: co::UserException( message )
	{;}
};

} // namespace co

namespace co {
template<> struct kindOf<co::IllegalArgumentException> : public kindOfBase<TK_EXCEPTION> {};
template<> struct nameOf<co::IllegalArgumentException> { static const char* get() { return "co.IllegalArgumentException"; } };
template<> struct typeOf<co::IllegalArgumentException> : public typeOfBase<co::IllegalArgumentException, ExceptionType> {};
} // namespace co

#endif // _CO_ILLEGALARGUMENTEXCEPTION_H_
