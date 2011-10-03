/*
 * Coral - Lightweight C++ Component Framework
 * See Copyright Notice in Coral.h
 */

#ifndef _CO_IANNOTATION_H_
#define _CO_IANNOTATION_H_

#include <co/TypeTraits.h>
#include <co/IService.h>

// co.IAnnotation Mapping:
namespace co {

class IAnnotation : public co::IService
{
public:
	virtual ~IAnnotation() {;}
};

} // namespace co

namespace co {
template<> struct kindOf<co::IAnnotation> : public kindOfBase<TK_INTERFACE> {};
template<> struct nameOf<co::IAnnotation> { static const char* get() { return "co.IAnnotation"; } };
template<> struct typeOf<co::IAnnotation> : public typeOfBase<co::IAnnotation, IInterface> {};
} // namespace co

#endif // _CO_IANNOTATION_H_
