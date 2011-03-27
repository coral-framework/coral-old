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

// ------ co.Field provides an interface named 'attributeInfo', of type co.IField ------ //

co::IInterface* Field_co_IField::getInterfaceType()
{
	return co::typeOf<co::IField>::get();
}

const std::string& Field_co_IField::getInterfaceName()
{
	static const std::string s_interfaceName( "attributeInfo" );
	return s_interfaceName;
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

co::IObject* Field_Base::getInterfaceOwner()
{
	return this;
}

void Field_Base::componentRetain()
{
	incrementRefCount();
}

void Field_Base::componentRelease()
{
	decrementRefCount();
}

co::IComponent* Field_Base::getComponentType()
{
	co::IType* type = co::getType( "co.Field" );
	assert( dynamic_cast<co::IComponent*>( type ) );
	return static_cast<co::IComponent*>( type );
}

co::IService* Field_Base::getInterface( co::IPort* port )
{
	checkValidPort( port );
	co::IService* res = NULL;
	switch( port->getIndex() )
	{
	case 0:		res = co::disambiguate<co::IService, co::IField>( this ); break;
	default:	raiseUnexpectedInterfaceIndex();
	}
	return res;
}

void Field_Base::setReceptacle( co::IPort* receptacle, co::IService* facet )
{
	checkValidReceptacle( receptacle );
	raiseUnexpectedInterfaceIndex();
	CORAL_UNUSED( facet );
}

} // namespace co
