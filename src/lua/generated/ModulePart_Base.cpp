/*******************************************************************************
** Base class generated for component 'lua.ModulePart'
**
** Created: Mon Sep 20 12:13:52 2010
**      by: Coral Compiler version 0.3.0
**
** WARNING! All changes made in this file will be lost when recompiling!
********************************************************************************/

#include "ModulePart_Base.h"
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

// ------ lua.ModulePart provides an interface named 'part', of type co.ModulePart ------ //

co::InterfaceType* ModulePart_co_ModulePart::getInterfaceType()
{
	return co::typeOf<co::ModulePart>::get();
}

const std::string& ModulePart_co_ModulePart::getInterfaceName()
{
	static const std::string s_interfaceName( "part" );
	return s_interfaceName;
}

// ------ ModulePart_Base ------ //

ModulePart_Base::ModulePart_Base()
{
	moduleRetain();
}

ModulePart_Base::~ModulePart_Base()
{
	moduleRelease();
}

co::Component* ModulePart_Base::getInterfaceOwner()
{
	return this;
}

void ModulePart_Base::componentRetain()
{
	incrementRefCount();
}

void ModulePart_Base::componentRelease()
{
	decrementRefCount();
}

co::ComponentType* ModulePart_Base::getComponentType()
{
	co::Type* type = co::getType( "lua.ModulePart" );
	assert( dynamic_cast<co::ComponentType*>( type ) );
	return static_cast<co::ComponentType*>( type );
}

co::Interface* ModulePart_Base::getInterface( co::InterfaceInfo* interfaceInfo )
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

void ModulePart_Base::bindInterface( co::InterfaceInfo* clientInterface, co::Interface* instance )
{
	checkValidClientInterface( clientInterface );
	raiseUnexpectedInterfaceIndex();
	CORAL_UNUSED( instance );
}

} // namespace lua
