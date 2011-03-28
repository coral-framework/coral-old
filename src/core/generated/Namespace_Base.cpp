/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#include "Namespace_Base.h"
#include <co/Coral.h>
#include <co/IComponent.h>
#include <co/IPort.h>
#include <co/IInterface.h>

namespace co {

// ------ co.Namespace provides an interface named 'namespace', of type co.INamespace ------ //

co::IInterface* Namespace_co_INamespace::getInterfaceType()
{
	return co::typeOf<co::INamespace>::get();
}

const std::string& Namespace_co_INamespace::getInterfaceName()
{
	static const std::string s_interfaceName( "namespace" );
	return s_interfaceName;
}

// ------ Namespace_Base ------ //

Namespace_Base::Namespace_Base()
{
	// empty
}

Namespace_Base::~Namespace_Base()
{
	// empty
}

co::IObject* Namespace_Base::getInterfaceOwner()
{
	return this;
}

void Namespace_Base::componentRetain()
{
	incrementRefCount();
}

void Namespace_Base::componentRelease()
{
	decrementRefCount();
}

co::IComponent* Namespace_Base::getComponentType()
{
	co::IType* type = co::getType( "co.Namespace" );
	assert( dynamic_cast<co::IComponent*>( type ) );
	return static_cast<co::IComponent*>( type );
}

co::IService* Namespace_Base::getInterface( co::IPort* port )
{
	checkValidPort( port );
	co::IService* res = NULL;
	switch( port->getIndex() )
	{
	case 0:		res = co::disambiguate<co::IService, co::INamespace>( this ); break;
	default:	raiseUnexpectedPortIndex();
	}
	return res;
}

void Namespace_Base::setReceptacle( co::IPort* receptacle, co::IService* service )
{
	checkValidReceptacle( receptacle );
	raiseUnexpectedPortIndex();
	CORAL_UNUSED( service );
}

} // namespace co
