/*******************************************************************************
** C++ mapping generated for type 'co.MissingInputException'
**
** Created: Thu Jun 10 20:53:30 2010
**      by: Coral Compiler version 0.1.0
**
** WARNING! All changes made in this file will be lost when recompiling!
********************************************************************************/

#ifndef _CO_MISSINGINPUTEXCEPTION_H_
#define _CO_MISSINGINPUTEXCEPTION_H_

#include <co/TypeTraits.h>
#include <co/UserException.h>

// co.MissingInputException Mapping:
namespace co {

class MissingInputException : public co::UserException
{
public:
	MissingInputException()
	{;}
	
	MissingInputException( const std::string& message )
		: co::UserException( message )
	{;}
};

} // namespace co

namespace co {
template<> struct kindOf<co::MissingInputException> : public kindOfBase<TK_EXCEPTION> {};
template<> struct nameOf<co::MissingInputException> { static const char* get() { return "co.MissingInputException"; } };
template<> struct typeOf<co::MissingInputException> : public typeOfBase<co::MissingInputException, ExceptionType> {};
} // namespace co

#endif // _CO_MISSINGINPUTEXCEPTION_H_
