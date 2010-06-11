/*******************************************************************************
** Base class generated for component 'co.ModuleComponent'
**
** Created: Thu Jun 10 20:53:30 2010
**      by: Coral Compiler version 0.1.0
**
** WARNING! All changes made in this file will be lost when recompiling!
********************************************************************************/

#include "ModuleComponent_Base.h"
#include <co/Coral.h>
#include <co/ComponentType.h>
#include <co/InterfaceInfo.h>

// ------ co.ModuleComponent provides an interface named 'module', of type co.Module ------ //

co::InterfaceType* co::ModuleComponent_co_Module::getInterfaceType()
{
	return co::typeOf<co::Module>::get();
}

const std::string& co::ModuleComponent_co_Module::getInterfaceName()
{
	static const std::string s_interfaceName( "module" );
	return s_interfaceName;
}

// ------ ModuleComponent_Base ------ //

co::ModuleComponent_Base::ModuleComponent_Base()
{
	// empty
}

co::ModuleComponent_Base::~ModuleComponent_Base()
{
	// empty
}

co::Component* co::ModuleComponent_Base::getInterfaceOwner()
{
	return this;
}

void co::ModuleComponent_Base::componentRetain()
{
	incrementRefCount();
}

void co::ModuleComponent_Base::componentRelease()
{
	decrementRefCount();
}

co::ComponentType* co::ModuleComponent_Base::getComponentType()
{
	co::Type* type = co::getType( "co.ModuleComponent" );
	co::ComponentType* componentType = dynamic_cast<co::ComponentType*>( type );
	assert( componentType );
	return componentType;
}

co::Interface* co::ModuleComponent_Base::getInterface( co::InterfaceInfo* interfaceInfo )
{
	checkValidInterface( interfaceInfo );
	co::Interface* res = NULL;
	switch( interfaceInfo->getIndex() )
	{
	case 0:		res = co::disambiguate<co::Interface, co::Module>( this ); break;
	default:	raiseUnexpectedInterfaceIndex();
	}
	return res;
}

void co::ModuleComponent_Base::bindInterface( co::InterfaceInfo* clientInterface, co::Interface* instance )
{
	checkValidClientInterface( clientInterface );
	raiseUnexpectedInterfaceIndex();
	CORAL_UNUSED( instance );
}
