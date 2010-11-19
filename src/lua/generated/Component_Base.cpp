/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#include "Component_Base.h"
#include <co/Coral.h>
#include <co/ComponentType.h>
#include <co/InterfaceInfo.h>
#include <co/InterfaceType.h>

namespace lua {

void moduleRetain();
void moduleRelease();

// ------ lua.Component provides an interface named 'dynamicProxyHandler', of type co.DynamicProxyHandler ------ //

co::InterfaceType* Component_co_DynamicProxyHandler::getInterfaceType()
{
	return co::typeOf<co::DynamicProxyHandler>::get();
}

const std::string& Component_co_DynamicProxyHandler::getInterfaceName()
{
	static const std::string s_interfaceName( "dynamicProxyHandler" );
	return s_interfaceName;
}

// ------ lua.Component provides an interface named 'reflector', of type co.Reflector ------ //

co::InterfaceType* Component_co_Reflector::getInterfaceType()
{
	return co::typeOf<co::Reflector>::get();
}

const std::string& Component_co_Reflector::getInterfaceName()
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

co::Component* Component_Base::getInterfaceOwner()
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

co::ComponentType* Component_Base::getComponentType()
{
	co::Type* type = co::getType( "lua.Component" );
	assert( dynamic_cast<co::ComponentType*>( type ) );
	return static_cast<co::ComponentType*>( type );
}

co::Interface* Component_Base::getInterface( co::InterfaceInfo* interfaceInfo )
{
	checkValidInterface( interfaceInfo );
	co::Interface* res = NULL;
	switch( interfaceInfo->getIndex() )
	{
	case 0:		res = co::disambiguate<co::Interface, co::DynamicProxyHandler>( this ); break;
	case 1:		res = co::disambiguate<co::Interface, co::Reflector>( this ); break;
	default:	raiseUnexpectedInterfaceIndex();
	}
	return res;
}

void Component_Base::bindInterface( co::InterfaceInfo* receptacle, co::Interface* instance )
{
	checkValidReceptacle( receptacle );
	raiseUnexpectedInterfaceIndex();
	CORAL_UNUSED( instance );
}

} // namespace lua
