/*******************************************************************************
** C++ mapping generated for type 'co.ExceptionType'
**
** Created: Thu Jun 10 20:53:30 2010
**      by: Coral Compiler version 0.1.0
**
** WARNING! All changes made in this file will be lost when recompiling!
********************************************************************************/

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
