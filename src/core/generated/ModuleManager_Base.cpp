/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#include "ModuleManager_Base.h"
#include <co/Coral.h>
#include <co/IComponent.h>
#include <co/IPort.h>
#include <co/IInterface.h>

namespace co {

// ------ co.ModuleManager provides an interface named 'moduleManager', of type co.IModuleManager ------ //

co::IInterface* ModuleManager_co_IModuleManager::getInterfaceType()
{
	return co::typeOf<co::IModuleManager>::get();
}

const std::string& ModuleManager_co_IModuleManager::getInterfaceName()
{
	static const std::string s_interfaceName( "moduleManager" );
	return s_interfaceName;
}

// ------ ModuleManager_Base ------ //

ModuleManager_Base::ModuleManager_Base()
{
	// empty
}

ModuleManager_Base::~ModuleManager_Base()
{
	// empty
}

co::IObject* ModuleManager_Base::getInterfaceOwner()
{
	return this;
}

void ModuleManager_Base::componentRetain()
{
	incrementRefCount();
}

void ModuleManager_Base::componentRelease()
{
	decrementRefCount();
}

co::IComponent* ModuleManager_Base::getComponentType()
{
	co::IType* type = co::getType( "co.ModuleManager" );
	assert( dynamic_cast<co::IComponent*>( type ) );
	return static_cast<co::IComponent*>( type );
}

co::IService* ModuleManager_Base::getInterface( co::IPort* port )
{
	checkValidPort( port );
	co::IService* res = NULL;
	switch( port->getIndex() )
	{
	case 0:		res = co::disambiguate<co::IService, co::IModuleManager>( this ); break;
	default:	raiseUnexpectedInterfaceIndex();
	}
	return res;
}

void ModuleManager_Base::setReceptacle( co::IPort* receptacle, co::IService* facet )
{
	checkValidReceptacle( receptacle );
	raiseUnexpectedInterfaceIndex();
	CORAL_UNUSED( facet );
}

} // namespace co
