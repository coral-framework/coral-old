/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#ifndef _PLATFORMUTILS_H_
#define _PLATFORMUTILS_H_

#include <core/tools/FileSystem.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cassert>
#include <sstream>
#include <sys/stat.h>

/*
	Misc platform-specific definitions.
 */
#if defined(CORAL_OS_WIN)
	#include <direct.h>
	#define getCWD _getcwd
	#define DIR_SEP_STR "\\"
	#define PATH_SEP_STR ";"
	#define EXE_SUFFIX ".exe"
#else
	#include <errno.h>
	#include <unistd.h>
	#include <sys/wait.h>
	#define getCWD getcwd
	#define DIR_SEP_STR "/"
	#define PATH_SEP_STR ":"
	#define EXE_SUFFIX
#endif

/*
	Function to get the current executable's directory.
 */
#if defined(CORAL_OS_WIN)
	#define WIN32_LEAN_AND_MEAN
	#include <Windows.h>
#elif defined(CORAL_OS_MAC)
	#include <mach-o/dyld.h>
#endif

inline bool getCurrentExecutableDir( std::string& dir )
{
	char buffer[FILENAME_MAX];

#if defined(CORAL_OS_WIN)
	DWORD count;
	if( ( count = GetModuleFileNameA( NULL, buffer, sizeof(buffer) ) ) == 0 )
		return false;
	dir.assign( buffer, count );
#elif defined(CORAL_OS_LINUX)
	ssize_t count = readlink( "/proc/self/exe", buffer, sizeof(buffer) - 1 );
	if( count == -1 )
		return false;
	dir.assign( buffer, count );
#elif defined(CORAL_OS_MAC)
	uint32_t count = sizeof(buffer);
	if( _NSGetExecutablePath( buffer, &count ) != 0 )
		return false;
	char* realPath = realpath( buffer, NULL );
	dir = realPath;
	free( realPath );
#else
	#error Unknown or unsupported platform.
#endif

	size_t lastSep = dir.rfind( DIR_SEP_STR );
	if( lastSep != std::string::npos )
		dir.resize( lastSep );

	return true;
}

/*
	Function to get the current working directory.
 */
inline bool getCurrentWorkingDir( std::string& dir )
{
	char buffer[FILENAME_MAX];
	if( !getCWD( buffer, sizeof(buffer) ) )
		return false;
	dir = buffer;
	return true;
}

/*
	Function to test whether a directory exists.
 */
inline bool dirExists( const std::string& path )
{
	CORAL_STAT_STRUCT info;
	if( CORAL_STAT_FUNC( path.c_str(), &info ) )
		return false;
	return S_ISDIR( info.st_mode ) != 0;
}

/*
	Function to test whether a file exists (and is not a dir).
 */
inline bool fileExists( const std::string& path )
{
	CORAL_STAT_STRUCT info;
	if( CORAL_STAT_FUNC( path.c_str(), &info ) )
		return false;
	return S_ISREG( info.st_mode ) != 0;
}

/*
	Function to set the value of an environment variable.
 */
inline void setEnvVar( const char* name, const std::string& value )
{
	int ok;
#if defined(CORAL_OS_WIN)
	ok = SetEnvironmentVariableA( name, value.c_str() );
#else
	ok = !setenv( name, value.c_str(), 1 );
#endif
	if( !ok )
		fprintf( stderr, "ERROR: could not set the environment variable '%s' to '%s'.\n", name, value.c_str() );
}

/*
	Function to prepend a string to an environment variable.
 */
inline void prependToEnvVar( const char* name, const std::string& prefix )
{
	std::string value( prefix );
	char* currentValue = getenv( name );
	if( currentValue )
		value += currentValue;
	setEnvVar( name, value );
}

/*
	Function to execute a program passing along an array of arguments.
 */
inline int executeProgram( int argc, char* const* argv )
{
#if defined(CORAL_OS_WIN)
	STARTUPINFO         si;
    PROCESS_INFORMATION pi;
    memset( &si, 0, sizeof(si) );
    memset( &pi, 0, sizeof(pi) );
    si.cb = sizeof(si);

	std::stringstream ss;
	for( int i = 0; i < argc; ++i )
	{
		if( i > 0 ) ss << ' ';
		ss << '"' << argv[i] << '"';
	}

	std::string cmdLine = ss.str();
	if( CreateProcessA( 0, const_cast<LPSTR>( cmdLine.c_str() ), 0, 0, 0, CREATE_DEFAULT_ERROR_MODE, 0, 0, &si, &pi ) == FALSE )
	{
		fprintf( stderr, "ERROR: could not execute '%s' (error code %i)\n", argv[0], GetLastError() );
		return 127;
	}
	
	// wait until the program has terminated
	WaitForSingleObject( pi.hProcess, INFINITE );

	DWORD exitCode;
	GetExitCodeProcess( pi.hProcess, &exitCode );
	return static_cast<int>( exitCode );

#else

	CORAL_UNUSED( argc );
	pid_t pID = fork();
	if( pID == 0 ) // child
	{
		execv( argv[0], argv );

		// if execv returns, it must have failed
		fprintf( stderr, "ERROR: could not execute '%s' (%s)\n", argv[0], strerror( errno ) );
		return 127;
	}
	else if( pID > 0 ) // parent
	{
		int status;
		if( waitpid( pID, &status, WUNTRACED ) != pID )
		{
			fprintf( stderr, "ERROR: waitpid() failed (%s)\n", strerror( errno ) );
			return 127;
		}
		return WEXITSTATUS( status );
	}
	else // failed to fork
	{
		fprintf( stderr, "ERROR: could not fork.\n" );
		return 127;
	}
#endif
}

#endif // _PLATFORMUTILS_H_
