/*******************************************************************************
** C++ mapping generated for type 'co.NoSuchInterfaceException'
**
** Created: Fri Jul 02 00:14:15 2010
**      by: Coral Compiler version 0.1.0
**
** WARNING! All changes made in this file will be lost when recompiling!
********************************************************************************/

#ifndef _CO_NOSUCHINTERFACEEXCEPTION_H_
#define _CO_NOSUCHINTERFACEEXCEPTION_H_

#include <co/TypeTraits.h>
#include <co/UserException.h>

// co.NoSuchInterfaceException Mapping:
namespace co {

class NoSuchInterfaceException : public co::UserException
{
public:
	NoSuchInterfaceException()
	{;}
	
	NoSuchInterfaceException( const std::string& message )
		: co::UserException( message )
	{;}
};

} // namespace co

namespace co {
template<> struct kindOf<co::NoSuchInterfaceException> : public kindOfBase<TK_EXCEPTION> {};
template<> struct nameOf<co::NoSuchInterfaceException> { static const char* get() { return "co.NoSuchInterfaceException"; } };
template<> struct typeOf<co::NoSuchInterfaceException> : public typeOfBase<co::NoSuchInterfaceException, ExceptionType> {};
} // namespace co

#endif // _CO_NOSUCHINTERFACEEXCEPTION_H_
