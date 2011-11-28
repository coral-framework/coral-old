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
};

} // namespace co

namespace co {
template<> struct kindOf<co::INativeClass> : public kindOfBase<TK_INTERFACE> {};
template<> struct nameOf<co::INativeClass> { static const char* get() { return "co.INativeClass"; } };
} // namespace co

#endif // _CO_INATIVECLASS_H_
