/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#include "Universe_Base.h"
#include <co/Coral.h>
#include <co/IComponent.h>
#include <co/IPort.h>
#include <co/IInterface.h>

namespace lua {

void moduleRetain();
void moduleRelease();

// ------ lua.Universe provides an interface named 'state', of type lua.IState ------ //

co::IInterface* Universe_lua_IState::getInterfaceType()
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

co::IObject* Universe_Base::getInterfaceOwner()
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

co::IComponent* Universe_Base::getComponentType()
{
	co::IType* type = co::getType( "lua.Universe" );
	assert( dynamic_cast<co::IComponent*>( type ) );
	return static_cast<co::IComponent*>( type );
}

co::IService* Universe_Base::getInterface( co::IPort* port )
{
	checkValidPort( port );
	co::IService* res = NULL;
	switch( port->getIndex() )
	{
	case 0:		res = co::disambiguate<co::IService, lua::IState>( this ); break;
	default:	raiseUnexpectedPortIndex();
	}
	return res;
}

void Universe_Base::setReceptacle( co::IPort* receptacle, co::IService* service )
{
	checkValidReceptacle( receptacle );
	raiseUnexpectedPortIndex();
	CORAL_UNUSED( service );
}

} // namespace lua
