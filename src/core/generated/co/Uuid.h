/*
 * Coral - Lightweight C++ Component Framework
 * See Copyright Notice in Coral.h
 */

#ifndef _CO_UUID_H_
#define _CO_UUID_H_

#include <co/TypeTraits.h>

		/*
			Note: only ubiquitous header files can be included from within
			a native class. The preferred solution is to forward declare the
			type and use the @co.Include( "headername.h" ) annotation.
		 */
		#include <co/reserved/Uuid.h>
	
namespace co {
template<> struct kindOf<co::Uuid> : public kindOfBase<TK_NATIVECLASS> {};
template<> struct nameOf<co::Uuid> { static const char* get() { return "co.Uuid"; } };
} // namespace co

#endif // _CO_UUID_H_
