/*
 * Coral - Lightweight C++ Component Framework
 * See Copyright Notice in Coral.h
 */

#include "Coral.h"
#include "System.h"
#include "ModuleInstaller.h"
#include "utils/StringTokenizer.h"
#include <co/Log.h>
#include <co/RefPtr.h>
#include <co/IReflector.h>
#include <co/reserved/OS.h>
#include <co/reserved/LibraryManager.h>

namespace co {

#ifdef CORAL_OS_WIN
	static const char* PATH_SEPARATORS = ";,";
#else
	static const char* PATH_SEPARATORS = ";:,";
#endif

static std::vector<std::string> sg_paths;
static uint8 sg_cslFlags( CSL_ANNOTATIONS );
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
			CORAL_LOG(WARNING) << "cannot add '" << dirPath <<  "' to the Coral path (not a dir)";
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

uint8 getCSLFlags()
{
	return sg_cslFlags;
}

void setCSLFlags( uint8 flags )
{
	sg_cslFlags = flags;
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
