/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#include "Module_Base.h"
#include <co/Coral.h>
#include <co/IComponent.h>
#include <co/IPort.h>
#include <co/IInterface.h>

namespace co {

// ------ co.Module provides an interface named 'module', of type co.IModule ------ //

co::IInterface* Module_co_IModule::getInterfaceType()
{
	return co::typeOf<co::IModule>::get();
}

const std::string& Module_co_IModule::getInterfaceName()
{
	static const std::string s_interfaceName( "module" );
	return s_interfaceName;
}

// ------ Module_Base ------ //

Module_Base::Module_Base()
{
	// empty
}

Module_Base::~Module_Base()
{
	// empty
}

co::IObject* Module_Base::getInterfaceOwner()
{
	return this;
}

void Module_Base::componentRetain()
{
	incrementRefCount();
}

void Module_Base::componentRelease()
{
	decrementRefCount();
}

co::IComponent* Module_Base::getComponentType()
{
	co::IType* type = co::getType( "co.Module" );
	assert( dynamic_cast<co::IComponent*>( type ) );
	return static_cast<co::IComponent*>( type );
}

co::IService* Module_Base::getInterface( co::IPort* port )
{
	checkValidPort( port );
	co::IService* res = NULL;
	switch( port->getIndex() )
	{
	case 0:		res = co::disambiguate<co::IService, co::IModule>( this ); break;
	default:	raiseUnexpectedInterfaceIndex();
	}
	return res;
}

void Module_Base::setReceptacle( co::IPort* receptacle, co::IService* facet )
{
	checkValidReceptacle( receptacle );
	raiseUnexpectedInterfaceIndex();
	CORAL_UNUSED( facet );
}

} // namespace co
