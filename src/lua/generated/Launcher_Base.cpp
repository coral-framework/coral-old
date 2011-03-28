/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#include "Launcher_Base.h"
#include <co/Coral.h>
#include <co/IComponent.h>
#include <co/IPort.h>
#include <co/IInterface.h>

namespace lua {

void moduleRetain();
void moduleRelease();

// ------ lua.Launcher provides an interface named 'launcher', of type lua.ILauncher ------ //

co::IInterface* Launcher_lua_ILauncher::getInterfaceType()
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

co::IObject* Launcher_Base::getInterfaceOwner()
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

co::IComponent* Launcher_Base::getComponentType()
{
	co::IType* type = co::getType( "lua.Launcher" );
	assert( dynamic_cast<co::IComponent*>( type ) );
	return static_cast<co::IComponent*>( type );
}

co::IService* Launcher_Base::getInterface( co::IPort* port )
{
	checkValidPort( port );
	co::IService* res = NULL;
	switch( port->getIndex() )
	{
	case 0:		res = co::disambiguate<co::IService, lua::ILauncher>( this ); break;
	default:	raiseUnexpectedPortIndex();
	}
	return res;
}

void Launcher_Base::setReceptacle( co::IPort* receptacle, co::IService* service )
{
	checkValidReceptacle( receptacle );
	raiseUnexpectedPortIndex();
	CORAL_UNUSED( service );
}

} // namespace lua
