/*******************************************************************************
** Base class generated for component 'lua.Launcher'
**
** Created: Mon Sep 20 12:13:52 2010
**      by: Coral Compiler version 0.3.0
**
** WARNING! All changes made in this file will be lost when recompiling!
********************************************************************************/

#include "Launcher_Base.h"
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

// ------ lua.Launcher provides an interface named 'launcher', of type lua.ILauncher ------ //

co::InterfaceType* Launcher_lua_ILauncher::getInterfaceType()
{
	return co::typeOf<lua::ILauncher>::get();
}

const std::string& Launcher_lua_ILauncher::getInterfaceName()
{
	static const std::string s_interfaceName( "launcher" );
	return s_interfaceName;
}

// ------ Launcher_Base ------ //

Launcher_Base::Launcher_Base()
{
	moduleRetain();
}

Launcher_Base::~Launcher_Base()
{
	moduleRelease();
}

co::Component* Launcher_Base::getInterfaceOwner()
{
	return this;
}

void Launcher_Base::componentRetain()
{
	incrementRefCount();
}

void Launcher_Base::componentRelease()
{
	decrementRefCount();
}

co::ComponentType* Launcher_Base::getComponentType()
{
	co::Type* type = co::getType( "lua.Launcher" );
	assert( dynamic_cast<co::ComponentType*>( type ) );
	return static_cast<co::ComponentType*>( type );
}

co::Interface* Launcher_Base::getInterface( co::InterfaceInfo* interfaceInfo )
{
	checkValidInterface( interfaceInfo );
	co::Interface* res = NULL;
	switch( interfaceInfo->getIndex() )
	{
	case 0:		res = co::disambiguate<co::Interface, lua::ILauncher>( this ); break;
	default:	raiseUnexpectedInterfaceIndex();
	}
	return res;
}

void Launcher_Base::bindInterface( co::InterfaceInfo* clientInterface, co::Interface* instance )
{
	checkValidClientInterface( clientInterface );
	raiseUnexpectedInterfaceIndex();
	CORAL_UNUSED( instance );
}

} // namespace lua
