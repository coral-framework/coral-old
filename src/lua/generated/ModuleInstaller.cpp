/*******************************************************************************
** ModuleInstaller class generated for module 'lua'
**
** Created: Mon Sep 20 12:13:52 2010
**      by: Coral Compiler version 0.3.0
**
** WARNING! All changes made in this file will be lost when recompiling!
********************************************************************************/

#include "ModuleInstaller.h"
#include <co/Type.h>

#if !defined( CORAL_COMPILER_OUTPUT_REVISION )
#error "The header file <co/Config.h> was not included."
#elif CORAL_COMPILER_OUTPUT_REVISION != 1
#error "This file was generated using the Coral Compiler v0.3.0. It"
#error "cannot be used with the include files from this version of Coral."
#error "(The code generation rules have changed too much.)"
#endif

lua::ModuleInstaller lua::ModuleInstaller::sm_instance;

lua::ModuleInstaller::ModuleInstaller()
{
	// empty
}

lua::ModuleInstaller::~ModuleInstaller()
{
	// empty
}

void lua::ModuleInstaller::initialize()
{
	for( unsigned int i = 0; i < TypeCount; ++i )
	{
		if( !_reflectors[i].isValid() )
			_reflectors[i] = createReflector( static_cast<TypeId>( i ) );
	}
}

void lua::ModuleInstaller::install()
{
	initialize();

	for( unsigned int i = 0; i < TypeCount; ++i )
	{
		co::Reflector* reflector = _reflectors[i].get();
		reflector->getType()->setReflector( reflector );
	}
}

void lua::ModuleInstaller::uninstall()
{
	for( unsigned int i = 0; i < TypeCount; ++i )
	{
		_reflectors[i]->getType()->setReflector( NULL );
		_reflectors[i] = NULL;
	}
}

co::Reflector* lua::ModuleInstaller::createReflector( TypeId typeId )
{
	co::Reflector* res = NULL;
	switch( typeId )
	{
	case TypeId_Exception: res = lua::__createExceptionReflector(); break;
	case TypeId_ILauncher: res = lua::__createILauncherReflector(); break;
	case TypeId_IState: res = lua::__createIStateReflector(); break;
	case TypeId_Launcher: res = lua::__createLauncherReflector(); break;
	case TypeId_lua: res = lua::__createluaReflector(); break;
	case TypeId_ModulePart: res = lua::__createModulePartReflector(); break;
	case TypeId_ModulePartLoader: res = lua::__createModulePartLoaderReflector(); break;
	case TypeId_Universe: res = lua::__createUniverseReflector(); break;
	default: assert( false );
	};
	assert( res != NULL );
	return res;
}
