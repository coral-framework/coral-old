/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#include "System_Base.h"
#include <co/Coral.h>
#include <co/IComponent.h>
#include <co/IPort.h>
#include <co/IInterface.h>

namespace co {

// ------ co.System provides an interface named 'system', of type co.ISystem ------ //

co::IInterface* System_co_ISystem::getInterfaceType()
{
	return co::typeOf<co::ISystem>::get();
}

const std::string& System_co_ISystem::getInterfaceName()
{
	static const std::string s_interfaceName( "system" );
	return s_interfaceName;
}

// ------ System_Base ------ //

System_Base::System_Base()
{
	// empty
}

System_Base::~System_Base()
{
	// empty
}

co::IObject* System_Base::getInterfaceOwner()
{
	return this;
}

void System_Base::componentRetain()
{
	incrementRefCount();
}

void System_Base::componentRelease()
{
	decrementRefCount();
}

co::IComponent* System_Base::getComponentType()
{
	co::IType* type = co::getType( "co.System" );
	assert( dynamic_cast<co::IComponent*>( type ) );
	return static_cast<co::IComponent*>( type );
}

co::IService* System_Base::getInterface( co::IPort* port )
{
	checkValidPort( port );
	co::IService* res = NULL;
	switch( port->getIndex() )
	{
	case 0:		res = co::disambiguate<co::IService, co::ISystem>( this ); break;
	default:	raiseUnexpectedInterfaceIndex();
	}
	return res;
}

void System_Base::setReceptacle( co::IPort* receptacle, co::IService* facet )
{
	checkValidReceptacle( receptacle );
	raiseUnexpectedInterfaceIndex();
	CORAL_UNUSED( facet );
}

} // namespace co
