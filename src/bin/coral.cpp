/*
 * Coral - Lightweight C++ Component Framework
 * See Copyright Notice in Coral.h
 */

#include "Utils.h"
#include <core/utils/Properties.h>
#include <core/utils/StringTokenizer.h>

#if defined(CORAL_OS_WIN)
	#define EXE_SUFFIX ".exe"
#else
	#define EXE_SUFFIX
#endif

enum Mode
{
	Mode_Debug,
	Mode_Release,
	Mode_Auto
};

Mode resolveMode( const char* str )
{
	return strCaseComp( str, "debug" ) == 0 ? Mode_Debug : Mode_Release;
}

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
	coralPath.append( CORAL_OS_DIR_SEP_STR "modules" );
	if( !co::OS::isDir( coralPath ) )
		coralPath.clear();

	// if property "path" is defined, add it to the CORAL_PATH
	const std::string& pathProperty = properties.getProperty( "path" );
	if( !pathProperty.empty() )
	{
		coralPath.reserve( coralPath.size() + pathProperty.size() + 2 );
		coralPath.append( CORAL_OS_PATH_SEP_STR );
		coralPath.append( pathProperty );
	}

	coralPath.append( CORAL_OS_PATH_SEP_STR );

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

	// determine in which mode to run the launcher
	Mode mode = Mode_Auto;
	const std::string& modeProperty = properties.getProperty( "mode" );
	if( !modeProperty.empty() )
		mode = resolveMode( modeProperty.c_str() );

	/*
		Process command-line options. Currently, only one option is supported,
		and if specified, it MUST be at the start of the command-line:

		--mode <string>
			Indicates in which mode to run the Coral Application Launcher.
			Any string other than 'debug' is interpreted as 'release'.
			If this option is specified and no launcher is available under the requested
			mode, an error is issued. However, if this option is omitted, the front-end
			will use any available launcher, giving preference to one in release mode.
	 */
	int argIndex = 1;
	if( argc >= 2 && strCaseComp( argv[1], "--mode" ) == 0 )
	{
		argIndex = 3;
		mode = resolveMode( argv[2] );
	}

	// determine the launcher executable that should be used
	std::string launcher;

	std::string launcherRelease( rootDir );
	launcherRelease.append( CORAL_OS_DIR_SEP_STR "bin" CORAL_OS_DIR_SEP_STR "launcher" EXE_SUFFIX );

	std::string launcherDebug( rootDir );
	launcherDebug.append( CORAL_OS_DIR_SEP_STR "bin" CORAL_OS_DIR_SEP_STR "launcher_debug" EXE_SUFFIX );

	switch( mode )
	{
	case Mode_Debug:
		launcher = properties.getProperty( "launcher_debug", launcherDebug );
		break;

	case Mode_Release:
		launcher = properties.getProperty( "launcher", launcherRelease );
		break;

	case Mode_Auto:
		/*
			Automatically choose a launcher executable. Search priority:
				1) bin/launcher
				2) bin/launcher_debug
				3) launcher
				4) launcher_debug
		 */

		if( co::OS::isFile( launcherRelease ) )
			launcher = launcherRelease;
		else if( co::OS::isFile( launcherDebug ) )
			launcher = launcherDebug;
		else
		{
			launcher = rootDir + ( CORAL_OS_DIR_SEP_STR "launcher" EXE_SUFFIX );
			if( !co::OS::isFile( launcher ) )
				launcher = rootDir + ( CORAL_OS_DIR_SEP_STR "launcher_debug" EXE_SUFFIX );
		}
		break;

	default:
		assert( false );
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

	// if property "args" was defined, prepend its arguments first
	std::vector<std::string> prependedArgs;
	const std::string& argsProperty = properties.getProperty( "args" );
	if( !argsProperty.empty() )
	{
		co::StringTokenizer st( argsProperty, ";" );
		while( st.nextToken() )
			prependedArgs.push_back( st.getToken() );
	}

	// append all args in prependedArgs
	for( size_t i = 0; i < prependedArgs.size(); ++i )
		args.push_back( const_cast<char*>( prependedArgs[i].c_str() ) );

	// append all unprocessed command-line args
	for( ; argIndex < argc; ++argIndex )
		args.push_back( argv[argIndex] );

	args.push_back( NULL );

	return executeProgram( &args[0] );
}
