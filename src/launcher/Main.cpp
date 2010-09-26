/*
 * Coral - A C++ Component Framework.
 * See Copyright Notice in Coral.h
 */

#include <co/Coral.h>
#include <co/System.h>
#include <co/Component.h>
#include <co/Exception.h>
#include <co/Reflector.h>
#include <co/MethodInfo.h>
#include <co/ComponentType.h>
#include <co/InterfaceInfo.h>
#include <co/InterfaceType.h>
#include <co/ParameterInfo.h>
#include <core/tools/FileSystem.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cassert>
#include <sstream>
#include <iostream>
#include <sys/stat.h>

/*
	Misc platform-specific definitions.
 */
#if defined(CORAL_OS_WIN)
	#include <direct.h>
	#define getCWD _getcwd
	const char DIR_SEP_CHAR = '\\';
#else
	#include <unistd.h>
	#define getCWD getcwd
	const char DIR_SEP_CHAR = '/';
#endif

/*
	Function to check if a directory exists.
 */
static bool dirExists( const std::string& path )
{
	CORAL_STAT_STRUCT info;
	if( CORAL_STAT_FUNC( path.c_str(), &info ) )
		return false;
	return S_ISDIR( info.st_mode ) != 0;
}

/*
	Function to get the current executable's directory.
 */
#if defined(CORAL_OS_WIN)
	#define WIN32_LEAN_AND_MEAN
	#include <Windows.h>
#elif defined(CORAL_OS_LINUX)
	#include <unistd.h>
#elif defined(CORAL_OS_MAC)
	#include <mach-o/dyld.h>
#else
	#error This platform is currently unsupported.
#endif

static bool getCurrentExecutableDir( std::string& dir )
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
	#error This platform is currently unsupported.
#endif

	size_t lastSep = dir.rfind( DIR_SEP_CHAR );
	if( lastSep != std::string::npos )
		dir.resize( lastSep );

	return true;
}

/*
	Function to get the current working directory.
 */
static bool getCurrentWorkingDir( std::string& dir )
{
	char buffer[FILENAME_MAX];
	if( !getCWD( buffer, sizeof(buffer) ) )
		return false;
	dir = buffer;
	return true;
}

/*
	Helper function to initialize the Coral path.
 */
static void addDefaultPaths()
{
#if defined(CORAL_PATH)
	// if specified at compile time, add the contents of 'CORAL_PATH' to the path;
	co::addPath( CORAL_PATH );
#endif

	// if the CORAL_PATH environment variable is specified, add it to the path
	char* coralPathEnv = getenv( "CORAL_PATH" );
	if( coralPathEnv )
		co::addPath( coralPathEnv );

	// add the '~/modules' dir (relative to the executable's dir) to the path
	std::string path;
	if( getCurrentExecutableDir( path ) )
	{
		path.append( "/modules" );
		if( dirExists( path ) )
			co::addPath( path );
	}
	else
	{
		std::cerr << "Oops! Could not determine the current executable's path.\n";
	}

	// add the current working dir to the path
	if( getCurrentWorkingDir( path ) )
	{
		// if the CWD contains a './modules' subdir, add it instead
		std::string subDir( path );
		subDir.append( "/modules" );
		if( dirExists( subDir ) )
			co::addPath( subDir );
		else
			co::addPath( path );
	}
	else
		std::cerr << "Oops! Could not determine the current working directory.\n";
}

void resolveCallee( const std::string& calleeName, co::ComponentType*& ct, co::InterfaceInfo*& ii, co::MethodInfo*& mi )
{
	// was a method name specified?
	size_t colonPos = calleeName.rfind( ':' );
	if( colonPos == std::string::npos )
	{
		// resolve component type
		co::Type* type = co::getType( calleeName );
		ct = dynamic_cast<co::ComponentType*>( type );
		if( !ct )
			CORAL_THROW( co::Exception, "'" << calleeName << "' is not a component." );
	
		// search the component's list of provided interfaces
		co::ArrayRange<co::InterfaceInfo* const> serverItfs = ct->getProvidedInterfaces();
		for( ; serverItfs; serverItfs.popFirst() )
		{
			ii = serverItfs.getFirst();
			co::MemberInfo* member = ii->getType()->getMember( "main" );
			if( member && ( mi = dynamic_cast<co::MethodInfo*>( member ) ) )
				return;
		}

		CORAL_THROW( co::Exception, "none of " << calleeName << "'s interfaces has a main() method." );
	}
	
	// parse the component.interface:method names

	std::string methodName( calleeName, colonPos + 1 );

	size_t interfaceDotPos = calleeName.rfind( '.', colonPos );
	if( interfaceDotPos == std::string::npos )
		CORAL_THROW( co::Exception, "both a component and interface name must be specified before the method name." );

	std::string interfaceName( calleeName, interfaceDotPos + 1, colonPos - interfaceDotPos - 1 );
	std::string componentName( calleeName, 0, interfaceDotPos );

	// resolve component type
	co::Type* type = co::getType( componentName );
	ct = dynamic_cast<co::ComponentType*>( type );
	if( !ct )
		CORAL_THROW( co::Exception, "'" << componentName << "' is not a component." );

	// resolve interface type
	co::MemberInfo* member = ct->getMember( interfaceName );
	if( !member )
		CORAL_THROW( co::Exception, "'" << componentName << "' has no interface named '" << interfaceName << "'." );

	ii = dynamic_cast<co::InterfaceInfo*>( member );
	assert( ii );

	if( !ii || !ii->getIsProvided() )
		CORAL_THROW( co::Exception, "'" << interfaceName << "' is not a provided interface of component '"
						<< componentName << "'." );

	// resolve method
	member = ii->getType()->getMember( methodName );
	if( !member )
		CORAL_THROW( co::Exception, "interface '" << ii->getType()->getFullName() << "' has no method named '"
						<< methodName << "'." );

	mi = dynamic_cast<co::MethodInfo*>( member );
	if( !mi )
		CORAL_THROW( co::Exception, "interface '" << ii->getType()->getFullName() << "' has a member named '"
						<< methodName << "', but it is not a method." );
}

