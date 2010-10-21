/*
 * Coral - A C++ Component Framework.
 * See Copyright Notice in Coral.h
 */

#include "ModulePartLoader.h"
#include "tools/Library.h"
#include "tools/StringTokenizer.h"
#include <co/Type.h>
#include <co/Uuid.h>
#include <co/Coral.h>
#include <co/Module.h>
#include <co/System.h>
#include <co/Namespace.h>
#include <co/ModulePart.h>
#include <co/ModuleManager.h>
#include <co/TypeLoadException.h>
#include <co/ModuleLoadException.h>
#include <co/IllegalArgumentException.h>
#include <co/reserved/FileLookUp.h>
#include <co/reserved/LibraryManager.h>
#include <cstdio>
#include <sstream>

//------ Module Bootstrap Functions -----------------------------------------//

struct TypeDependency
{
	const char* name;
	const char* binarySignature;
};

typedef const char* (*QueryVerificationDataFunction)();
typedef const TypeDependency* (*QueryDependenciesFunction)();
typedef co::ModulePart* (*ModulePartInstanceFunction)();

struct BootstrapFunctions
{
	QueryVerificationDataFunction queryVerificationData;
	QueryDependenciesFunction queryDependencies;
	ModulePartInstanceFunction modulePartInstance;
};


//------ Default ModulePartLoader -------------------------------------------//

ModulePartLoader::ModulePartLoader()
{
	// empty
}

ModulePartLoader::~ModulePartLoader()
{
	// empty
}

bool ModulePartLoader::canLoadModulePart( const std::string& moduleName )
{
	return locateModuleLibrary( moduleName );
}

co::ModulePart* ModulePartLoader::loadModulePart( const std::string& moduleName )
{
	std::string libraryFilename;
	if( !locateModuleLibrary( moduleName, &libraryFilename ) )
		throw co::ModuleLoadException( "unexpected missing module library file" );

	co::RefPtr<co::Library> lib( new co::Library( libraryFilename ) );
	lib->load();

	// resolve module bootstrap functions
	BootstrapFunctions bf;
	resolveModuleFunctions( lib.get(), bf );

	// sanity check the module
	const char* verificationData = bf.queryVerificationData();
	if( !checkVerificationData( verificationData ) )
		throw co::ModuleLoadException( "library contains malformed verification data" );

	checkTypeDependencies( bf.queryDependencies() );

	co::ModulePart* part = bf.modulePartInstance();
	if( !part )
		throw co::ModuleLoadException( "library provided a null ModulePart" );

	co::LibraryManager::add( part, lib.get() );

	return part;
}

bool ModulePartLoader::locateModuleLibrary( const std::string& moduleName, std::string* filename )
{
	static std::vector<std::string> s_validExtensions;
	if( s_validExtensions.empty() )
	{
#ifdef CORAL_OS_WIN
		s_validExtensions.push_back( "dll" );
#else
		s_validExtensions.push_back( "so" );
#endif

#ifdef CORAL_OS_MAC
		s_validExtensions.push_back( "dylib" );
#endif
	}

	const char* moduleBaseName = moduleName.c_str();
	size_t lastDotPos = moduleName.rfind( '.' );
	if( lastDotPos != std::string::npos )
		moduleBaseName += ( lastDotPos + 1 );

	co::FileLookUp fileLookUp( co::getPaths(), s_validExtensions );

	std::string filePath;

	// avoid string re-alocations
	const int PADDING = 1 + 3 + 6; // for '.', 'lib' and '_debug'
	filePath.reserve( moduleName.length() + ( moduleBaseName - moduleName.c_str() ) + PADDING );

#ifndef CORAL_NDEBUG
	// in debug mode, first try moduleName/moduleName_debug
	filePath = moduleName;
	filePath.push_back( '.' );
	filePath.append( moduleBaseName );
	filePath.append( "_debug" );
	fileLookUp.addFilePath( filePath, true );

	#ifdef CORAL_OS_UNIX
		// on UNIX, also try moduleName/libmoduleName_debug
		filePath = moduleName;
		filePath.append( ".lib" );
		filePath.append( moduleBaseName );
		filePath.append( "_debug" );
		fileLookUp.addFilePath( filePath, true );
	#endif
#endif

	// try moduleName/moduleName
	filePath = moduleName;
	filePath.push_back( '.' );
	filePath.append( moduleBaseName );
	fileLookUp.addFilePath( filePath, true );

#ifdef CORAL_OS_UNIX
	// on UNIX, also try moduleName/libmoduleName
	filePath = moduleName;
	filePath.append( ".lib" );
	filePath.append( moduleBaseName );
	fileLookUp.addFilePath( filePath, true );
#endif

	return fileLookUp.locate( filename ? *filename : filePath, NULL, NULL );
}

