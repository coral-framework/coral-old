/*******************************************************************************
** Base class generated for component 'lua.lua'
**
** Created: Mon Sep 20 12:13:52 2010
**      by: Coral Compiler version 0.3.0
**
** WARNING! All changes made in this file will be lost when recompiling!
********************************************************************************/

#include "lua_Base.h"
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

// ------ lua.lua provides an interface named 'part', of type co.ModulePart ------ //

co::InterfaceType* lua_co_ModulePart::getInterfaceType()
{
	return co::typeOf<co::ModulePart>::get();
}

const std::string& lua_co_ModulePart::getInterfaceName()
{
	static const std::string s_interfaceName( "part" );
	return s_interfaceName;
}

// ------ lua_Base ------ //

lua_Base::lua_Base()
{
	moduleRetain();
}

lua_Base::~lua_Base()
{
	moduleRelease();
}

co::Component* lua_Base::getInterfaceOwner()
{
	return this;
}

void lua_Base::componentRetain()
{
	incrementRefCount();
}

void lua_Base::componentRelease()
{
	decrementRefCount();
}

co::ComponentType* lua_Base::getComponentType()
{
	co::Type* type = co::getType( "lua.lua" );
	assert( dynamic_cast<co::ComponentType*>( type ) );
	return static_cast<co::ComponentType*>( type );
}

co::Interface* lua_Base::getInterface( co::InterfaceInfo* interfaceInfo )
{
	checkValidInterface( interfaceInfo );
	co::Interface* res = NULL;
	switch( interfaceInfo->getIndex() )
	{
	case 0:		res = co::disambiguate<co::Interface, co::ModulePart>( this ); break;
	default:	raiseUnexpectedInterfaceIndex();
	}
	return res;
}

void lua_Base::bindInterface( co::InterfaceInfo* clientInterface, co::Interface* instance )
{
	checkValidClientInterface( clientInterface );
	raiseUnexpectedInterfaceIndex();
	CORAL_UNUSED( instance );
}

} // namespace lua
