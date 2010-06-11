/*******************************************************************************
** Base class generated for component 'co.ModulePartLoaderComponent'
**
** Created: Thu Jun 10 20:53:30 2010
**      by: Coral Compiler version 0.1.0
**
** WARNING! All changes made in this file will be lost when recompiling!
********************************************************************************/

#include "ModulePartLoaderComponent_Base.h"
#include <co/Coral.h>
#include <co/ComponentType.h>
#include <co/InterfaceInfo.h>

// ------ co.ModulePartLoaderComponent provides an interface named 'loader', of type co.ModulePartLoader ------ //

co::InterfaceType* co::ModulePartLoaderComponent_co_ModulePartLoader::getInterfaceType()
{
	return co::typeOf<co::ModulePartLoader>::get();
}

const std::string& co::ModulePartLoaderComponent_co_ModulePartLoader::getInterfaceName()
{
	static const std::string s_interfaceName( "loader" );
	return s_interfaceName;
}

// ------ ModulePartLoaderComponent_Base ------ //

co::ModulePartLoaderComponent_Base::ModulePartLoaderComponent_Base()
{
	// empty
}

co::ModulePartLoaderComponent_Base::~ModulePartLoaderComponent_Base()
{
	// empty
}

co::Component* co::ModulePartLoaderComponent_Base::getInterfaceOwner()
{
	return this;
}

void co::ModulePartLoaderComponent_Base::componentRetain()
{
	incrementRefCount();
}

void co::ModulePartLoaderComponent_Base::componentRelease()
{
	decrementRefCount();
}

co::ComponentType* co::ModulePartLoaderComponent_Base::getComponentType()
{
	co::Type* type = co::getType( "co.ModulePartLoaderComponent" );
	co::ComponentType* componentType = dynamic_cast<co::ComponentType*>( type );
	assert( componentType );
	return componentType;
}

co::Interface* co::ModulePartLoaderComponent_Base::getInterface( co::InterfaceInfo* interfaceInfo )
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

void co::ModulePartLoaderComponent_Base::bindInterface( co::InterfaceInfo* clientInterface, co::Interface* instance )
{
	checkValidClientInterface( clientInterface );
	raiseUnexpectedInterfaceIndex();
	CORAL_UNUSED( instance );
}
