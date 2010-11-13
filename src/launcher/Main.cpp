/*
 * Coral - Lightweight C++ Component Framework.
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
#include <co/ModuleManager.h>
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
		fprintf( stderr, "Oops! Could not determine the current executable's path.\n" );
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
	{
		fprintf( stderr, "Oops! Could not determine the current working directory.\n" );
	}

#if defined(CORAL_PATH)
	// if specified at compile time, add the contents of 'CORAL_PATH' to the path;
	co::addPath( CORAL_PATH );
#endif
}

void resolveCallee( const std::string& calleeName, co::ComponentType*& ct, co::InterfaceInfo*& facet, co::MethodInfo*& method )
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

		// search the component's list of facets
		co::ArrayRange<co::InterfaceInfo* const> facets = ct->getFacets();
		for( ; facets; facets.popFirst() )
		{
			facet = facets.getFirst();
			co::MemberInfo* member = facet->getType()->getMember( "main" );
			if( member && ( method = dynamic_cast<co::MethodInfo*>( member ) ) )
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

	facet = dynamic_cast<co::InterfaceInfo*>( member );
	assert( facet );

	if( !facet || !facet->getIsFacet() )
		CORAL_THROW( co::Exception, "'" << interfaceName << "' is not a facet of component '" << componentName << "'." );

	// resolve method
	member = facet->getType()->getMember( methodName );
	if( !member )
		CORAL_THROW( co::Exception, "interface '" << facet->getType()->getFullName()
						<< "' has no method named '" << methodName << "'." );

	method = dynamic_cast<co::MethodInfo*>( member );
	if( !method )
		CORAL_THROW( co::Exception, "interface '" << facet->getType()->getFullName()
						<< "' has a member named '" << methodName << "', but it is not a method." );
}

/*
	The main() function.
 */
int main( int argc, char* argv[] )
{
	int index = 1;

	// process command-line options
	bool abiChecks = true;
	while( index < argc && argv[index][0] == '-' )
	{
		if( argv[index][1] == 'p' && argv[index][2] == '\0' )
		{
			if( ++index >= argc )
			{
				fprintf( stderr, "Missing dir list after option '-p'." );
				return EXIT_FAILURE;
			}
			co::addPath( argv[index] );
		}
		else if( strcmp( argv[index] + 1, "-no-abi-checks" ) == 0 )
		{
			abiChecks = false;
		}
		else
		{
			fprintf( stderr, "Unrecognized flag: %s\n", argv[index] );
		}
		++index;
	}

	// print help if not enough args were passed
	if( index >= argc )
	{
		printf(
			"Coral Application Launcher v" CORAL_VERSION_STR " (" CORAL_BUILD_KEY ")\n"
			"Usage: coral [options] callee [ARG] ...\n"
			"Description:\n"
			"  A 'callee' must be specified as either a component or a method name within\n"
			"  a component interface. If a method is not specified, one named 'main' will be\n"
			"  searched for in all facets of the component. The called method must receive\n"
			"  either no argument or an array of strings. If the method returns a number, it\n"
			"  will be used as the application's return status."
			"Examples:\n"
			"  coral myModule.MyComponent arg1 arg2 arg3\n"
			"  coral someModule.SomeComponent.someInterface:someMethod arg1 arg2\n"
			"Available Options:\n"
			"  -p EXTRA,DIRS    Add a list of repositories to the Coral path.\n"
			"  --no-abi-checks  Disable ABI compatibility checks when loading modules.\n"
		);
		return EXIT_FAILURE;
	}

	addDefaultPaths();

	int exitStatus = EXIT_SUCCESS;
	try
	{
		// initialize the Coral framework
		co::System* system;
		try
		{
			system = co::getSystem();
			system->getModules()->setBinaryCompatibilityChecking( abiChecks );
			system->setup();
		}
		catch( std::exception& e )
		{
			fprintf( stderr, "System initialization failed.\nException: %s\n", e.what() );
			throw;
		}

		// resolve the callee
		co::ComponentType* ct = NULL;
		co::InterfaceInfo* facet = NULL;
		co::MethodInfo* method = NULL;
		try
		{
			resolveCallee( argv[index], ct, facet, method );
			assert( ct && facet && method );
		}
		catch( std::exception& e )
		{
			fprintf( stderr, "Invalid callee '%s'.\nException: %s\n", argv[index], e.what() );
			throw;
		}

		// check the method signature
		try
		{
			co::ArrayRange<co::ParameterInfo* const> params = method->getParameters();
			if( !params.isEmpty() && ( params.getSize() > 1 || params[0]->getType()->getFullName() != "string[]" ) )
				throw co::Exception( "method can only have a single parameter, of type 'string[]'" );
		}
		catch( std::exception& e )
		{
			fprintf( stderr, "Invalid method '%s:%s'.\nException: %s\n",
					facet->getType()->getFullName().c_str(), method->getName().c_str(), e.what() );
			throw;
		}

		// instantiate the component & obtain the interface instance
		co::RefPtr<co::Component> component;
		co::Interface* itf;
		try
		{
			component = ct->getReflector()->newInstance();
			assert( component.isValid() );

			itf = component->getInterface( facet );
			assert( itf );
		}
		catch( std::exception& e )
		{
			fprintf( stderr, "Instantiation failed: %s\n", e.what() );
			throw;
		}

		// prepare to invoke the method
		co::Reflector* reflector;
		try
		{
			reflector = method->getOwner()->getReflector();
		}
		catch( std::exception& e )
		{
			fprintf( stderr, "Invocation failed: %s\n", e.what() );
			throw;
		}

		// invoke the method
		try
		{
			std::vector<std::string> args;
			while( ++index < argc )
				args.push_back( argv[index] );

			co::Any res;
			co::Any arg;
			arg.set<std::vector<std::string>&>( args );
			reflector->invokeMethod( itf, method, co::ArrayRange<co::Any const>( &arg, 1 ), res );

			// if the result is a number, use it as the return status; otherwise, print it
			if( res.isValid() )
			{
				if( res.getKind() >= co::TK_INT8 && res.getKind() <= co::TK_DOUBLE )
				{
					exitStatus = res.get<int>();
				}
				else
				{
					std::stringstream ss;
					ss << res;
					printf( "Method returned %s\n", ss.str().c_str() );
				}
			}
		}
		catch( std::exception& e )
		{
			fprintf( stderr, "Program terminated with an unhandled exception.\nMessage: %s\n", e.what() );
			throw;
		}
	}
	catch( std::exception& )
	{
		exitStatus = EXIT_FAILURE;
	}

	co::shutdown();

	return exitStatus;
}
