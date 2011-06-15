/*
 * Coral - Lightweight C++ Component Framework
 * See Copyright Notice in Coral.h
 */

#include "Utils.h"
#include <core/tools/Properties.h>
#include <core/tools/StringTokenizer.h>

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

	co::Properties properties;
	properties.load( rootDir + ( CORAL_OS_DIR_SEP_STR "coral.properties" ) );

	// if dir "$CORAL_ROOT/modules" exists, add it to the CORAL_PATH
	std::string coralPath( rootDir );
	coralPath.append( CORAL_OS_DIR_SEP_STR "modules" CORAL_OS_PATH_SEP_STR );
	if( !co::OS::isDir( coralPath ) )
		coralPath.clear();

	// if property "path" is defined, add it to the CORAL_PATH
	const std::string& pathProperty = properties.getProperty( "path" );
	if( !pathProperty.empty() )
	{
		coralPath.reserve( coralPath.size() + pathProperty.size() + 1 );
		if( !coralPath.empty() )
			coralPath.append( CORAL_OS_PATH_SEP_STR );
		coralPath.append( pathProperty );
	}

	// add entries to the CORAL_PATH environment variable
	prependToEnvVar( "CORAL_PATH", coralPath );

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
		and if specified, it MUST be at the start of the command-line:

		--mode <string>
			Indicates in which mode to run the Coral Application Launcher.
			Any string other than 'Debug' (case sensitive) is interpreted as 'Release'.
			If this option is specified and no launcher is available under the requested
			mode, an error is issued. However, if this option is omitted, the front-end
			will use any available launcher, giving preference to one in Release mode.
	 */
	const char* launcherReleasePath = CORAL_OS_DIR_SEP_STR "bin" CORAL_OS_DIR_SEP_STR "launcher" EXE_SUFFIX;
	const char* launcherDebugPath = CORAL_OS_DIR_SEP_STR "bin" CORAL_OS_DIR_SEP_STR "launcher_debug" EXE_SUFFIX;

	int argIndex = 1;
	std::string launcher;

	if( argc >= 2 && strCaseComp( argv[1], "--mode" ) == 0 )
	{
		argIndex = 3;
		if( strCaseComp( argv[2], "debug" ) == 0 )
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
	std::vector<char*> args;
	args.reserve( argc );
	args.push_back( const_cast<char*>( launcher.c_str() ) );

	// if property "args" was defined, parse its arguments first
	std::vector<std::string> stringStorage;
	const std::string& argsProperty = properties.getProperty( "args" );
	if( !argsProperty.empty() )
	{
		co::StringTokenizer st( argsProperty, ";" );
		while( st.nextToken() )
		{
			stringStorage.push_back( st.getToken() );
			args.push_back( const_cast<char*>( stringStorage.back().c_str() ) );
		}
	}

	// append all unprocessed command-line args
	for( ; argIndex <= argc; ++argIndex )
		args.push_back( argv[argIndex] );

	args.push_back( NULL );

	return executeProgram( args.size(), &args[0] );
}
