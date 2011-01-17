/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#ifndef _BIN_UTILS_H_
#define _BIN_UTILS_H_

#include <co/reserved/OS.h>
#include <cerrno>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cassert>
#include <sstream>

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

#endif // _BIN_UTILS_H_
