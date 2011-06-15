/*
 * Coral - Lightweight C++ Component Framework
 * See Copyright Notice in Coral.h
 */

#ifndef _CO_INATIVECLASS_H_
#define _CO_INATIVECLASS_H_

#include <co/TypeTraits.h>
#include <co/IClassType.h>

// co.INativeClass Mapping:
namespace co {

class INativeClass : public co::IClassType
{
public:
	virtual ~INativeClass() {;}

	virtual const std::string& getNativeHeader() = 0;

	virtual const std::string& getNativeName() = 0;
};

} // namespace co

namespace co {
template<> struct kindOf<co::INativeClass> : public kindOfBase<TK_INTERFACE> {};
template<> struct nameOf<co::INativeClass> { static const char* get() { return "co.INativeClass"; } };
template<> struct typeOf<co::INativeClass> : public typeOfBase<co::INativeClass, IInterface> {};
} // namespace co

#endif // _CO_INATIVECLASS_H_
