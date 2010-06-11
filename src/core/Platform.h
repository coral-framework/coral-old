/*
 * Coral - A C++ Component Framework.
 * See Copyright Notice in Coral.h
 */

#ifndef _CO_PLATFORM_H_
#define _CO_PLATFORM_H_

//! \file Platform.h Platform Info and Portable Types

#include <co/Config.h>
#include <cassert>
#include <cstddef>

#ifndef __cplusplus
	#error Coral requires a C++ compiler.
#endif

//------ Build Mode Detection ------------------------------------------------

#if defined(_DEBUG) || !defined(NDEBUG)
	#define CORAL_BUILD_MODE "debug"
#else
	#define CORAL_NDEBUG
	#define CORAL_BUILD_MODE "release"
#endif

//------ Recognized Operating Systems (CORAL_OS_x) ---------------------------

#if defined(__APPLE__) && (defined(__GNUC__) || defined(__xlC__) || defined(__xlc__))
	#define CORAL_OS_MAC
	#define CORAL_OS_BSD4
	#ifdef __LP64__
		#define CORAL_OS_MAC64
	#else
		#define CORAL_OS_MAC32
	#endif
#elif defined(__CYGWIN__)
	#define CORAL_OS_CYGWIN
#elif defined(MSDOS) || defined(_MSDOS)
	#define CORAL_OS_MSDOS
#elif !defined(SAG_COM) && (defined(WIN64) || defined(_WIN64) || defined(__WIN64__))
	#define CORAL_OS_WIN32
	#define CORAL_OS_WIN64
#elif !defined(SAG_COM) && (defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__))
	#define CORAL_OS_WIN32
#elif defined(__linux__) || defined(__linux)
	#define CORAL_OS_LINUX
#elif defined(__FreeBSD__) || defined(__DragonFly__)
	#define CORAL_OS_FREEBSD
	#define CORAL_OS_BSD4
#elif defined(__NetBSD__)
	#define CORAL_OS_NETBSD
	#define CORAL_OS_BSD4
#elif defined(__OpenBSD__)
	#define CORAL_OS_OPENBSD
	#define CORAL_OS_BSD4
#elif defined(__sgi)
	#define CORAL_OS_IRIX
#else
	#error Oops! Unknown or unsupported OS!
#endif

#if defined(CORAL_OS_WIN32) || defined(CORAL_OS_WIN64)
	#define CORAL_OS_WIN
#endif

#if defined(CORAL_OS_MSDOS) || defined(CORAL_OS_WIN)
	#undef CORAL_OS_UNIX
#elif !defined(CORAL_OS_UNIX)
	#define CORAL_OS_UNIX
#endif

//------ Recognized compilers (CORAL_CC_x) -----------------------------------

#if defined(_MSC_VER)
	#define CORAL_CC_MSVC
	#if defined(__INTEL_COMPILER)
		#define CORAL_CC_INTEL
	#endif
#elif defined(__GNUC__)
	#define CORAL_CC_GNU
	#if defined(__MINGW32__)
		#define CORAL_CC_MINGW
	#endif
	#if defined(__INTEL_COMPILER)
		#define CORAL_CC_INTEL
	#endif
#endif

//------ Determine the CORAL_BUILD_KEY ---------------------------------------

#if defined(CORAL_OS_WIN)
	#define CORAL_OS_NAME "Windows"
#elif defined(CORAL_OS_LINUX)
	#define CORAL_OS_NAME "Linux"
#elif defined(CORAL_OS_MAC)
	#define CORAL_OS_NAME "MacOSX"
#else
	#error "Unsupported operating system! Coral requires Windows, Linux or MacOSX."
#endif

#if defined(__i386) || defined(__x86_64) || defined(_M_IX86) || defined(_M_X64)
	#if CORAL_POINTER_SIZE == 4
		#define CORAL_ARCH_NAME "x86_32"
	#else
		#define CORAL_ARCH_NAME "x86_64"
	#endif
#else
	#error "Unsupported architecture! Coral requires a 32 or 64-bit x86 processor."
#endif

#define CORAL_DO_STRINGIFY(X) #X
#define CORAL_STRINGIFY(X) CORAL_DO_STRINGIFY(X)

#if defined(CORAL_CC_MSVC) && !defined(CORAL_CC_INTEL)
	#define CORAL_CC_NAME "msvc"
	#if _MSC_VER > 1600
		#error "Your MSVC compiler was not recognized by this Coral version (maybe it's too new?)."
	#elif _MSC_VER >= 1600
		#define CORAL_CC_VERSION "10.0"
	#elif _MSC_VER >= 1500
		#define CORAL_CC_VERSION "9.0"
	#elif _MSC_VER >= 1400
		#define CORAL_CC_VERSION "8.0"
	#else
		#error "Coral requires Visual Studio 8 (2005) or newer."
	#endif
#elif defined(CORAL_CC_GNU) && !defined(CORAL_CC_MINGW) && !defined(CORAL_CC_INTEL)
	#define CORAL_CC_NAME "g++"
	#define CORAL_CC_VERSION CORAL_STRINGIFY(__GNUC__) "." CORAL_STRINGIFY(__GNUC_MINOR__)
#else
	#error "Unsupported compiler! Coral requires GCC (g++) or MSVC."
#endif

#define CORAL_BUILD_KEY		CORAL_OS_NAME " " CORAL_ARCH_NAME " " CORAL_CC_NAME "-" CORAL_CC_VERSION


//------ Portable Integer Types ----------------------------------------------

#if defined(CORAL_CC_GNU)
	#include <sys/types.h>
#endif

