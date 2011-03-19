/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#include "ModuleInstaller.h"
#include <co/IType.h>

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
		co::IReflector* reflector = _reflectors[i].get();
		reflector->getType()->setReflector( reflector );
	}
}

void lua::ModuleInstaller::uninstall()
{
	for( unsigned int i = 0; i < TypeCount; ++i )
	{
		if( _reflectors[i].isValid() )
		{
			_reflectors[i]->getType()->setReflector( NULL );
			_reflectors[i] = NULL;
		}
	}
}

co::IReflector* lua::ModuleInstaller::createReflector( TypeId typeId )
{
	co::IReflector* res = NULL;
	switch( typeId )
	{
	case TypeId_Component: res = lua::__createComponentIReflector(); break;
	case TypeId_Exception: res = lua::__createExceptionIReflector(); break;
	case TypeId_ILauncher: res = lua::__createILauncherIReflector(); break;
	case TypeId_IState: res = lua::__createIStateIReflector(); break;
	case TypeId_Launcher: res = lua::__createLauncherIReflector(); break;
	case TypeId_lua: res = lua::__createluaIReflector(); break;
	case TypeId_ModulePart: res = lua::__createModulePartIReflector(); break;
	case TypeId_ModulePartLoader: res = lua::__createModulePartLoaderIReflector(); break;
	case TypeId_Universe: res = lua::__createUniverseIReflector(); break;
	default: assert( false );
	};
	assert( res != NULL );
	return res;
}
