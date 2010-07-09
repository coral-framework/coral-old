/*******************************************************************************
** C++ mapping generated for type 'co.TypeLoadException'
**
** Created: Fri Jul 02 00:14:15 2010
**      by: Coral Compiler version 0.1.0
**
** WARNING! All changes made in this file will be lost when recompiling!
********************************************************************************/

#ifndef _CO_TYPELOADEXCEPTION_H_
#define _CO_TYPELOADEXCEPTION_H_

#include <co/TypeTraits.h>
#include <co/UserException.h>

// co.TypeLoadException Mapping:
namespace co {

class TypeLoadException : public co::UserException
{
public:
	TypeLoadException()
	{;}
	
	TypeLoadException( const std::string& message )
		: co::UserException( message )
	{;}
};

} // namespace co

namespace co {
template<> struct kindOf<co::TypeLoadException> : public kindOfBase<TK_EXCEPTION> {};
template<> struct nameOf<co::TypeLoadException> { static const char* get() { return "co.TypeLoadException"; } };
template<> struct typeOf<co::TypeLoadException> : public typeOfBase<co::TypeLoadException, ExceptionType> {};
} // namespace co

#endif // _CO_TYPELOADEXCEPTION_H_
