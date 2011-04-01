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

// ------ lua.Universe has a facet named 'state', of type lua.IState ------ //

co::IInterface* Universe_lua_IState::getInterface()
{
	return co::typeOf<lua::IState>::get();
}

co::IPort* Universe_lua_IState::getFacet()
{
	co::IComponent* component = static_cast<co::IComponent*>( co::getType( "lua.Universe" ) );
	assert( component );
	co::IPort* facet = static_cast<co::IPort*>( component->getMember( "state" ) );
	assert( facet );
	return facet;
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

co::IObject* Universe_Base::getProvider()
{
	return this;
}

void Universe_Base::serviceRetain()
{
	incrementRefCount();
}

void Universe_Base::serviceRelease()
{
	decrementRefCount();
}

co::IComponent* Universe_Base::getComponent()
{
	co::IType* type = co::getType( "lua.Universe" );
	assert( dynamic_cast<co::IComponent*>( type ) );
	return static_cast<co::IComponent*>( type );
}

co::IService* Universe_Base::getService( co::IPort* port )
{
	checkValidPort( port );
	co::IService* res = NULL;
	switch( port->getIndex() )
	{
	case 0:		res = static_cast<lua::IState*>( this ); break;
	default:	raiseUnexpectedPortIndex();
	}
	return res;
}

void Universe_Base::setService( co::IPort* receptacle, co::IService* service )
{
	checkValidReceptacle( receptacle );
	raiseUnexpectedPortIndex();
	CORAL_UNUSED( service );
}

} // namespace lua
