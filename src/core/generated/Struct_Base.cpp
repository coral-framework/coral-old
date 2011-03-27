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

co::IInterface* Struct_co_IStruct::getInterfaceType()
{
	return co::typeOf<co::IStruct>::get();
}

const std::string& Struct_co_IStruct::getInterfaceName()
{
	static const std::string s_interfaceName( "type" );
	return s_interfaceName;
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

co::IObject* Struct_Base::getInterfaceOwner()
{
	return this;
}

void Struct_Base::componentRetain()
{
	incrementRefCount();
}

void Struct_Base::componentRelease()
{
	decrementRefCount();
}

co::IComponent* Struct_Base::getComponentType()
{
	co::IType* type = co::getType( "co.Struct" );
	assert( dynamic_cast<co::IComponent*>( type ) );
	return static_cast<co::IComponent*>( type );
}

co::IService* Struct_Base::getInterface( co::IPort* port )
{
	checkValidPort( port );
	co::IService* res = NULL;
	switch( port->getIndex() )
	{
	case 0:		res = co::disambiguate<co::IService, co::IStruct>( this ); break;
	default:	raiseUnexpectedInterfaceIndex();
	}
	return res;
}

void Struct_Base::setReceptacle( co::IPort* receptacle, co::IService* facet )
{
	checkValidReceptacle( receptacle );
	raiseUnexpectedInterfaceIndex();
	CORAL_UNUSED( facet );
}

} // namespace co
