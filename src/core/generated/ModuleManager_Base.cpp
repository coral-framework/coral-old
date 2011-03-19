/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#include "ModuleManager_Base.h"
#include <co/Coral.h>
#include <co/IComponentType.h>
#include <co/IInterfaceInfo.h>
#include <co/IInterfaceType.h>

namespace co {

// ------ co.ModuleManager provides an interface named 'moduleManager', of type co.IModuleManager ------ //

co::IInterfaceType* ModuleManager_co_IModuleManager::getInterfaceType()
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

co::IComponent* ModuleManager_Base::getInterfaceOwner()
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

co::IComponentType* ModuleManager_Base::getComponentType()
{
	co::IType* type = co::getType( "co.ModuleManager" );
	assert( dynamic_cast<co::IComponentType*>( type ) );
	return static_cast<co::IComponentType*>( type );
}

co::Interface* ModuleManager_Base::getInterface( co::IInterfaceInfo* interfaceInfo )
{
	checkValidInterface( interfaceInfo );
	co::Interface* res = NULL;
	switch( interfaceInfo->getIndex() )
	{
	case 0:		res = co::disambiguate<co::Interface, co::IModuleManager>( this ); break;
	default:	raiseUnexpectedInterfaceIndex();
	}
	return res;
}

void ModuleManager_Base::setReceptacle( co::IInterfaceInfo* receptacle, co::Interface* facet )
{
	checkValidReceptacle( receptacle );
	raiseUnexpectedInterfaceIndex();
	CORAL_UNUSED( facet );
}

} // namespace co
