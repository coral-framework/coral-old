/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#include "PlatformUtils.h"
#include <cstring>

int main( int argc, char* argv[] )
{
	// resolve the CORAL_ROOT
	std::string rootDir;
	getCurrentExecutableDir( rootDir );

	// call $CORAL_ROOT/coral lua.Launcher co.compiler.cli $@
	std::string program( rootDir );
	program += DIR_SEP_STR "coral";
	
	std::string arg1( "lua.Launcher" );
	std::string arg2( "co.compiler.cli" );

	int numArgs = argc + 2;
	char** args = new char*[numArgs + 1];

	int k = 0;
	args[k++] = const_cast<char*>( program.c_str() );
	args[k++] = const_cast<char*>( arg1.c_str() );
	args[k++] = const_cast<char*>( arg2.c_str() );
	for( int i = 1; i <= argc; ++i )
		args[k++] = argv[i];
	args[k] = NULL;

	int res = executeProgram( numArgs, args );

	delete[] args;
	
	return res;
}
