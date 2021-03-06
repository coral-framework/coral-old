/*
 * Coral - Lightweight C++ Component Framework
 * See Copyright Notice in Coral.h
 */

#ifndef _CO_UUID_H_
#define _CO_UUID_H_

#include <co/TypeTraits.h>

		namespace co {
			class Uuid;
		} // namespace co
	
namespace co {
template<> struct kindOf<co::Uuid> : public kindOfBase<TK_NATIVECLASS> {};
template<> struct nameOf<co::Uuid> { static const char* get() { return "co.Uuid"; } };
} // namespace co

#endif // _CO_UUID_H_
