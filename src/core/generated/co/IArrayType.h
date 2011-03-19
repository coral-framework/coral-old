/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#ifndef _CO_IARRAYTYPE_H_
#define _CO_IARRAYTYPE_H_

#include <co/TypeTraits.h>
#include <co/IType.h>

// co.IArrayType Mapping:
namespace co {

class IArrayType : public co::IType
{
public:
	virtual ~IArrayType() {;}

	virtual co::IType* getElementType() = 0;
};

} // namespace co

namespace co {
template<> struct kindOf<co::IArrayType> : public kindOfBase<TK_INTERFACE> {};
template<> struct nameOf<co::IArrayType> { static const char* get() { return "co.IArrayType"; } };
template<> struct typeOf<co::IArrayType> : public typeOfBase<co::IArrayType, IInterfaceType> {};
} // namespace co

#endif // _CO_IARRAYTYPE_H_
