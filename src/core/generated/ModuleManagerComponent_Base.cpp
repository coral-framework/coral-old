/*******************************************************************************
** Base class generated for component 'co.ModuleManagerComponent'
**
** Created: Wed Aug 25 16:31:31 2010
**      by: Coral Compiler version 0.1.0
**
** WARNING! All changes made in this file will be lost when recompiling!
********************************************************************************/

#include "ModuleManagerComponent_Base.h"
#include <co/Coral.h>
#include <co/ComponentType.h>
#include <co/InterfaceInfo.h>
#include <co/InterfaceType.h>

namespace co {

// ------ co.ModuleManagerComponent provides an interface named 'moduleManager', of type co.ModuleManager ------ //

co::InterfaceType* ModuleManagerComponent_co_ModuleManager::getInterfaceType()
{
	return co::typeOf<co::ModuleManager>::get();
}

const std::string& ModuleManagerComponent_co_ModuleManager::getInterfaceName()
{
	static const std::string s_interfaceName( "moduleManager" );
	return s_interfaceName;
}

// ------ ModuleManagerComponent_Base ------ //

ModuleManagerComponent_Base::ModuleManagerComponent_Base()
{
	// empty
}

ModuleManagerComponent_Base::~ModuleManagerComponent_Base()
{
	// empty
}

co::Component* ModuleManagerComponent_Base::getInterfaceOwner()
{
	return this;
}

void ModuleManagerComponent_Base::componentRetain()
{
	incrementRefCount();
}

void ModuleManagerComponent_Base::componentRelease()
{
	decrementRefCount();
}

co::ComponentType* ModuleManagerComponent_Base::getComponentType()
{
	co::Type* type = co::getType( "co.ModuleManagerComponent" );
	assert( dynamic_cast<co::ComponentType*>( type ) );
	return static_cast<co::ComponentType*>( type );
}

co::Interface* ModuleManagerComponent_Base::getInterface( co::InterfaceInfo* interfaceInfo )
{
	checkValidInterface( interfaceInfo );
	co::Interface* res = NULL;
	switch( interfaceInfo->getIndex() )
	{
	case 0:		res = co::disambiguate<co::Interface, co::ModuleManager>( this ); break;
	default:	raiseUnexpectedInterfaceIndex();
	}
	return res;
}

void ModuleManagerComponent_Base::bindInterface( co::InterfaceInfo* clientInterface, co::Interface* instance )
{
	checkValidClientInterface( clientInterface );
	raiseUnexpectedInterfaceIndex();
	CORAL_UNUSED( instance );
}

} // namespace co
