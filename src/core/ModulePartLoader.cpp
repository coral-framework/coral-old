/*
 * Coral - Lightweight C++ Component Framework
 * See Copyright Notice in Coral.h
 */

#include "ModulePartLoader.h"
#include "utils/Library.h"
#include "utils/StringTokenizer.h"
#include <co/Coral.h>
#include <co/IType.h>
#include <co/IModule.h>
#include <co/ISystem.h>
#include <co/INamespace.h>
#include <co/IModulePart.h>
#include <co/IModuleManager.h>
#include <co/TypeLoadException.h>
#include <co/ModuleLoadException.h>
#include <co/IllegalArgumentException.h>
#include <co/reserved/OS.h>
#include <co/reserved/Uuid.h>
#include <co/reserved/LibraryManager.h>
#include <cstdio>
#include <sstream>

namespace co {

// ------ Module Bootstrap Functions -------------------------------------------

struct TypeDependency
{
	const char* name;
	const char* binarySignature;
};

typedef const char* (*QueryVerificationDataFunction)();
typedef const TypeDependency* (*QueryDependenciesFunction)();
typedef IModulePart* (*ModulePartInstanceFunction)();

struct BootstrapFunctions
{
	QueryVerificationDataFunction queryVerificationData;
	QueryDependenciesFunction queryDependencies;
	ModulePartInstanceFunction modulePartInstance;
};


// ------ Default IModulePartLoader ---------------------------------------------

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

IModulePart* ModulePartLoader::loadModulePart( const std::string& moduleName )
{
	std::string libraryFilename;
	if( !locateModuleLibrary( moduleName, &libraryFilename ) )
		throw ModuleLoadException( "unexpected missing module library file" );

	RefPtr<Library> lib( new Library( libraryFilename ) );
	lib->setLoadHints( Library::GlobalSymbolsHint );
	lib->load();

	// resolve module bootstrap functions
	BootstrapFunctions bf;
	resolveModuleFunctions( lib.get(), bf );

	// sanity check the module
	const char* verificationData = bf.queryVerificationData();
	if( !checkVerificationData( verificationData ) )
		throw ModuleLoadException( "library contains malformed verification data" );

	checkTypeDependencies( bf.queryDependencies() );

	IModulePart* part = bf.modulePartInstance();
	if( !part )
		throw ModuleLoadException( "library provided a null IModulePart" );

	LibraryManager::add( part, lib.get() );

	return part;
}

#if defined(CORAL_OS_WIN)
	#define MODULE_LIB_EXT ".dll"
#else
	#define MODULE_LIB_EXT ".so"
#endif

bool ModulePartLoader::locateModuleLibrary( const std::string& moduleName, std::string* filename )
{
	const char* moduleBaseName = moduleName.c_str();
	size_t lastDotPos = moduleName.rfind( '.' );
	if( lastDotPos != std::string::npos )
		moduleBaseName += ( lastDotPos + 1 );

	int n = 0;
	std::string fileNames[4];

	// avoid string re-alocations
	const int PADDING = 3 + 6 + 4; // for 'lib', '_debug' and '.dll'
	size_t reserveLen = ( moduleName.length() + ( moduleBaseName - moduleName.c_str() ) + PADDING );

#ifndef CORAL_NDEBUG
	// moduleName/moduleName_debug
	fileNames[n].reserve( reserveLen );
	fileNames[n].assign( moduleBaseName );
	fileNames[n].append( "_debug" MODULE_LIB_EXT );
	++n;

	// moduleName/libmoduleName_debug
	fileNames[n].reserve( reserveLen );
	fileNames[n].assign( "lib" );
	fileNames[n].append( moduleBaseName );
	fileNames[n].append( "_debug" MODULE_LIB_EXT );
	++n;
#endif

	// moduleName/moduleName
	fileNames[n].assign( moduleBaseName );
	fileNames[n].append( MODULE_LIB_EXT );
	++n;

	// moduleName/libmoduleName
	fileNames[n].reserve( reserveLen );
	fileNames[n].assign( "lib" );
	fileNames[n].append( moduleBaseName );
	fileNames[n].append( MODULE_LIB_EXT );
	++n;

	std::string modulePath( moduleName );
	OS::convertDotsToDirSeps( modulePath );

	return OS::searchFile3( getPaths(),
							Range<std::string>( &modulePath, 1 ),
							Range<std::string>( fileNames, n ),
							filename ? *filename : modulePath );
}

void ModulePartLoader::resolveModuleFunctions( Library* lib, BootstrapFunctions& bf )
{
	bf.queryVerificationData = reinterpret_cast<QueryVerificationDataFunction>(
												lib->resolve( "coral_module_query_verification_data" ) );
	bf.queryDependencies = reinterpret_cast<QueryDependenciesFunction>(
												lib->resolve( "coral_module_query_dependencies" ) );
	bf.modulePartInstance = reinterpret_cast<ModulePartInstanceFunction>(
												lib->resolve( "coral_module_part_instance" ) );

	if( !bf.queryVerificationData || !bf.queryDependencies || !bf.modulePartInstance )
		CORAL_THROW( ModuleLoadException, "incompatible library: '" << lib->getFileName()
			<< "' does not properly export the module bootstrap functions" );
}

bool ModulePartLoader::checkVerificationData( const std::string& data )
{
	StringTokenizer st( data, "=\n" );
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
		CORAL_THROW( ModuleLoadException, "malformed version string '"
						<< str << "' in module verification data" );

	if( major > CORAL_VERSION_MAJOR || minor > CORAL_VERSION_MINOR )
		CORAL_THROW( ModuleLoadException, "module was compiled for a newer Coral version ("
						<< major << '.' << minor << "; running: " CORAL_VERSION_STR ")" );

	// check the build key; currently it must be a perfect match
	if( !st.nextToken() || str != "buildkey" || !st.nextToken() )
		return false;

	if( str != CORAL_BUILD_KEY )
		CORAL_THROW( ModuleLoadException, "incompatible build keys (module: "
						<< str << "; core: " CORAL_BUILD_KEY ")" );

	// check the build mode
	if( !st.nextToken() || str != "buildmode" || !st.nextToken() )
		return false;

	// in release mode we cannot load modules built in debug mode, and vice versa
	if( str != CORAL_BUILD_MODE )
	{
		CORAL_THROW( ModuleLoadException, "incompatible build modes (module: "
						<< str << "; core: " CORAL_BUILD_MODE ")" );
	}

	return true;
}

void ModulePartLoader::checkTypeDependencies( const TypeDependency* td )
{
	std::string currentBinarySignature;
	std::vector<IType*> incompatibleTypes;

	try
	{
		while( td->name && td->binarySignature )
		{
			IType* type = getType( td->name );
			type->getBinarySignature().getString( currentBinarySignature );

			if( currentBinarySignature != td->binarySignature )
				incompatibleTypes.push_back( type );

			++td;
		}
	}
	catch( TypeLoadException& e )
	{
		CORAL_THROW( ModuleLoadException, "could not load required type '"
						<< td->name << "': " << e.getMessage() );
	}
	catch( std::exception& e )
	{
		CORAL_THROW( ModuleLoadException, "type dependency check error: " << e.what() );
	}

	if( !getSystem()->getModules()->getBinaryCompatibilityChecking() )
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

	throw ModuleLoadException( ss.str() );
}

CORAL_EXPORT_COMPONENT( ModulePartLoader, ModulePartLoader );

} // namespace co
