/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#include "Type_Base.h"
#include <co/Coral.h>
#include <co/IComponent.h>
#include <co/IPort.h>
#include <co/IInterface.h>

namespace co {

// ------ co.Type provides an interface named 'type', of type co.IType ------ //

co::IInterface* Type_co_IType::getInterfaceType()
{
	return co::typeOf<co::IType>::get();
}

const std::string& Type_co_IType::getInterfaceName()
{
	static const std::string s_interfaceName( "type" );
	return s_interfaceName;
}

// ------ Type_Base ------ //

Type_Base::Type_Base()
{
	// empty
}

Type_Base::~Type_Base()
{
	// empty
}

co::IObject* Type_Base::getInterfaceOwner()
{
	return this;
}

void Type_Base::componentRetain()
{
	incrementRefCount();
}

void Type_Base::componentRelease()
{
	decrementRefCount();
}

co::IComponent* Type_Base::getComponentType()
{
	co::IType* type = co::getType( "co.Type" );
	assert( dynamic_cast<co::IComponent*>( type ) );
	return static_cast<co::IComponent*>( type );
}

co::IService* Type_Base::getInterface( co::IPort* port )
{
	checkValidPort( port );
	co::IService* res = NULL;
	switch( port->getIndex() )
	{
	case 0:		res = co::disambiguate<co::IService, co::IType>( this ); break;
	default:	raiseUnexpectedPortIndex();
	}
	return res;
}

void Type_Base::setReceptacle( co::IPort* receptacle, co::IService* service )
{
	checkValidReceptacle( receptacle );
	raiseUnexpectedPortIndex();
	CORAL_UNUSED( service );
}

} // namespace co
