/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#include "lua_Base.h"
#include <co/Coral.h>
#include <co/IComponent.h>
#include <co/IPort.h>
#include <co/IInterface.h>

namespace lua {

void moduleRetain();
void moduleRelease();

// ------ lua.lua provides an interface named 'part', of type co.IModulePart ------ //

co::IInterface* lua_co_IModulePart::getInterfaceType()
{
	return co::typeOf<co::IModulePart>::get();
}

const std::string& lua_co_IModulePart::getInterfaceName()
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

co::IObject* lua_Base::getInterfaceOwner()
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

co::IComponent* lua_Base::getComponentType()
{
	co::IType* type = co::getType( "lua.lua" );
	assert( dynamic_cast<co::IComponent*>( type ) );
	return static_cast<co::IComponent*>( type );
}

co::IService* lua_Base::getInterface( co::IPort* port )
{
	checkValidPort( port );
	co::IService* res = NULL;
	switch( port->getIndex() )
	{
	case 0:		res = co::disambiguate<co::IService, co::IModulePart>( this ); break;
	default:	raiseUnexpectedPortIndex();
	}
	return res;
}

void lua_Base::setReceptacle( co::IPort* receptacle, co::IService* service )
{
	checkValidReceptacle( receptacle );
	raiseUnexpectedPortIndex();
	CORAL_UNUSED( service );
}

} // namespace lua
