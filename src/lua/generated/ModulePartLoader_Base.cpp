/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#include "ModulePartLoader_Base.h"
#include <co/Coral.h>
#include <co/IComponent.h>
#include <co/IPort.h>
#include <co/IInterface.h>

namespace lua {

void moduleRetain();
void moduleRelease();

// ------ lua.ModulePartLoader provides an interface named 'loader', of type co.IModulePartLoader ------ //

co::IInterface* ModulePartLoader_co_IModulePartLoader::getInterfaceType()
{
	return co::typeOf<co::IModulePartLoader>::get();
}

const std::string& ModulePartLoader_co_IModulePartLoader::getInterfaceName()
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

co::IObject* ModulePartLoader_Base::getInterfaceOwner()
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

co::IComponent* ModulePartLoader_Base::getComponentType()
{
	co::IType* type = co::getType( "lua.ModulePartLoader" );
	assert( dynamic_cast<co::IComponent*>( type ) );
	return static_cast<co::IComponent*>( type );
}

co::IService* ModulePartLoader_Base::getInterface( co::IPort* port )
{
	checkValidPort( port );
	co::IService* res = NULL;
	switch( port->getIndex() )
	{
	case 0:		res = co::disambiguate<co::IService, co::IModulePartLoader>( this ); break;
	default:	raiseUnexpectedInterfaceIndex();
	}
	return res;
}

void ModulePartLoader_Base::setReceptacle( co::IPort* receptacle, co::IService* facet )
{
	checkValidReceptacle( receptacle );
	raiseUnexpectedInterfaceIndex();
	CORAL_UNUSED( facet );
}

} // namespace lua
