/*******************************************************************************
** C++ mapping generated for type 'co.UnsupportedOperationException'
**
** Created: Thu Jun 10 20:53:30 2010
**      by: Coral Compiler version 0.1.0
**
** WARNING! All changes made in this file will be lost when recompiling!
********************************************************************************/

#ifndef _CO_UNSUPPORTEDOPERATIONEXCEPTION_H_
#define _CO_UNSUPPORTEDOPERATIONEXCEPTION_H_

#include <co/TypeTraits.h>
#include <co/UserException.h>

// co.UnsupportedOperationException Mapping:
namespace co {

class UnsupportedOperationException : public co::UserException
{
public:
	UnsupportedOperationException()
	{;}
	
	UnsupportedOperationException( const std::string& message )
		: co::UserException( message )
	{;}
};

} // namespace co

namespace co {
template<> struct kindOf<co::UnsupportedOperationException> : public kindOfBase<TK_EXCEPTION> {};
template<> struct nameOf<co::UnsupportedOperationException> { static const char* get() { return "co.UnsupportedOperationException"; } };
template<> struct typeOf<co::UnsupportedOperationException> : public typeOfBase<co::UnsupportedOperationException, ExceptionType> {};
} // namespace co

#endif // _CO_UNSUPPORTEDOPERATIONEXCEPTION_H_
