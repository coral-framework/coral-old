/*
 * Coral - Lightweight C++ Component Framework
 * See Copyright Notice in Coral.h
 */

#include "Utils.h"

int main( int argc, char* argv[] )
{
	// resolve the CORAL_ROOT
	std::string rootDir;
	co::OS::getApplicationDir( rootDir );

	// call $CORAL_ROOT/coral lua.Launcher co.compiler.cli $@
	std::string program( rootDir );
	program += CORAL_OS_DIR_SEP_STR "coral";

	int numArgs = argc + 2;
	char** args = new char*[numArgs + 1];

	int k = 0;
	args[k++] = const_cast<char*>( program.c_str() );
	args[k++] = const_cast<char*>( "lua.Launcher" );
	args[k++] = const_cast<char*>( "co.compiler.cli" );
	for( int i = 1; i <= argc; ++i )
		args[k++] = argv[i];
	args[k] = NULL;

	int res = executeProgram( numArgs, args );

	delete[] args;
	
	return res;
}
