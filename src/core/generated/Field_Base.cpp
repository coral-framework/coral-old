/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#include "Field_Base.h"
#include <co/Coral.h>
#include <co/IComponent.h>
#include <co/IPort.h>
#include <co/IInterface.h>

namespace co {

// ------ co.Field provides an interface named 'field', of type co.IField ------ //

co::IInterface* Field_co_IField::getInterface()
{
	return co::typeOf<co::IField>::get();
}

co::IPort* Field_co_IField::getFacet()
{
	co::IComponent* component = static_cast<co::IComponent*>( co::getType( "co.Field" ) );
	assert( component );
	co::IPort* facet = static_cast<co::IPort*>( component->getMember( "field" ) );
	assert( facet );
	return facet;
}

// ------ Field_Base ------ //

Field_Base::Field_Base()
{
	// empty
}

Field_Base::~Field_Base()
{
	// empty
}

co::IObject* Field_Base::getProvider()
{
	return this;
}

void Field_Base::serviceRetain()
{
	incrementRefCount();
}

void Field_Base::serviceRelease()
{
	decrementRefCount();
}

co::IComponent* Field_Base::getComponent()
{
	co::IType* type = co::getType( "co.Field" );
	assert( dynamic_cast<co::IComponent*>( type ) );
	return static_cast<co::IComponent*>( type );
}

co::IService* Field_Base::getService( co::IPort* port )
{
	checkValidPort( port );
	co::IService* res = NULL;
	switch( port->getIndex() )
	{
	case 0:		res = co::disambiguate<co::IService, co::IField>( this ); break;
	default:	raiseUnexpectedPortIndex();
	}
	return res;
}

void Field_Base::setService( co::IPort* receptacle, co::IService* service )
{
	checkValidReceptacle( receptacle );
	raiseUnexpectedPortIndex();
	CORAL_UNUSED( service );
}

} // namespace co
