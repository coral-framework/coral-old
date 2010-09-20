/*******************************************************************************
** Base class generated for component 'lua.ModulePartLoader'
**
** Created: Mon Sep 20 12:13:52 2010
**      by: Coral Compiler version 0.3.0
**
** WARNING! All changes made in this file will be lost when recompiling!
********************************************************************************/

#include "ModulePartLoader_Base.h"
#include <co/Coral.h>
#include <co/ComponentType.h>
#include <co/InterfaceInfo.h>
#include <co/InterfaceType.h>

#if !defined( CORAL_COMPILER_OUTPUT_REVISION )
#error "The header file <co/Config.h> was not included."
#elif CORAL_COMPILER_OUTPUT_REVISION != 1
#error "This file was generated using the Coral Compiler v0.3.0. It"
#error "cannot be used with the include files from this version of Coral."
#error "(The code generation rules have changed too much.)"
#endif

namespace lua {

void moduleRetain();
void moduleRelease();

// ------ lua.ModulePartLoader provides an interface named 'loader', of type co.ModulePartLoader ------ //

co::InterfaceType* ModulePartLoader_co_ModulePartLoader::getInterfaceType()
{
	return co::typeOf<co::ModulePartLoader>::get();
}

const std::string& ModulePartLoader_co_ModulePartLoader::getInterfaceName()
{
	static const std::string s_interfaceName( "loader" );
	return s_interfaceName;
}

// ------ ModulePartLoader_Base ------ //

ModulePartLoader_Base::ModulePartLoader_Base()
{
	moduleRetain();
}

ModulePartLoader_Base::~ModulePartLoader_Base()
{
	moduleRelease();
}

co::Component* ModulePartLoader_Base::getInterfaceOwner()
{
	return this;
}

void ModulePartLoader_Base::componentRetain()
{
	incrementRefCount();
}

void ModulePartLoader_Base::componentRelease()
{
	decrementRefCount();
}

co::ComponentType* ModulePartLoader_Base::getComponentType()
{
	co::Type* type = co::getType( "lua.ModulePartLoader" );
	assert( dynamic_cast<co::ComponentType*>( type ) );
	return static_cast<co::ComponentType*>( type );
}

co::Interface* ModulePartLoader_Base::getInterface( co::InterfaceInfo* interfaceInfo )
{
	checkValidInterface( interfaceInfo );
	co::Interface* res = NULL;
	switch( interfaceInfo->getIndex() )
	{
	case 0:		res = co::disambiguate<co::Interface, co::ModulePartLoader>( this ); break;
	default:	raiseUnexpectedInterfaceIndex();
	}
	return res;
}

void ModulePartLoader_Base::bindInterface( co::InterfaceInfo* clientInterface, co::Interface* instance )
{
	checkValidClientInterface( clientInterface );
	raiseUnexpectedInterfaceIndex();
	CORAL_UNUSED( instance );
}

} // namespace lua
