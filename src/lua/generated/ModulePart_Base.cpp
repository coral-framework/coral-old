/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#include "ModulePart_Base.h"
#include <co/Coral.h>
#include <co/IComponent.h>
#include <co/IPort.h>
#include <co/IInterface.h>

namespace lua {

void moduleRetain();
void moduleRelease();

// ------ lua.ModulePart provides an interface named 'part', of type co.IModulePart ------ //

co::IInterface* ModulePart_co_IModulePart::getInterface()
{
	return co::typeOf<co::IModulePart>::get();
}

co::IPort* ModulePart_co_IModulePart::getFacet()
{
	co::IComponent* component = static_cast<co::IComponent*>( co::getType( "lua.ModulePart" ) );
	assert( component );
	co::IPort* facet = static_cast<co::IPort*>( component->getMember( "part" ) );
	assert( facet );
	return facet;
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

co::IObject* ModulePart_Base::getProvider()
{
	return this;
}

void ModulePart_Base::serviceRetain()
{
	incrementRefCount();
}

void ModulePart_Base::serviceRelease()
{
	decrementRefCount();
}

co::IComponent* ModulePart_Base::getComponent()
{
	co::IType* type = co::getType( "lua.ModulePart" );
	assert( dynamic_cast<co::IComponent*>( type ) );
	return static_cast<co::IComponent*>( type );
}

co::IService* ModulePart_Base::getService( co::IPort* port )
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

void ModulePart_Base::setService( co::IPort* receptacle, co::IService* service )
{
	checkValidReceptacle( receptacle );
	raiseUnexpectedPortIndex();
	CORAL_UNUSED( service );
}

} // namespace lua
