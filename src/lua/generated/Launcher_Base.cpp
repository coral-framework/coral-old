/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#include "Launcher_Base.h"
#include <co/Coral.h>
#include <co/ComponentType.h>
#include <co/InterfaceInfo.h>
#include <co/InterfaceType.h>

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

void Launcher_Base::setReceptacle( co::InterfaceInfo* receptacle, co::Interface* facet )
{
	checkValidReceptacle( receptacle );
	raiseUnexpectedInterfaceIndex();
	CORAL_UNUSED( facet );
}

} // namespace lua
