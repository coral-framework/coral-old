/*
 * Coral - A C++ Component Framework.
 * See Copyright Notice in Coral.h
 */

#include "lua_Base.h"
#include <co/Coral.h>
#include <co/ComponentType.h>
#include <co/InterfaceInfo.h>
#include <co/InterfaceType.h>

namespace lua {

void moduleRetain();
void moduleRelease();

// ------ lua.lua provides an interface named 'part', of type co.ModulePart ------ //

co::InterfaceType* lua_co_ModulePart::getInterfaceType()
{
	return co::typeOf<co::ModulePart>::get();
}

const std::string& lua_co_ModulePart::getInterfaceName()
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

co::Component* lua_Base::getInterfaceOwner()
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

co::ComponentType* lua_Base::getComponentType()
{
	co::Type* type = co::getType( "lua.lua" );
	assert( dynamic_cast<co::ComponentType*>( type ) );
	return static_cast<co::ComponentType*>( type );
}

co::Interface* lua_Base::getInterface( co::InterfaceInfo* interfaceInfo )
{
	checkValidInterface( interfaceInfo );
	co::Interface* res = NULL;
	switch( interfaceInfo->getIndex() )
	{
	case 0:		res = co::disambiguate<co::Interface, co::ModulePart>( this ); break;
	default:	raiseUnexpectedInterfaceIndex();
	}
	return res;
}

void lua_Base::bindInterface( co::InterfaceInfo* clientInterface, co::Interface* instance )
{
	checkValidClientInterface( clientInterface );
	raiseUnexpectedInterfaceIndex();
	CORAL_UNUSED( instance );
}

} // namespace lua
