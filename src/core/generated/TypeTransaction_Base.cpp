/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#include "TypeTransaction_Base.h"
#include <co/Coral.h>
#include <co/IComponent.h>
#include <co/IPort.h>
#include <co/IInterface.h>

namespace co {

// ------ co.TypeTransaction provides an interface named 'transaction', of type co.ITypeTransaction ------ //

co::IInterface* TypeTransaction_co_ITypeTransaction::getInterfaceType()
{
	return co::typeOf<co::ITypeTransaction>::get();
}

const std::string& TypeTransaction_co_ITypeTransaction::getInterfaceName()
{
	static const std::string s_interfaceName( "transaction" );
	return s_interfaceName;
}

// ------ TypeTransaction_Base ------ //

TypeTransaction_Base::TypeTransaction_Base()
{
	// empty
}

TypeTransaction_Base::~TypeTransaction_Base()
{
	// empty
}

co::IObject* TypeTransaction_Base::getInterfaceOwner()
{
	return this;
}

void TypeTransaction_Base::componentRetain()
{
	incrementRefCount();
}

void TypeTransaction_Base::componentRelease()
{
	decrementRefCount();
}

co::IComponent* TypeTransaction_Base::getComponentType()
{
	co::IType* type = co::getType( "co.TypeTransaction" );
	assert( dynamic_cast<co::IComponent*>( type ) );
	return static_cast<co::IComponent*>( type );
}

co::IService* TypeTransaction_Base::getInterface( co::IPort* port )
{
	checkValidPort( port );
	co::IService* res = NULL;
	switch( port->getIndex() )
	{
	case 0:		res = co::disambiguate<co::IService, co::ITypeTransaction>( this ); break;
	default:	raiseUnexpectedInterfaceIndex();
	}
	return res;
}

void TypeTransaction_Base::setReceptacle( co::IPort* receptacle, co::IService* facet )
{
	checkValidReceptacle( receptacle );
	raiseUnexpectedInterfaceIndex();
	CORAL_UNUSED( facet );
}

} // namespace co
