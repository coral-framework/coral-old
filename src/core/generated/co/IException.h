/*
 * Coral - Lightweight C++ Component Framework
 * See Copyright Notice in Coral.h
 */

#ifndef _CO_IEXCEPTION_H_
#define _CO_IEXCEPTION_H_

#include <co/TypeTraits.h>
#include <co/IType.h>

// co.IException Mapping:
namespace co {

class IException : public co::IType
{
public:
	virtual ~IException() {;}
};

} // namespace co

namespace co {
template<> struct kindOf<co::IException> : public kindOfBase<TK_INTERFACE> {};
template<> struct nameOf<co::IException> { static const char* get() { return "co.IException"; } };
template<> struct typeOf<co::IException> : public typeOfBase<co::IException, IInterface> {};
} // namespace co

#endif // _CO_IEXCEPTION_H_
