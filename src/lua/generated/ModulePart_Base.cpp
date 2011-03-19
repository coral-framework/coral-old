/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#include "ModulePart_Base.h"
#include <co/Coral.h>
#include <co/IComponentType.h>
#include <co/IInterfaceInfo.h>
#include <co/IInterfaceType.h>

namespace lua {

void moduleRetain();
void moduleRelease();

// ------ lua.ModulePart provides an interface named 'part', of type co.IModulePart ------ //

co::IInterfaceType* ModulePart_co_IModulePart::getInterfaceType()
{
	return co::typeOf<co::IModulePart>::get();
}

const std::string& ModulePart_co_IModulePart::getInterfaceName()
{
	static const std::string s_interfaceName( "part" );
	return s_interfaceName;
}

// ------ ModulePart_Base ------ //

ModulePart_Base::ModulePart_Base()
{
	moduleRetain();
}

ModulePart_Base::~ModulePart_Base()
{
	moduleRelease();
}

co::IComponent* ModulePart_Base::getInterfaceOwner()
{
	return this;
}

void ModulePart_Base::componentRetain()
{
	incrementRefCount();
}

void ModulePart_Base::componentRelease()
{
	decrementRefCount();
}

co::IComponentType* ModulePart_Base::getComponentType()
{
	co::IType* type = co::getType( "lua.ModulePart" );
	assert( dynamic_cast<co::IComponentType*>( type ) );
	return static_cast<co::IComponentType*>( type );
}

co::Interface* ModulePart_Base::getInterface( co::IInterfaceInfo* interfaceInfo )
{
	checkValidInterface( interfaceInfo );
	co::Interface* res = NULL;
	switch( interfaceInfo->getIndex() )
	{
	case 0:		res = co::disambiguate<co::Interface, co::IModulePart>( this ); break;
	default:	raiseUnexpectedInterfaceIndex();
	}
	return res;
}

void ModulePart_Base::setReceptacle( co::IInterfaceInfo* receptacle, co::Interface* facet )
{
	checkValidReceptacle( receptacle );
	raiseUnexpectedInterfaceIndex();
	CORAL_UNUSED( facet );
}

} // namespace lua
