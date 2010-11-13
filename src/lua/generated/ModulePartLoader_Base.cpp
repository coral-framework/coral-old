/*
 * Coral - Lightweight C++ Component Framework.
 * See Copyright Notice in Coral.h
 */

#include "ModulePartLoader_Base.h"
#include <co/Coral.h>
#include <co/ComponentType.h>
#include <co/InterfaceInfo.h>
#include <co/InterfaceType.h>

namespace lua {

void moduleRetain();
void moduleRelease();

// ------ lua.ModulePartLoader provides an interface named 'loader', of type co.ModulePartLoader ------ //

co::InterfaceType* ModulePartLoader_co_ModulePartLoader::getInterfaceType()
{
	return co::typeOf<co::ModulePartLoader>::get();
}

const std::string& ModulePartLoader_co_ModulePartLoader::getInterfaceName()
{
	static const std::string s_interfaceName( "loader" );
	return s_interfaceName;
}

// ------ ModulePartLoader_Base ------ //

ModulePartLoader_Base::ModulePartLoader_Base()
{
	moduleRetain();
}

ModulePartLoader_Base::~ModulePartLoader_Base()
{
	moduleRelease();
}

co::Component* ModulePartLoader_Base::getInterfaceOwner()
{
	return this;
}

void ModulePartLoader_Base::componentRetain()
{
	incrementRefCount();
}

void ModulePartLoader_Base::componentRelease()
{
	decrementRefCount();
}

co::ComponentType* ModulePartLoader_Base::getComponentType()
{
	co::Type* type = co::getType( "lua.ModulePartLoader" );
	assert( dynamic_cast<co::ComponentType*>( type ) );
	return static_cast<co::ComponentType*>( type );
}

co::Interface* ModulePartLoader_Base::getInterface( co::InterfaceInfo* interfaceInfo )
{
	checkValidInterface( interfaceInfo );
	co::Interface* res = NULL;
	switch( interfaceInfo->getIndex() )
	{
	case 0:		res = co::disambiguate<co::Interface, co::ModulePartLoader>( this ); break;
	default:	raiseUnexpectedInterfaceIndex();
	}
	return res;
}

void ModulePartLoader_Base::bindInterface( co::InterfaceInfo* receptacle, co::Interface* instance )
{
	checkValidReceptacle( receptacle );
	raiseUnexpectedInterfaceIndex();
	CORAL_UNUSED( instance );
}

} // namespace lua
