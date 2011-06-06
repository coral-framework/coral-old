/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#ifndef _CO_PLATFORM_H_
#define _CO_PLATFORM_H_

/*!
	\file
	Platform Info, Portable Types and Utilities
 */

#ifndef __cplusplus
	#error Coral requires a C++ compiler.
#endif

#include <co/Config.h>
#include <cassert>
#include <cstddef>

//------ Build Mode Detection --------------------------------------------------

#if defined(_DEBUG) || !defined(NDEBUG)
	#define CORAL_BUILD_MODE "debug"
#else
	#define CORAL_NDEBUG
	#define CORAL_BUILD_MODE "release"
#endif

//------ Recognized Operating Systems (CORAL_OS_x) -----------------------------

#if defined(__APPLE__) && defined(__GNUC__)
	#define CORAL_OS_MAC
#elif defined(__linux__) || defined(__linux)
	#define CORAL_OS_LINUX
#elif defined(_WIN32) || defined(__WIN32__)
	#define CORAL_OS_WIN
#else
	#error Oops - unknown or unsupported OS!
#endif

#if !defined(CORAL_OS_WIN)
	#define CORAL_OS_UNIX
#endif

//------ Recognized compilers (CORAL_CC_x) -------------------------------------

#if defined(_MSC_VER)
	#define CORAL_CC_MSVC
#elif defined(__GNUC__)
	#define CORAL_CC_GNU
	#if defined(__MINGW32__)
		#define CORAL_CC_MINGW
	#endif
#endif

//------ Determine the CORAL_BUILD_KEY -----------------------------------------

#if defined(CORAL_OS_LINUX)
	#define CORAL_OS_NAME "Linux"
#elif defined(CORAL_OS_MAC)
	#define CORAL_OS_NAME "MacOSX"
#elif defined(CORAL_OS_WIN)
	#define CORAL_OS_NAME "Windows"
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

#if defined(CORAL_CC_MSVC)
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
#elif defined(CORAL_CC_GNU)
	#if defined(CORAL_CC_MINGW)
		#define CORAL_CC_NAME "mingw"
	#else
		#define CORAL_CC_NAME "g++"
	#endif
	#define CORAL_CC_VERSION CORAL_STRINGIFY(__GNUC__) "." CORAL_STRINGIFY(__GNUC_MINOR__)
#else
	#error "Unsupported compiler! Coral requires GCC or MSVC."
#endif

#define CORAL_BUILD_KEY		CORAL_OS_NAME " " CORAL_ARCH_NAME " " CORAL_CC_NAME "-" CORAL_CC_VERSION


//------ Portable Integer Types ------------------------------------------------

#if defined(CORAL_CC_GNU)
	#include <sys/types.h>
#endif

//! Coral's main namespace.
namespace co {

#if CORAL_POINTER_SIZE == 4
	typedef char				int8;
	typedef unsigned char		uint8;
	typedef short				int16;
	typedef unsigned short		uint16;
	typedef long				int32;
	typedef unsigned long		uint32;
	typedef long long			int64;
	typedef unsigned long long	uint64;
	typedef int32				intptr;
	typedef uint32				uintptr;
#elif CORAL_POINTER_SIZE == 8
	typedef char				int8;
	typedef unsigned char		uint8;
	typedef short				int16;
	typedef unsigned short		uint16;
	typedef int					int32;
	typedef unsigned int		uint32;
	#ifdef CORAL_CC_MSVC
		typedef long long			int64;
		typedef unsigned long long	uint64;
	#else
		typedef long				int64;
		typedef unsigned long		uint64;
	#endif
	typedef int64				intptr;
	typedef uint64				uintptr;
#else
	#error Portable integers were not defined because CORAL_POINTER_SIZE is invalid.
#endif

// Limits for the portable integer types.
// (current definitions are only valid for x86 architectures)
const int8		MIN_INT8	= -127 - 1;
const int8		MAX_INT8	=  127;
const uint8		MAX_UINT8	=  0xFF;

const int16		MIN_INT16	= -32767 - 1;
const int16		MAX_INT16	=  32767;
const uint16	MAX_UINT16	= 0xFFFF;

const int32		MIN_INT32	= -2147483647 - 1;
const int32		MAX_INT32	=  2147483647;
const uint32	MAX_UINT32	=  0xFFFFFFFF;

#if defined(CORAL_CC_MSVC)
const int64		MAX_INT64	= 0x7FFFFFFFFFFFFFFF;
const uint64	MAX_UINT64	= 0xFFFFFFFFFFFFFFFF;
#else
const int64		MAX_INT64	= 0x7FFFFFFFFFFFFFFFLL;
const uint64	MAX_UINT64	= 0xFFFFFFFFFFFFFFFFULL;
#endif

const int64		MIN_INT64	= -MAX_INT64 - 1;

} // namespace co

/*!
	\brief Imports all portable integer types into the current namespace.

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
	\brief Evaluates a constant expression and, if the result is false,
	aborts the compilation with an error message.
	\param[in] const_expr compile-time integral or pointer expression.
	\param[in] id_msg C++ identifier that describes the error (it does not need to be defined).
		Something like 'invalid_element_size'.
 */
#define CORAL_STATIC_CHECK( const_expr, id_msg ) \
    { co::CompileTimeError<( (const_expr) != 0 )> ERROR_##id_msg; (void)ERROR_##id_msg; }

/*!
	\brief Returns the number of elements in a statically-allocated array.
	\param[in] array name of a statically-allocated array.
 */
#define CORAL_ARRAY_LENGTH( array )	( sizeof( array ) / sizeof( array[0] ) )

/*!
	\def CORAL_INT64_C( c )
	\brief Wraps the signed 64-bit integer literal 'c' in a platform-independent way.

	\def CORAL_UINT64_C( uc )
	\brief Wraps the unsigned 64-bit integer literal 'uc' in a platform-independent way.
 */
#if defined(CORAL_OS_WIN) && !defined(CORAL_CC_GNU)
	#define CORAL_INT64_C( c ) c ## i64
	#define CORAL_UINT64_C( uc ) uc ## ui64
#elif CORAL_POINTER_SIZE == 8 && !defined(CORAL_OS_MAC)
	#define CORAL_INT64_C( c ) c ## L
	#define CORAL_UINT64_C( uc ) uc ## UL
#else
	#define CORAL_INT64_C( c ) c ## LL
	#define CORAL_UINT64_C( uc ) uc ## ULL
#endif

//! Supresses 'unused parameter' warnings.
#define CORAL_UNUSED( x ) (void)x;

/*!
	\def CORAL_FORCE_INLINE
	\brief Portable function attribute to force inlining.
	\def CORAL_NO_INLINE
	\brief Portable function attribute to forbid inlining.
 */
#if defined(CORAL_CC_MSVC)
	#define CORAL_FORCE_INLINE __forceinline 
	#define CORAL_NO_INLINE __declspec(noinline)
#else // assumes the compiler is GCC-compatible 
	#define CORAL_FORCE_INLINE __attribute__((always_inline)) 
	#define CORAL_NO_INLINE __attribute__((noinline)) 
#endif 

namespace co {

#ifndef DOXYGEN

// Template trick to yield compile time errors:
template<int> struct CompileTimeError;
template<> struct CompileTimeError<true> {};

#endif // DOXYGEN

} // namespace co

//------ Portable shared-library interface attributes --------------------------

#ifndef DOXYGEN

#if defined(CORAL_OS_WIN) && !defined(CORAL_NO_EXPORT)
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

#endif // DOXYGEN

#endif // _CO_PLATFORM_H_
