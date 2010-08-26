/*******************************************************************************
** Base class generated for component 'co.ModulePartLoaderComponent'
**
** Created: Wed Aug 25 16:31:31 2010
**      by: Coral Compiler version 0.1.0
**
** WARNING! All changes made in this file will be lost when recompiling!
********************************************************************************/

#include "ModulePartLoaderComponent_Base.h"
#include <co/Coral.h>
#include <co/ComponentType.h>
#include <co/InterfaceInfo.h>
#include <co/InterfaceType.h>

namespace co {

// ------ co.ModulePartLoaderComponent provides an interface named 'loader', of type co.ModulePartLoader ------ //

co::InterfaceType* ModulePartLoaderComponent_co_ModulePartLoader::getInterfaceType()
{
	return co::typeOf<co::ModulePartLoader>::get();
}

const std::string& ModulePartLoaderComponent_co_ModulePartLoader::getInterfaceName()
{
	static const std::string s_interfaceName( "loader" );
	return s_interfaceName;
}

// ------ ModulePartLoaderComponent_Base ------ //

ModulePartLoaderComponent_Base::ModulePartLoaderComponent_Base()
{
	// empty
}

ModulePartLoaderComponent_Base::~ModulePartLoaderComponent_Base()
{
	// empty
}

co::Component* ModulePartLoaderComponent_Base::getInterfaceOwner()
{
	return this;
}

void ModulePartLoaderComponent_Base::componentRetain()
{
	incrementRefCount();
}

void ModulePartLoaderComponent_Base::componentRelease()
{
	decrementRefCount();
}

co::ComponentType* ModulePartLoaderComponent_Base::getComponentType()
{
	co::Type* type = co::getType( "co.ModulePartLoaderComponent" );
	assert( dynamic_cast<co::ComponentType*>( type ) );
	return static_cast<co::ComponentType*>( type );
}

co::Interface* ModulePartLoaderComponent_Base::getInterface( co::InterfaceInfo* interfaceInfo )
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

void ModulePartLoaderComponent_Base::bindInterface( co::InterfaceInfo* clientInterface, co::Interface* instance )
{
	checkValidClientInterface( clientInterface );
	raiseUnexpectedInterfaceIndex();
	CORAL_UNUSED( instance );
}

} // namespace co
