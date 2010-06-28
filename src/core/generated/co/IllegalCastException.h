/*******************************************************************************
** C++ mapping generated for type 'co.IllegalCastException'
**
** Created: Mon Jun 28 12:01:36 2010
**      by: Coral Compiler version 0.1.0
**
** WARNING! All changes made in this file will be lost when recompiling!
********************************************************************************/

#ifndef _CO_ILLEGALCASTEXCEPTION_H_
#define _CO_ILLEGALCASTEXCEPTION_H_

#include <co/TypeTraits.h>
#include <co/UserException.h>

// co.IllegalCastException Mapping:
namespace co {

class IllegalCastException : public co::UserException
{
public:
	IllegalCastException()
	{;}
	
	IllegalCastException( const std::string& message )
		: co::UserException( message )
	{;}
};

} // namespace co

namespace co {
template<> struct kindOf<co::IllegalCastException> : public kindOfBase<TK_EXCEPTION> {};
template<> struct nameOf<co::IllegalCastException> { static const char* get() { return "co.IllegalCastException"; } };
template<> struct typeOf<co::IllegalCastException> : public typeOfBase<co::IllegalCastException, ExceptionType> {};
} // namespace co

#endif // _CO_ILLEGALCASTEXCEPTION_H_
