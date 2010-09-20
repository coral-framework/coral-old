/*
 * Coral - A C++ Component Framework.
 * See Copyright Notice in Coral.h
 */

#ifndef _CO_UUID_H_
#define _CO_UUID_H_

#include <co/TypeTraits.h>
#include <co/reserved/Uuid.h>

// co.Uuid Mapping:
namespace co {

	// This is a native class mapping.
	// You may use type 'co::Uuid' directly, or through the typedef below.
	typedef co::Uuid Uuid;

} // namespace co

namespace co {
template<> struct kindOf<co::Uuid> : public kindOfBase<TK_NATIVECLASS> {};
template<> struct nameOf<co::Uuid> { static const char* get() { return "co.Uuid"; } };
template<> struct typeOf<co::Uuid> : public typeOfBase<co::Uuid, NativeClassType> {};
} // namespace co

#endif // _CO_UUID_H_
