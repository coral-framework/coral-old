/*
 * Coral - A C++ Component Framework.
 * See Copyright Notice in Coral.h
 */

#include "System.h"
#include "ModuleInstaller.h"
#include "tools/StringTokenizer.h"
#include <co/Coral.h>
#include <co/RefPtr.h>
#include <co/Reflector.h>
#include <cstdlib>

#ifdef CORAL_OS_WIN
	static const char* PATH_SEPARATORS = ";,";
#else
	static const char* PATH_SEPARATORS = ";:,";
#endif

static std::vector<std::string> sg_paths;
static co::RefPtr<System> sg_system;
static co::TypeManager* sg_typeManager( NULL );
static co::ServiceManager* sg_serviceManager( NULL );

co::ArrayRange<const std::string> co::getPaths()
{
	static bool s_readEnvVar( false );
	if( !s_readEnvVar )
	{
		// automatically add the CORAL_PATH env var
		const char* envVar = getenv( "CORAL_PATH" );
		if( envVar != NULL )
			co::addPath( envVar );
		s_readEnvVar = true;
	}

	return sg_paths;
}

void co::addPath( const std::string& path )
{
	co::StringTokenizer st( path, PATH_SEPARATORS );
	while( st.nextToken() )
	{
		const std::string& dirPath = st.getToken();
		if( !dirPath.empty() )
			sg_paths.push_back( dirPath );
	}
}

co::System* co::getSystem()
{
	if( !sg_system )
	{
		sg_system = new ::System;
		sg_system->initialize();
		co::ModuleInstaller::instance().install();
	}
	return sg_system.get();
}

void co::shutdown()
{
	if( !sg_system.isValid() )
		return;

	co::SystemState systemState = sg_system->getState();

	// System::setupBase() ran successfully but System::setupPresentation() was not called?
	assert( systemState != co::SystemState_Integrated );

	// tear down the system if it's still running
	if( systemState == co::SystemState_Running )
		sg_system->tearDown();

	co::ModuleInstaller::instance().uninstall();

	sg_serviceManager = NULL;
	sg_typeManager = NULL;
	sg_system = NULL;
}

co::Type* co::getType( const std::string& fullName )
{
	if( !sg_typeManager )
		sg_typeManager = co::getSystem()->getTypes();
	return sg_typeManager->getType( fullName );
}

co::Component* co::newInstance( const std::string& fullName )
{
	co::Type* type = co::getType( fullName );
	assert( type );
	co::Reflector* reflector = type->getReflector();
	assert( reflector );
	return reflector->newInstance();
}

inline co::ServiceManager* getServices()
{
	if( !sg_serviceManager )
		sg_serviceManager = co::getSystem()->getServices();
	return sg_serviceManager;
}

co::Interface* co::getServiceForType( co::InterfaceType* serviceType, co::InterfaceType* clientType )
{
	if( clientType )
		return getServices()->getServiceForType( serviceType, clientType );
	else
		return getServices()->getService( serviceType );
}

co::Interface* co::getServiceForInstance( co::InterfaceType* serviceType, co::Interface* clientInstance )
{
	return getServices()->getServiceForInstance( serviceType, clientInstance );
}
