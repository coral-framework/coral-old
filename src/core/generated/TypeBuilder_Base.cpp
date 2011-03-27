/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#include "TypeBuilder_Base.h"
#include <co/Coral.h>
#include <co/IComponent.h>
#include <co/IPort.h>
#include <co/IInterface.h>

namespace co {

// ------ co.TypeBuilder provides an interface named 'typeBuilder', of type co.ITypeBuilder ------ //

co::IInterface* TypeBuilder_co_ITypeBuilder::getInterfaceType()
{
	return co::typeOf<co::ITypeBuilder>::get();
}

const std::string& TypeBuilder_co_ITypeBuilder::getInterfaceName()
{
	static const std::string s_interfaceName( "typeBuilder" );
	return s_interfaceName;
}

// ------ TypeBuilder_Base ------ //

TypeBuilder_Base::TypeBuilder_Base()
{
	// empty
}

TypeBuilder_Base::~TypeBuilder_Base()
{
	// empty
}

co::IObject* TypeBuilder_Base::getInterfaceOwner()
{
	return this;
}

void TypeBuilder_Base::componentRetain()
{
	incrementRefCount();
}

void TypeBuilder_Base::componentRelease()
{
	decrementRefCount();
}

co::IComponent* TypeBuilder_Base::getComponentType()
{
	co::IType* type = co::getType( "co.TypeBuilder" );
	assert( dynamic_cast<co::IComponent*>( type ) );
	return static_cast<co::IComponent*>( type );
}

co::IService* TypeBuilder_Base::getInterface( co::IPort* port )
{
	checkValidPort( port );
	co::IService* res = NULL;
	switch( port->getIndex() )
	{
	case 0:		res = co::disambiguate<co::IService, co::ITypeBuilder>( this ); break;
	default:	raiseUnexpectedInterfaceIndex();
	}
	return res;
}

void TypeBuilder_Base::setReceptacle( co::IPort* receptacle, co::IService* facet )
{
	checkValidReceptacle( receptacle );
	raiseUnexpectedInterfaceIndex();
	CORAL_UNUSED( facet );
}

} // namespace co
