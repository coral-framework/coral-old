/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#include "Component_Base.h"
#include <co/Coral.h>
#include <co/IComponent.h>
#include <co/IPort.h>
#include <co/IInterface.h>

namespace lua {

void moduleRetain();
void moduleRelease();

// ------ lua.Component provides an interface named 'dynamicProxyHandler', of type co.IDynamicServiceProvider ------ //

co::IInterface* Component_co_IDynamicServiceProvider::getInterfaceType()
{
	return co::typeOf<co::IDynamicServiceProvider>::get();
}

const std::string& Component_co_IDynamicServiceProvider::getInterfaceName()
{
	static const std::string s_interfaceName( "dynamicProxyHandler" );
	return s_interfaceName;
}

// ------ lua.Component provides an interface named 'reflector', of type co.IReflector ------ //

co::IInterface* Component_co_IReflector::getInterfaceType()
{
	return co::typeOf<co::IReflector>::get();
}

const std::string& Component_co_IReflector::getInterfaceName()
{
	static const std::string s_interfaceName( "reflector" );
	return s_interfaceName;
}

// ------ Component_Base ------ //

Component_Base::Component_Base()
{
	moduleRetain();
}

Component_Base::~Component_Base()
{
	moduleRelease();
}

co::IObject* Component_Base::getInterfaceOwner()
{
	return this;
}

void Component_Base::componentRetain()
{
	incrementRefCount();
}

void Component_Base::componentRelease()
{
	decrementRefCount();
}

co::IComponent* Component_Base::getComponentType()
{
	co::IType* type = co::getType( "lua.Component" );
	assert( dynamic_cast<co::IComponent*>( type ) );
	return static_cast<co::IComponent*>( type );
}

co::IService* Component_Base::getInterface( co::IPort* port )
{
	checkValidPort( port );
	co::IService* res = NULL;
	switch( port->getIndex() )
	{
	case 0:		res = co::disambiguate<co::IService, co::IDynamicServiceProvider>( this ); break;
	case 1:		res = co::disambiguate<co::IService, co::IReflector>( this ); break;
	default:	raiseUnexpectedInterfaceIndex();
	}
	return res;
}

void Component_Base::setReceptacle( co::IPort* receptacle, co::IService* facet )
{
	checkValidReceptacle( receptacle );
	raiseUnexpectedInterfaceIndex();
	CORAL_UNUSED( facet );
}

} // namespace lua
