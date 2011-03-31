/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#include "TypeManager_Base.h"
#include <co/Coral.h>
#include <co/IComponent.h>
#include <co/IPort.h>
#include <co/IInterface.h>

namespace co {

// ------ co.TypeManager provides an interface named 'typeManager', of type co.ITypeManager ------ //

co::IInterface* TypeManager_co_ITypeManager::getInterface()
{
	return co::typeOf<co::ITypeManager>::get();
}

co::IPort* TypeManager_co_ITypeManager::getFacet()
{
	co::IComponent* component = static_cast<co::IComponent*>( co::getType( "co.TypeManager" ) );
	assert( component );
	co::IPort* facet = static_cast<co::IPort*>( component->getMember( "typeManager" ) );
	assert( facet );
	return facet;
}

// ------ TypeManager_Base ------ //

TypeManager_Base::TypeManager_Base()
{
	// empty
}

TypeManager_Base::~TypeManager_Base()
{
	// empty
}

co::IObject* TypeManager_Base::getProvider()
{
	return this;
}

void TypeManager_Base::serviceRetain()
{
	incrementRefCount();
}

void TypeManager_Base::serviceRelease()
{
	decrementRefCount();
}

co::IComponent* TypeManager_Base::getComponent()
{
	co::IType* type = co::getType( "co.TypeManager" );
	assert( dynamic_cast<co::IComponent*>( type ) );
	return static_cast<co::IComponent*>( type );
}

co::IService* TypeManager_Base::getService( co::IPort* port )
{
	checkValidPort( port );
	co::IService* res = NULL;
	switch( port->getIndex() )
	{
	case 0:		res = co::disambiguate<co::IService, co::ITypeManager>( this ); break;
	default:	raiseUnexpectedPortIndex();
	}
	return res;
}

void TypeManager_Base::setService( co::IPort* receptacle, co::IService* service )
{
	checkValidReceptacle( receptacle );
	raiseUnexpectedPortIndex();
	CORAL_UNUSED( service );
}

} // namespace co
