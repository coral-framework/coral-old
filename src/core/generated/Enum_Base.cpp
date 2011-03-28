/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#include "Enum_Base.h"
#include <co/Coral.h>
#include <co/IComponent.h>
#include <co/IPort.h>
#include <co/IInterface.h>

namespace co {

// ------ co.Enum provides an interface named 'type', of type co.IEnum ------ //

co::IInterface* Enum_co_IEnum::getInterfaceType()
{
	return co::typeOf<co::IEnum>::get();
}

const std::string& Enum_co_IEnum::getInterfaceName()
{
	static const std::string s_interfaceName( "type" );
	return s_interfaceName;
}

// ------ Enum_Base ------ //

Enum_Base::Enum_Base()
{
	// empty
}

Enum_Base::~Enum_Base()
{
	// empty
}

co::IObject* Enum_Base::getInterfaceOwner()
{
	return this;
}

void Enum_Base::componentRetain()
{
	incrementRefCount();
}

void Enum_Base::componentRelease()
{
	decrementRefCount();
}

co::IComponent* Enum_Base::getComponentType()
{
	co::IType* type = co::getType( "co.Enum" );
	assert( dynamic_cast<co::IComponent*>( type ) );
	return static_cast<co::IComponent*>( type );
}

co::IService* Enum_Base::getInterface( co::IPort* port )
{
	checkValidPort( port );
	co::IService* res = NULL;
	switch( port->getIndex() )
	{
	case 0:		res = co::disambiguate<co::IService, co::IEnum>( this ); break;
	default:	raiseUnexpectedPortIndex();
	}
	return res;
}

void Enum_Base::setReceptacle( co::IPort* receptacle, co::IService* service )
{
	checkValidReceptacle( receptacle );
	raiseUnexpectedPortIndex();
	CORAL_UNUSED( service );
}

} // namespace co