void ModulePartLoader::resolveModuleFunctions( co::Library* lib, BootstrapFunctions& bf )
{
	bf.queryVerificationData = reinterpret_cast<QueryVerificationDataFunction>(
												lib->resolve( "coral_module_query_verification_data" ) );
	bf.queryDependencies = reinterpret_cast<QueryDependenciesFunction>(
												lib->resolve( "coral_module_query_dependencies" ) );
	bf.modulePartInstance = reinterpret_cast<ModulePartInstanceFunction>(
												lib->resolve( "coral_module_part_instance" ) );

	if( !bf.queryVerificationData || !bf.queryDependencies || !bf.modulePartInstance )
		CORAL_THROW( co::ModuleLoadException, "incompatible library: '" << lib->getFileName()
						<< "' does not properly export the module bootstrap functions" );
}

bool ModulePartLoader::checkVerificationData( const std::string& data )
{
	co::StringTokenizer st( data, "=\n" );
	const std::string& str = st.getToken();

	if( !st.nextToken() || str != "pattern" )
		return false;

	if( !st.nextToken() || str != "CORAL_MODULE_VERIFICATION_DATA" )
		return false;

	if( !st.nextToken() || str != "version" || !st.nextToken() )
		return false;

	/*
		Coral version compatibility check: the module's Coral version
		must be the same or older. The 'patch' number is ignored, since
		patches are required to maintain full backwards compatibility.
	 */
	int major, minor;
	if( sscanf( str.c_str(), " %d . %d . %*d", &major, &minor ) != 2 )
		CORAL_THROW( co::ModuleLoadException, "malformed version string '"
						<< str << "' in module verification data" );

	if( major > CORAL_VERSION_MAJOR || minor > CORAL_VERSION_MINOR )
		CORAL_THROW( co::ModuleLoadException, "module was compiled for a newer Coral version ("
						<< major << '.' << minor << "; running: " CORAL_VERSION_STR ")" );

	// check the build key; currently it must be a perfect match
	if( !st.nextToken() || str != "buildkey" || !st.nextToken() )
		return false;

	if( str != CORAL_BUILD_KEY )
		CORAL_THROW( co::ModuleLoadException, "incompatible build keys (module: "
						<< str << "; core: " CORAL_BUILD_KEY ")" );

	// check the build mode
	if( !st.nextToken() || str != "buildmode" || !st.nextToken() )
		return false;

#ifdef CORAL_NDEBUG

	// being in release mode, we cannot load modules built in debug mode
	if( str == "debug" )
		CORAL_THROW( co::ModuleLoadException, "incompatible build modes (module: "
						<< str << "; core: " CORAL_BUILD_MODE ")" );

#endif

	return true;
}

void ModulePartLoader::checkTypeDependencies( const TypeDependency* td )
{
	std::string currentBinarySignature;
	std::vector<co::Type*> incompatibleTypes;

	try
	{
		while( td->name && td->binarySignature )
		{
			co::Type* type = co::getType( td->name );
			type->getBinarySignature().getString( currentBinarySignature );

			if( currentBinarySignature != td->binarySignature )
				incompatibleTypes.push_back( type );

			++td;
		}
	}
	catch( co::TypeLoadException& e )
	{
		CORAL_THROW( co::ModuleLoadException, "could not load required type '"
						<< td->name << "': " << e.getMessage() );
	}
	catch( std::exception& e )
	{
		CORAL_THROW( co::ModuleLoadException, "type dependency check error: " << e.what() );
	}

	if( !co::getSystem()->getModules()->getBinaryCompatibilityChecking() )
		return; // binary interface check errors are disabled

	size_t numIncompatibleTypes = incompatibleTypes.size();
	if( numIncompatibleTypes == 0 )
		return;

	std::stringstream ss;
	
	if( numIncompatibleTypes == 1 )
		ss << "the module was compiled with a different, binary-incompatible version of '";
	else
		ss << "the module was compiled with different, binary-incompatible versions of the following types: ";

	for( size_t i = 0; i < numIncompatibleTypes; ++i )
	{
		if( i > 0 )
			ss << ", ";
		ss << incompatibleTypes[i]->getFullName();
	}

	if( numIncompatibleTypes == 1 )
		ss << '\'';

	throw co::ModuleLoadException( ss.str() );
}

CORAL_EXPORT_COMPONENT( ModulePartLoader, ModulePartLoaderComponent );
