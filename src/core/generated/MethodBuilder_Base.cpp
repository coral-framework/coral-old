/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#include "MethodBuilder_Base.h"
#include <co/Coral.h>
#include <co/IComponent.h>
#include <co/IPort.h>
#include <co/IInterface.h>

namespace co {

// ------ co.MethodBuilder has a facet named 'methodBuilder', of type co.IMethodBuilder ------ //

co::IInterface* MethodBuilder_co_IMethodBuilder::getInterface()
{
	return co::typeOf<co::IMethodBuilder>::get();
}

co::IPort* MethodBuilder_co_IMethodBuilder::getFacet()
{
	co::IComponent* component = static_cast<co::IComponent*>( co::getType( "co.MethodBuilder" ) );
	assert( component );
	co::IPort* facet = static_cast<co::IPort*>( component->getMember( "methodBuilder" ) );
	assert( facet );
	return facet;
}

// ------ MethodBuilder_Base ------ //

MethodBuilder_Base::MethodBuilder_Base()
{
	// empty
}

MethodBuilder_Base::~MethodBuilder_Base()
{
	// empty
}

co::IObject* MethodBuilder_Base::getProvider()
{
	return this;
}

void MethodBuilder_Base::serviceRetain()
{
	incrementRefCount();
}

void MethodBuilder_Base::serviceRelease()
{
	decrementRefCount();
}

co::IComponent* MethodBuilder_Base::getComponent()
{
	co::IType* type = co::getType( "co.MethodBuilder" );
	assert( dynamic_cast<co::IComponent*>( type ) );
	return static_cast<co::IComponent*>( type );
}

co::IService* MethodBuilder_Base::getService( co::IPort* port )
{
	checkValidPort( port );
	co::IService* res = NULL;
	switch( port->getIndex() )
	{
	case 0:		res = static_cast<co::IMethodBuilder*>( this ); break;
	default:	raiseUnexpectedPortIndex();
	}
	return res;
}

void MethodBuilder_Base::setService( co::IPort* receptacle, co::IService* service )
{
	checkValidReceptacle( receptacle );
	raiseUnexpectedPortIndex();
	CORAL_UNUSED( service );
}

} // namespace co
