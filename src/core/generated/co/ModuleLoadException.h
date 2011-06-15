/*
 * Coral - Lightweight C++ Component Framework
 * See Copyright Notice in Coral.h
 */

#ifndef _CO_MODULELOADEXCEPTION_H_
#define _CO_MODULELOADEXCEPTION_H_

#include <co/TypeTraits.h>
#include <co/Exception.h>

// co.ModuleLoadException Mapping:
namespace co {

class ModuleLoadException : public co::Exception
{
public:
	ModuleLoadException()
	{;}

	ModuleLoadException( const std::string& message )
		: co::Exception( message )
	{;}

	virtual ~ModuleLoadException() throw()
	{;}

	inline const char* getTypeName() const { return "co.ModuleLoadException"; }
};

} // namespace co

namespace co {
template<> struct kindOf<co::ModuleLoadException> : public kindOfBase<TK_EXCEPTION> {};
template<> struct nameOf<co::ModuleLoadException> { static const char* get() { return "co.ModuleLoadException"; } };
template<> struct typeOf<co::ModuleLoadException> : public typeOfBase<co::ModuleLoadException, IException> {};
} // namespace co

#endif // _CO_MODULELOADEXCEPTION_H_
