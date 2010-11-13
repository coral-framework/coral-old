/*
 * Coral - Lightweight C++ Component Framework.
 * See Copyright Notice in Coral.h
 */

#ifndef _CO_EXCEPTIONTYPE_H_
#define _CO_EXCEPTIONTYPE_H_

#include <co/TypeTraits.h>
#include <co/Type.h>

// co.ExceptionType Mapping:
namespace co {

class ExceptionType : public co::Type
{
public:
	virtual ~ExceptionType() {;}
};

} // namespace co

namespace co {
template<> struct kindOf<co::ExceptionType> : public kindOfBase<TK_INTERFACE> {};
template<> struct nameOf<co::ExceptionType> { static const char* get() { return "co.ExceptionType"; } };
template<> struct typeOf<co::ExceptionType> : public typeOfBase<co::ExceptionType, InterfaceType> {};
} // namespace co

#endif // _CO_EXCEPTIONTYPE_H_
