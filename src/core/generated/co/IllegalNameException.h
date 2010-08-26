/*******************************************************************************
** C++ mapping generated for type 'co.IllegalNameException'
**
** Created: Wed Aug 25 16:31:31 2010
**      by: Coral Compiler version 0.1.0
**
** WARNING! All changes made in this file will be lost when recompiling!
********************************************************************************/

#ifndef _CO_ILLEGALNAMEEXCEPTION_H_
#define _CO_ILLEGALNAMEEXCEPTION_H_

#include <co/TypeTraits.h>
#include <co/UserException.h>

// co.IllegalNameException Mapping:
namespace co {

class IllegalNameException : public co::UserException
{
public:
	IllegalNameException()
	{;}
	
	IllegalNameException( const std::string& message )
		: co::UserException( message )
	{;}
};

} // namespace co

namespace co {
template<> struct kindOf<co::IllegalNameException> : public kindOfBase<TK_EXCEPTION> {};
template<> struct nameOf<co::IllegalNameException> { static const char* get() { return "co.IllegalNameException"; } };
template<> struct typeOf<co::IllegalNameException> : public typeOfBase<co::IllegalNameException, ExceptionType> {};
} // namespace co

#endif // _CO_ILLEGALNAMEEXCEPTION_H_
