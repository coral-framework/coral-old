/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#include "Utils.h"

#if defined(CORAL_OS_WIN)
	#define EXE_SUFFIX ".exe"
#else
	#define EXE_SUFFIX
#endif

int main( int argc, char* argv[] )
{
	// set CORAL_ROOT = this executable's dir
	std::string rootDir;
	co::OS::getApplicationDir( rootDir );
	setEnvVar( "CORAL_ROOT", rootDir );

	// add $CORAL_ROOT/modules to the CORAL_PATH
	prependToEnvVar( "CORAL_PATH", rootDir + ( CORAL_OS_DIR_SEP_STR "modules" CORAL_OS_PATH_SEP_STR ) );

	// set the OS-specific var that enables the system to locate the coral library
	std::string libDir( rootDir );
	libDir += CORAL_OS_DIR_SEP_STR "lib" CORAL_OS_PATH_SEP_STR;
#if defined(CORAL_OS_WIN)
	prependToEnvVar( "PATH", libDir );
#elif defined(CORAL_OS_LINUX)
	prependToEnvVar( "LD_LIBRARY_PATH", libDir );
#elif defined(CORAL_OS_MAC)
	prependToEnvVar( "DYLD_LIBRARY_PATH", libDir );
#else
	#error Unknown or unsupported platform.
#endif
	
	/*
		Process command-line options. Currently, only one option is supported,
		and it MUST be specified at the start of the command-line:

		--mode <string>
			Indicates in which mode to run the Coral Application Launcher.
			Any string other than 'Debug' (case sensitive) is interpreted as 'Release'.
			If this option is specified and no launcher is available under the requested
			mode, an error is issued. However, if this option is omitted, the front-end
			will use any available launcher, giving preference to one in Release mode.
	 */
	const char* launcherReleasePath = CORAL_OS_PATH_SEP_STR "bin" CORAL_OS_PATH_SEP_STR "launcher" EXE_SUFFIX;
	const char* launcherDebugPath = CORAL_OS_PATH_SEP_STR "bin" CORAL_OS_PATH_SEP_STR "launcher_debug" EXE_SUFFIX;

	int argIndex = 1;
	std::string launcher;

	if( argc >= 2 && strcmp( argv[1], "--mode" ) == 0 )
	{
		argIndex = 3;
		if( strcmp( argv[2], "Debug" ) == 0 )
			launcher = rootDir + launcherDebugPath;
		else
			launcher = rootDir + launcherReleasePath;
	}
	else // automatically choose a launcher executable
	{
		launcher = rootDir + launcherReleasePath;
		if( !co::OS::isFile( launcher ) )
		{
			// fallback to the debug executable
			launcher = rootDir + launcherDebugPath;
		}
	}
	
	if( !co::OS::isFile( launcher ) )
	{
		fprintf( stderr, "ERROR: the launcher executable is not available (%s)\n", launcher.c_str() );
		return -1;
	}

	/*
		Prepare the command-line to invoke the Coral Application Launcher.
		first arg = program path; last arg = NULL;
	 */
	int numArgs = ( argc - argIndex ) + 1;
	char** args = new char*[numArgs + 1];
	args[0] = const_cast<char*>( launcher.c_str() );
	for( int i = 1; i < numArgs; ++i )
		args[i] = argv[argIndex++];
	args[numArgs] = NULL;

	int res = executeProgram( numArgs, args );

	delete[] args;
	
	return res;
}
