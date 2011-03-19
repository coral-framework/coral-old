/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#include "lua_Base.h"
#include <co/Coral.h>
#include <co/IComponentType.h>
#include <co/IInterfaceInfo.h>
#include <co/IInterfaceType.h>

namespace lua {

void moduleRetain();
void moduleRelease();

// ------ lua.lua provides an interface named 'part', of type co.IModulePart ------ //

co::IInterfaceType* lua_co_IModulePart::getInterfaceType()
{
	return co::typeOf<co::IModulePart>::get();
}

const std::string& lua_co_IModulePart::getInterfaceName()
{
	static const std::string s_interfaceName( "part" );
	return s_interfaceName;
}

// ------ lua_Base ------ //

lua_Base::lua_Base()
{
	moduleRetain();
}

lua_Base::~lua_Base()
{
	moduleRelease();
}

co::IComponent* lua_Base::getInterfaceOwner()
{
	return this;
}

void lua_Base::componentRetain()
{
	incrementRefCount();
}

void lua_Base::componentRelease()
{
	decrementRefCount();
}

co::IComponentType* lua_Base::getComponentType()
{
	co::IType* type = co::getType( "lua.lua" );
	assert( dynamic_cast<co::IComponentType*>( type ) );
	return static_cast<co::IComponentType*>( type );
}

co::Interface* lua_Base::getInterface( co::IInterfaceInfo* interfaceInfo )
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

void lua_Base::setReceptacle( co::IInterfaceInfo* receptacle, co::Interface* facet )
{
	checkValidReceptacle( receptacle );
	raiseUnexpectedInterfaceIndex();
	CORAL_UNUSED( facet );
}

} // namespace lua
