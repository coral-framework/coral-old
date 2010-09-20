/*
 * Coral - A C++ Component Framework.
 * See Copyright Notice in Coral.h
 */

#ifndef _CO_ARRAYTYPE_H_
#define _CO_ARRAYTYPE_H_

#include <co/TypeTraits.h>
#include <co/Type.h>

// co.ArrayType Mapping:
namespace co {

class ArrayType : public co::Type
{
public:
	virtual ~ArrayType() {;}

	virtual co::Type* getElementType() = 0;
};

} // namespace co

namespace co {
template<> struct kindOf<co::ArrayType> : public kindOfBase<TK_INTERFACE> {};
template<> struct nameOf<co::ArrayType> { static const char* get() { return "co.ArrayType"; } };
template<> struct typeOf<co::ArrayType> : public typeOfBase<co::ArrayType, InterfaceType> {};
} // namespace co

#endif // _CO_ARRAYTYPE_H_
