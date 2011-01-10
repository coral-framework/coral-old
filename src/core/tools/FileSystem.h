/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#ifndef _CO_FILESYSTEM_H_
#define _CO_FILESYSTEM_H_

//! \file FileSystem.h Auxiliary platform-specific structures and macros for filesystem operations.

#include <co/Platform.h>
#include <string>

// enable large file support on Linux
#ifndef _FILE_OFFSET_BITS
#define _FILE_OFFSET_BITS 64
#endif
#ifndef _LARGEFILE64_SOURCE
#define _LARGEFILE64_SOURCE
#endif

#ifdef CORAL_OS_WIN
	#if _INTEGRAL_MAX_BITS >= 64
		#define CORAL_STAT_STRUCT struct _stati64
		#define CORAL_STAT_FUNC _stati64
	#else
		#define CORAL_STAT_STRUCT struct _stati32
		#define CORAL_STAT_FUNC _stati32
	#endif
#else
	#define CORAL_STAT_STRUCT struct stat
	#define CORAL_STAT_FUNC stat
#endif

#ifdef CORAL_OS_WIN
	#ifndef S_ISDIR
		#define S_ISDIR( mode )  ( mode & _S_IFDIR )
	#endif
	#ifndef S_ISREG
		#define S_ISREG( mode )  ( mode & _S_IFREG )
	#endif
#endif

#endif // _CO_FILESYSTEM_H_
