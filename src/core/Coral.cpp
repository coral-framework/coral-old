/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#include "System.h"
#include "ModuleInstaller.h"
#include "tools/StringTokenizer.h"
#include <co/Coral.h>
#include <co/RefPtr.h>
#include <co/IReflector.h>
#include <co/IPort.h>
#include <co/NoSuchInterfaceException.h>
#include <co/reserved/OS.h>
#include <co/reserved/LibraryManager.h>
#include <cstdio>
#include <cstdarg>
#include <cstdlib>
#include <sstream>
#include <algorithm>

namespace co {

#ifdef CORAL_OS_WIN
	static const char* PATH_SEPARATORS = ";,";
#else
	static const char* PATH_SEPARATORS = ";:,";
#endif

static std::vector<std::string> sg_paths;
static RefPtr<System> sg_system;
static ITypeManager* sg_typeManager( NULL );
static IServiceManager* sg_serviceManager( NULL );

Range<const std::string> getPaths()
{
	return sg_paths;
}

void addPath( const std::string& path )
{
	StringTokenizer st( path, PATH_SEPARATORS );
	while( st.nextToken() )
	{
		std::string dirPath( st.getToken() );
		if( dirPath.empty() )
			continue;

		// normalize & absolutize the path
		OS::makeAbs( dirPath );

		if( !OS::isDir( dirPath ) )
		{
			debug( Dbg_Warning, "cannot add '%s' to the Coral path (not a dir)", dirPath.c_str() );
			continue;
		}

		/*
			Check whether the dir is not in the CORAL_PATH already.
			This makes addPath() quadratic (what should never be a problem).
		 */
		if( std::find( sg_paths.begin(), sg_paths.end(), dirPath ) == sg_paths.end() )
			sg_paths.push_back( dirPath );
	}
}

ISystem* getSystem()
{
	if( !sg_system )
	{
		sg_system = new System;
		sg_system->initialize();
		ModuleInstaller::instance().install();
	}
	return sg_system.get();
}

void shutdown()
{
	if( !sg_system.isValid() )
		return;

	SystemState systemState = sg_system->getState();

	// ISystem::setupBase() ran successfully but ISystem::setupPresentation() was not called?
	assert( systemState != SystemState_Integrated );

	// tear down the system if it's still running
	if( systemState == SystemState_Running )
		sg_system->tearDown();

	// uninstall the 'core' module
	ModuleInstaller::instance().uninstall();

	// release the main system interfaces
	sg_serviceManager = NULL;
	sg_typeManager = NULL;
	sg_system = NULL;

	// flush all released libraries
	LibraryManager::flush();
}

static void defaultDebugEventHandler( DebugEvent ev, const char* message )
{
	static const char* s_eventName[] = { "DEBUG", "WARNING", "CRITICAL", "FATAL" };

#ifdef CORAL_NDEBUG
	if( ev == Dbg_Message )
		return;
#endif

	const char* eventName = s_eventName[ev >= 0 && ev <= Dbg_Fatal ? ev : Dbg_Fatal];
	fprintf( stderr, "[%s] %s\n", eventName, message );
	if( ev == Dbg_Fatal )
		abort();
}

static DebugEventHandler sg_debugEventHandler = defaultDebugEventHandler;

DebugEventHandler installDebugEventHandler( DebugEventHandler handler )
{
	DebugEventHandler previous = sg_debugEventHandler;
	sg_debugEventHandler = handler ? handler : defaultDebugEventHandler;
	return previous;
}

void debug( DebugEvent event, const char* msg, ... )
{
	char buffer[1024] = { 0 };
	va_list va;
	va_start( va, msg );
	if( msg )
		vsprintf( buffer, msg, va );
	va_end( va );
	sg_debugEventHandler( event, buffer );
}

IType* getType( const std::string& fullName )
{
	if( !sg_typeManager )
		sg_typeManager = getSystem()->getTypes();
	return sg_typeManager->getType( fullName );
}

IObject* newInstance( const std::string& fullName )
{
	IType* type = getType( fullName );
	assert( type );
	IReflector* reflector = type->getReflector();
	assert( reflector );
	return reflector->newInstance();
}

void setReceptacleByName( IObject* instance, const std::string& receptacleName, IService* facet )
{
	assert( instance );
	IComponent* ct = instance->getComponentType();
	IPort* ii = dynamic_cast<IPort*>( ct->getMember( receptacleName ) );
	if( !ii )
		CORAL_THROW( NoSuchInterfaceException, "no such interface '" << receptacleName
						<< "' in component " << ct->getFullName() );
	instance->setReceptacle( ii, facet );
}

inline IServiceManager* getServices()
{
	if( !sg_serviceManager )
		sg_serviceManager = getSystem()->getServices();
	return sg_serviceManager;
}

IService* getServiceForType( IInterface* serviceType, IInterface* clientType )
{
	if( clientType )
		return getServices()->getServiceForType( serviceType, clientType );
	else
		return getServices()->getService( serviceType );
}

IService* getServiceForInstance( IInterface* serviceType, IService* clientInstance )
{
	return getServices()->getServiceForInstance( serviceType, clientInstance );
}

bool findModuleFile( const std::string& moduleName, const std::string& fileName, std::string& filePath )
{
	std::string modulePath( moduleName );
	OS::convertDotsToDirSeps( modulePath );
	return OS::searchFile3( getPaths(),
				Range<const std::string>( &modulePath, 1 ),
				Range<const std::string>( &fileName, 1 ),
				filePath );
}

} // namespace co
