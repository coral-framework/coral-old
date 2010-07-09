/*******************************************************************************
** C++ mapping generated for type 'co.ArrayType'
**
** Created: Fri Jul 02 00:14:15 2010
**      by: Coral Compiler version 0.1.0
**
** WARNING! All changes made in this file will be lost when recompiling!
********************************************************************************/

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
