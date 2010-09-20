/*******************************************************************************
** Base class generated for component 'lua.Universe'
**
** Created: Mon Sep 20 12:13:52 2010
**      by: Coral Compiler version 0.3.0
**
** WARNING! All changes made in this file will be lost when recompiling!
********************************************************************************/

#include "Universe_Base.h"
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

// ------ lua.Universe provides an interface named 'state', of type lua.IState ------ //

co::InterfaceType* Universe_lua_IState::getInterfaceType()
{
	return co::typeOf<lua::IState>::get();
}

const std::string& Universe_lua_IState::getInterfaceName()
{
	static const std::string s_interfaceName( "state" );
	return s_interfaceName;
}

// ------ Universe_Base ------ //

Universe_Base::Universe_Base()
{
	moduleRetain();
}

Universe_Base::~Universe_Base()
{
	moduleRelease();
}

co::Component* Universe_Base::getInterfaceOwner()
{
	return this;
}

void Universe_Base::componentRetain()
{
	incrementRefCount();
}

void Universe_Base::componentRelease()
{
	decrementRefCount();
}

co::ComponentType* Universe_Base::getComponentType()
{
	co::Type* type = co::getType( "lua.Universe" );
	assert( dynamic_cast<co::ComponentType*>( type ) );
	return static_cast<co::ComponentType*>( type );
}

co::Interface* Universe_Base::getInterface( co::InterfaceInfo* interfaceInfo )
{
	checkValidInterface( interfaceInfo );
	co::Interface* res = NULL;
	switch( interfaceInfo->getIndex() )
	{
	case 0:		res = co::disambiguate<co::Interface, lua::IState>( this ); break;
	default:	raiseUnexpectedInterfaceIndex();
	}
	return res;
}

void Universe_Base::bindInterface( co::InterfaceInfo* clientInterface, co::Interface* instance )
{
	checkValidClientInterface( clientInterface );
	raiseUnexpectedInterfaceIndex();
	CORAL_UNUSED( instance );
}

} // namespace lua