namespace co {

#if CORAL_POINTER_SIZE == 4
	typedef char				int8;
	typedef short				int16;
	typedef long				int32;
	typedef long long			int64;
	typedef unsigned char		uint8;
	typedef unsigned short		uint16;
	typedef unsigned long		uint32;
	typedef unsigned long long	uint64;
#elif CORAL_POINTER_SIZE == 8
	typedef char				int8;
	typedef short				int16;
	typedef int					int32;
	typedef long				int64;
	typedef unsigned char		uint8;
	typedef unsigned short		uint16;
	typedef unsigned int		uint32;
	typedef unsigned long		uint64;
#else
	#error Portable integers were not defined because CORAL_POINTER_SIZE is invalid.
#endif

// Limits for the portable integer types.
// (current definitions are only valid for the x86_* architectures)
#ifdef CORAL_OS_MAC
	#undef INT8_MIN
	#undef INT8_MAX
	#undef UINT8_MAX

	#undef INT16_MIN
	#undef INT16_MAX
	#undef UINT16_MAX

	#undef INT32_MIN
	#undef INT32_MAX
	#undef UINT32_MAX

	#undef INT64_MIN
	#undef INT64_MAX
	#undef UINT64_MAX
#endif

const int8		INT8_MIN	= -127 - 1;
const int8		INT8_MAX	=  127;
const uint8		UINT8_MAX	=  0xFF;

const int16		INT16_MIN	= -32767 - 1;
const int16		INT16_MAX	=  32767;
const uint16	UINT16_MAX	= 0xFFFF;

const int32		INT32_MIN	= -2147483647 - 1;
const int32		INT32_MAX	=  2147483647;
const uint32	UINT32_MAX	=  0xFFFFFFFF;

#if defined(CORAL_CC_MSVC)
const int64		INT64_MAX	= 0x7FFFFFFFFFFFFFFF;
const uint64	UINT64_MAX	= 0xFFFFFFFFFFFFFFFF;
#else
const int64		INT64_MAX	= 0x7FFFFFFFFFFFFFFFLL;
const uint64	UINT64_MAX	= 0xFFFFFFFFFFFFFFFFULL;
#endif

const int64		INT64_MIN	= -INT64_MAX - 1;

} // namespace co

/*!
	Imports all portable integer types into the current namespace.
	This is useful if you want to write \c uint8, \c int32, etc. in your code.
 */
#define USING_CORAL_INTEGER_TYPES	\
	using co::int8;		\
	using co::int16;	\
	using co::int32;	\
	using co::int64;	\
	using co::uint8;	\
	using co::uint16;	\
	using co::uint32;	\
	using co::uint64;

/*!
	\addtogroup platformutils Utility Macros and Functions
	@{
 */

/*!
	\def CORAL_STATIC_CHECK( const_expr, id_msg )
	Evaluates a constant expression and, if the result is false, aborts the
	compilation with an error message.

	\param[in] const_expr compile-time integral or pointer expression.
	\param[in] id_msg C++ identifier that describes the error (it does not need to be defined).
		Something like 'invalid_element_size'.
 */
#define CORAL_STATIC_CHECK( const_expr, id_msg ) \
    { co::CompileTimeError<( (const_expr) != 0 )> ERROR_##id_msg; (void)ERROR_##id_msg; }

/*!
	\def CORAL_ARRAY_LENGTH( array )
	Returns the number of elements in a statically allocated array.
	\param[in] array name of a statically allocated array.
 */
#define CORAL_ARRAY_LENGTH( array )	( sizeof( array ) / sizeof( array[0] ) )

/*!
	\def CORAL_INT64_C( c )
	Wraps the signed 64-bit integer literal 'c' in a platform-independent way.

	\def CORAL_UINT64_C( uc )
	Wraps the unsigned 64-bit integer literal 'uc' in a platform-independent way.
 */
#if defined(CORAL_OS_WIN) && !defined(CORAL_CC_GNU)
	#define CORAL_INT64_C( c ) c ## i64
	#define CORAL_UINT64_C( c ) c ## ui64
#elif CORAL_POINTER_SIZE == 8 && !defined(CORAL_OS_MAC)
	#define CORAL_INT64_C( c ) c ## L
	#define CORAL_UINT64_C( c ) c ## UL
#else
	#define CORAL_INT64_C( c ) c ## LL
	#define CORAL_UINT64_C( c ) c ## ULL
#endif

//! Supresses 'unused parameter' warnings.
#define CORAL_UNUSED( x ) (void)x;

// Portable function attributes to force or forbid inlining.
#if defined(CORAL_CC_MSVC)
	#define CORAL_FORCE_INLINE __forceinline 
	#define CORAL_NO_INLINE __declspec(noinline)
#else
	// assumes the compiler is GCC-compatible 
	#define CORAL_FORCE_INLINE __attribute__((always_inline)) 
	#define CORAL_NO_INLINE __attribute__((noinline)) 
#endif 

/*! @} */ // end of group platformutils

//! Main namespace of this library.
namespace co {

// Template trick to yield compile time errors:
template<int> struct CompileTimeError;
template<> struct CompileTimeError<true> {};

} // namespace co

//------ Portable shared-library interface attributes ------

#if defined(CORAL_OS_WIN)
	#define CORAL_DLL_EXPORT __declspec(dllexport)
	#define CORAL_DLL_IMPORT __declspec(dllimport)
	#if defined(BUILDING_CORAL_CORE)
		#define CORAL_EXPORT CORAL_DLL_EXPORT
	#else
		#define CORAL_EXPORT CORAL_DLL_IMPORT
	#endif
#else
	#define CORAL_EXPORT
	#define CORAL_DLL_EXPORT
#endif

#endif // _CO_PLATFORM_H_
