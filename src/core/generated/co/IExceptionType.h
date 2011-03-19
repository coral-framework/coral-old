/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#ifndef _CO_IEXCEPTIONTYPE_H_
#define _CO_IEXCEPTIONTYPE_H_

#include <co/TypeTraits.h>
#include <co/IType.h>

// co.IExceptionType Mapping:
namespace co {

class IExceptionType : public co::IType
{
public:
	virtual ~IExceptionType() {;}
};

} // namespace co

namespace co {
template<> struct kindOf<co::IExceptionType> : public kindOfBase<TK_INTERFACE> {};
template<> struct nameOf<co::IExceptionType> { static const char* get() { return "co.IExceptionType"; } };
template<> struct typeOf<co::IExceptionType> : public typeOfBase<co::IExceptionType, IInterfaceType> {};
} // namespace co

#endif // _CO_IEXCEPTIONTYPE_H_
