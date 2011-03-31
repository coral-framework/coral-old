/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#include "Struct_Base.h"
#include <co/Coral.h>
#include <co/IComponent.h>
#include <co/IPort.h>
#include <co/IInterface.h>

namespace co {

// ------ co.Struct provides an interface named 'type', of type co.IStruct ------ //

co::IInterface* Struct_co_IStruct::getInterface()
{
	return co::typeOf<co::IStruct>::get();
}

co::IPort* Struct_co_IStruct::getFacet()
{
	co::IComponent* component = static_cast<co::IComponent*>( co::getType( "co.Struct" ) );
	assert( component );
	co::IPort* facet = static_cast<co::IPort*>( component->getMember( "type" ) );
	assert( facet );
	return facet;
}

// ------ Struct_Base ------ //

Struct_Base::Struct_Base()
{
	// empty
}

Struct_Base::~Struct_Base()
{
	// empty
}

co::IObject* Struct_Base::getProvider()
{
	return this;
}

void Struct_Base::serviceRetain()
{
	incrementRefCount();
}

void Struct_Base::serviceRelease()
{
	decrementRefCount();
}

co::IComponent* Struct_Base::getComponent()
{
	co::IType* type = co::getType( "co.Struct" );
	assert( dynamic_cast<co::IComponent*>( type ) );
	return static_cast<co::IComponent*>( type );
}

co::IService* Struct_Base::getService( co::IPort* port )
{
	checkValidPort( port );
	co::IService* res = NULL;
	switch( port->getIndex() )
	{
	case 0:		res = co::disambiguate<co::IService, co::IStruct>( this ); break;
	default:	raiseUnexpectedPortIndex();
	}
	return res;
}

void Struct_Base::setService( co::IPort* receptacle, co::IService* service )
{
	checkValidReceptacle( receptacle );
	raiseUnexpectedPortIndex();
	CORAL_UNUSED( service );
}

} // namespace co
