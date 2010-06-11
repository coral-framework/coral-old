/*******************************************************************************
** Base class generated for component 'co.ModuleManagerComponent'
**
** Created: Thu Jun 10 20:53:30 2010
**      by: Coral Compiler version 0.1.0
**
** WARNING! All changes made in this file will be lost when recompiling!
********************************************************************************/

#include "ModuleManagerComponent_Base.h"
#include <co/Coral.h>
#include <co/ComponentType.h>
#include <co/InterfaceInfo.h>

// ------ co.ModuleManagerComponent provides an interface named 'moduleManager', of type co.ModuleManager ------ //

co::InterfaceType* co::ModuleManagerComponent_co_ModuleManager::getInterfaceType()
{
	return co::typeOf<co::ModuleManager>::get();
}

const std::string& co::ModuleManagerComponent_co_ModuleManager::getInterfaceName()
{
	static const std::string s_interfaceName( "moduleManager" );
	return s_interfaceName;
}

// ------ ModuleManagerComponent_Base ------ //

co::ModuleManagerComponent_Base::ModuleManagerComponent_Base()
{
	// empty
}

co::ModuleManagerComponent_Base::~ModuleManagerComponent_Base()
{
	// empty
}

co::Component* co::ModuleManagerComponent_Base::getInterfaceOwner()
{
	return this;
}

void co::ModuleManagerComponent_Base::componentRetain()
{
	incrementRefCount();
}

void co::ModuleManagerComponent_Base::componentRelease()
{
	decrementRefCount();
}

co::ComponentType* co::ModuleManagerComponent_Base::getComponentType()
{
	co::Type* type = co::getType( "co.ModuleManagerComponent" );
	co::ComponentType* componentType = dynamic_cast<co::ComponentType*>( type );
	assert( componentType );
	return componentType;
}

co::Interface* co::ModuleManagerComponent_Base::getInterface( co::InterfaceInfo* interfaceInfo )
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

void co::ModuleManagerComponent_Base::bindInterface( co::InterfaceInfo* clientInterface, co::Interface* instance )
{
	checkValidClientInterface( clientInterface );
	raiseUnexpectedInterfaceIndex();
	CORAL_UNUSED( instance );
}
