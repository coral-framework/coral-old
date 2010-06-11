/*******************************************************************************
** C++ mapping generated for type 'co.ModuleLoadException'
**
** Created: Thu Jun 10 20:53:30 2010
**      by: Coral Compiler version 0.1.0
**
** WARNING! All changes made in this file will be lost when recompiling!
********************************************************************************/

#ifndef _CO_MODULELOADEXCEPTION_H_
#define _CO_MODULELOADEXCEPTION_H_

#include <co/TypeTraits.h>
#include <co/UserException.h>

// co.ModuleLoadException Mapping:
namespace co {

class ModuleLoadException : public co::UserException
{
public:
	ModuleLoadException()
	{;}
	
	ModuleLoadException( const std::string& message )
		: co::UserException( message )
	{;}
};

} // namespace co

namespace co {
template<> struct kindOf<co::ModuleLoadException> : public kindOfBase<TK_EXCEPTION> {};
template<> struct nameOf<co::ModuleLoadException> { static const char* get() { return "co.ModuleLoadException"; } };
template<> struct typeOf<co::ModuleLoadException> : public typeOfBase<co::ModuleLoadException, ExceptionType> {};
} // namespace co

#endif // _CO_MODULELOADEXCEPTION_H_
