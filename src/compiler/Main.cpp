/*
 * Coral - A C++ Component Framework.
 * See Copyright Notice in Coral.h
 */

#include "ModuleCompiler.h"
#include <co/Coral.h>
#include <co/System.h>
#include <core/TypeLoader.h>
#include <gflags/gflags.h>
#include <glog/logging.h>

DEFINE_string( path, "./", "comma-separated list of type repositories" );
DEFINE_string( outdir, "./generated", "output dir for generated files" );
DEFINE_string( mappingsdir, "", "separate output dir for mappings" );

// access to other GFlag flags:
DECLARE_bool( help );
DECLARE_string( helpmatch );
DECLARE_bool( log_prefix );
DECLARE_bool( logtostderr );

int main( int argc, char* argv[] )
{
	// modify default GFlags settings
	FLAGS_help = false;
	//FLAGS_helpmatch = "coral";
	FLAGS_log_prefix = false;
	FLAGS_logtostderr = true;

	// set usage message
	google::SetUsageMessage(
		"Coral Compiler Usage:\n"
		"    coralc [OPTIONS] moduleName [typeName, ...]\n"
		"\n"
		"The list of typeNames following the moduleName indicates extra\n"
		"module dependencies, for which the compiler will generate mappings.\n"
		"\n"
		"Options Summary:\n"
		"    -path extra,dirs (extra type repositories)\n"
		"    -outdir dir (where files should be generated)\n"
		"    -mappingsdir dir (a separate dir for mappings)\n"
		"\n"
		"Unless 'mappingsdir' is specified, mappings are generated in the 'outdir'.\n"
	);

	google::ParseCommandLineFlags( &argc, &argv, true );

	google::InitGoogleLogging( argv[0] );

	if( argc < 2 )
	{
		LOG( ERROR ) << "Missing required argument 'moduleName'. For help, try \"-helpshort\".";
		return -1;
	}

	LOG( INFO ) << "Coral Compiler v" << CORAL_VERSION_STR << " (" << CORAL_BUILD_KEY << ")";

	co::TypeLoader::enableDocMap();
	co::TypeLoader::enableCppBlockMap();

	co::addPath( fLS::FLAGS_path );

	std::string moduleName = argv[1];
	LOG( INFO )	<< "Processing module '" << moduleName << "'...";

	const std::string& mappingsDir = ( fLS::FLAGS_mappingsdir.empty() ? fLS::FLAGS_outdir : fLS::FLAGS_mappingsdir );
	ModuleCompiler moduleCompiler( moduleName, fLS::FLAGS_outdir, mappingsDir );

	for( int i = 2; i < argc; ++i )
		moduleCompiler.addExtraDependency( argv[i] );

	return moduleCompiler.run();
}
