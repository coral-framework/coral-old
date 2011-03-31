/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#include "ArrayType_Base.h"
#include <co/Coral.h>
#include <co/IComponent.h>
#include <co/IPort.h>
#include <co/IInterface.h>

namespace co {

// ------ co.ArrayType provides an interface named 'type', of type co.IArray ------ //

co::IInterface* ArrayType_co_IArray::getInterface()
{
	return co::typeOf<co::IArray>::get();
}

co::IPort* ArrayType_co_IArray::getFacet()
{
	co::IComponent* component = static_cast<co::IComponent*>( co::getType( "co.ArrayType" ) );
	assert( component );
	co::IPort* facet = static_cast<co::IPort*>( component->getMember( "type" ) );
	assert( facet );
	return facet;
}

// ------ ArrayType_Base ------ //

ArrayType_Base::ArrayType_Base()
{
	// empty
}

ArrayType_Base::~ArrayType_Base()
{
	// empty
}

co::IObject* ArrayType_Base::getProvider()
{
	return this;
}

void ArrayType_Base::serviceRetain()
{
	incrementRefCount();
}

void ArrayType_Base::serviceRelease()
{
	decrementRefCount();
}

co::IComponent* ArrayType_Base::getComponent()
{
	co::IType* type = co::getType( "co.ArrayType" );
	assert( dynamic_cast<co::IComponent*>( type ) );
	return static_cast<co::IComponent*>( type );
}

co::IService* ArrayType_Base::getService( co::IPort* port )
{
	checkValidPort( port );
	co::IService* res = NULL;
	switch( port->getIndex() )
	{
	case 0:		res = co::disambiguate<co::IService, co::IArray>( this ); break;
	default:	raiseUnexpectedPortIndex();
	}
	return res;
}

void ArrayType_Base::setService( co::IPort* receptacle, co::IService* service )
{
	checkValidReceptacle( receptacle );
	raiseUnexpectedPortIndex();
	CORAL_UNUSED( service );
}

} // namespace co