/*
	The main() function.
 */
int main( int argc, char* argv[] )
{
	if( argc < 2 || ( argv[1][0] == '-' && argc < 4 ) )
	{
		std::cout <<
			"Coral Application Launcher v" CORAL_VERSION_STR " (" CORAL_BUILD_KEY ")\n"
			"Usage: coral [options] callee [ARG] ...\n"
			"Description:\n"
			"  A 'callee' must be specified as either a component or a method name within\n"
			"  a component interface. If a method is not specified, one named 'main' will be\n"
			"  searched for in all provided interfaces of the component. The called method\n"
			"  must receive either no argument or an array of strings. If the method returns\n"
			"  a number, it will be used as the application return status."
			"Examples:\n"
			"  coral myModule.MyComponent arg1 arg2 arg3\n"
			"  coral someModule.SomeComponent.someInterface:someMethod arg1 arg2\n"
			"Available Options:\n"
			"  -p EXTRA,DIRS   Add a list of repositories to the Coral path.\n";

		return 0;
	}

	int index = 1;
	if( argv[1][0] == '-' )
	{
		index = 3;
		if( argv[1][1] == 'p' && argv[1][2] == '\0' )
			co::addPath( argv[2] );
		else
			std::cerr << "Unrecognized flag '" << argv[1] << "'.\n";
	}

	assert( argc >= index );

	addDefaultPaths();

	int status = 0;

	try
	{
		// initialize the Coral framework
		try
		{
			co::getSystem()->setup();
		}
		catch( std::exception& e )
		{
			std::cerr << "System initialization failed.\nException: " << e.what() << "\n";
			throw;
		}

		// resolve the callee
		co::ComponentType* ct = NULL;
		co::InterfaceInfo* ii = NULL;
		co::MethodInfo* mi = NULL;
		try
		{
			resolveCallee( argv[index], ct, ii, mi );
			assert( ct && ii && mi );
		}
		catch( std::exception& e )
		{
			std::cerr << "Invalid callee '" << argv[index] << "'.\nException: " << e.what() << "\n";
			throw;
		}

		// check the method signature
		try
		{
			co::ArrayRange<co::ParameterInfo* const> params = mi->getParameters();
			if( !params.isEmpty() && ( params.getSize() > 1 || params[0]->getType()->getFullName() != "string[]" ) )
				throw co::Exception( "method can only have a single parameter, of type 'string[]'" );
		}
		catch( std::exception& e )
		{
			std::cerr << "Invalid method '" << ii->getType()->getFullName() << ":"
				<< mi->getName().c_str() << "'.\nException: " << e.what() << "\n";
			throw;
		}

		// instantiate the component & obtain the interface instance
		co::RefPtr<co::Component> component;
		co::Interface* itf;
		try
		{
			component = ct->getReflector()->newInstance();
			assert( component.isValid() );

			itf = component->getInterface( ii );
			assert( itf );
		}
		catch( std::exception& e )
		{
			std::cerr << "Instantiation failed: " << e.what() << "\n";
			throw;
		}
		
		// prepare to invoke the method
		co::Reflector* reflector;
		try
		{
			reflector = mi->getOwner()->getReflector();
		}
		catch( std::exception& e )
		{
			std::cerr << "Invocation failed: " << e.what() << "\n";
			throw;
		}

		// invoke the method
		try
		{
			std::vector<std::string> args;
			while( ++index < argc )
				args.push_back( argv[index] );

			co::Any res;
			co::Any arg( &args );
			reflector->invokeMethod( itf, mi, co::ArrayRange<co::Any const>( &arg, 1 ), res );

			// if the result is a number, use it as the return status; otherwise, print it
			if( res.isValid() )
			{
				if( res.getKind() >= co::TK_INT8 && res.getKind() <= co::TK_DOUBLE )
					status = res.get<int>();
				else
					std::cout << "Method returned " << res << "\n";
			}
		}
		catch( std::exception& e )
		{
			std::cerr << "Program terminated with an unhandled exception.\nMessage: " << e.what() << "\n";
			throw;
		}
	}
	catch( std::exception& )
	{
		status = -1;
	}

	co::shutdown();

	return status;
}
