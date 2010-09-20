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

DEFINE_string( G, "", "generates code for the indicated module" );
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
	FLAGS_log_prefix = false;
	FLAGS_logtostderr = true;

	// set usage message
	google::SetUsageMessage(
		"Coral Compiler Usage:\n"
		"    coralc [OPTIONS] [-G moduleName] [typeName, ...]\n"
		"\n"
		"Generates mappings for the list of types passed as command-line arguments.\n"
		"If the -G option is specified (alongside a module name), the compiler will\n"
		"generate code for a module. In this case, the passed list of types will be\n"
		"considered extra module dependencies.\n"
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

	LOG( INFO ) << "Coral Compiler v" << CORAL_VERSION_STR << " (" << CORAL_BUILD_KEY << ")";

	co::addPath( fLS::FLAGS_path );

	const std::string& moduleName = fLS::FLAGS_G;
	if( !moduleName.empty() )
		LOG( INFO )	<< "Processing module '" << moduleName << "'...";

	const std::string& mappingsDir = ( fLS::FLAGS_mappingsdir.empty() ? fLS::FLAGS_outdir : fLS::FLAGS_mappingsdir );
	ModuleCompiler moduleCompiler( moduleName, fLS::FLAGS_outdir, mappingsDir );

	for( int i = 1; i < argc; ++i )
		moduleCompiler.addExtraDependency( argv[i] );

	return moduleCompiler.run();
}
