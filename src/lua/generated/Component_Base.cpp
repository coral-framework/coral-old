/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#include "Component_Base.h"
#include <co/Coral.h>
#include <co/IComponentType.h>
#include <co/IInterfaceInfo.h>
#include <co/IInterfaceType.h>

namespace lua {

void moduleRetain();
void moduleRelease();

// ------ lua.Component provides an interface named 'dynamicProxyHandler', of type co.IDynamicProxyHandler ------ //

co::IInterfaceType* Component_co_IDynamicProxyHandler::getInterfaceType()
{
	return co::typeOf<co::IDynamicProxyHandler>::get();
}

const std::string& Component_co_IDynamicProxyHandler::getInterfaceName()
{
	static const std::string s_interfaceName( "dynamicProxyHandler" );
	return s_interfaceName;
}

// ------ lua.Component provides an interface named 'reflector', of type co.IReflector ------ //

co::IInterfaceType* Component_co_IReflector::getInterfaceType()
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

co::IComponent* Component_Base::getInterfaceOwner()
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

co::IComponentType* Component_Base::getComponentType()
{
	co::IType* type = co::getType( "lua.Component" );
	assert( dynamic_cast<co::IComponentType*>( type ) );
	return static_cast<co::IComponentType*>( type );
}

co::Interface* Component_Base::getInterface( co::IInterfaceInfo* interfaceInfo )
{
	checkValidInterface( interfaceInfo );
	co::Interface* res = NULL;
	switch( interfaceInfo->getIndex() )
	{
	case 0:		res = co::disambiguate<co::Interface, co::IDynamicProxyHandler>( this ); break;
	case 1:		res = co::disambiguate<co::Interface, co::IReflector>( this ); break;
	default:	raiseUnexpectedInterfaceIndex();
	}
	return res;
}

void Component_Base::setReceptacle( co::IInterfaceInfo* receptacle, co::Interface* facet )
{
	checkValidReceptacle( receptacle );
	raiseUnexpectedInterfaceIndex();
	CORAL_UNUSED( facet );
}

} // namespace lua
