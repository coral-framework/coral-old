/*******************************************************************************
** Base class generated for component 'co.SystemComponent'
**
** Created: Mon Jun 28 12:01:36 2010
**      by: Coral Compiler version 0.1.0
**
** WARNING! All changes made in this file will be lost when recompiling!
********************************************************************************/

#include "SystemComponent_Base.h"
#include <co/Coral.h>
#include <co/ComponentType.h>
#include <co/InterfaceInfo.h>
#include <co/InterfaceType.h>

// ------ co.SystemComponent provides an interface named 'system', of type co.System ------ //

co::InterfaceType* co::SystemComponent_co_System::getInterfaceType()
{
	return co::typeOf<co::System>::get();
}

const std::string& co::SystemComponent_co_System::getInterfaceName()
{
	static const std::string s_interfaceName( "system" );
	return s_interfaceName;
}

// ------ SystemComponent_Base ------ //

co::SystemComponent_Base::SystemComponent_Base()
{
	// empty
}

co::SystemComponent_Base::~SystemComponent_Base()
{
	// empty
}

co::Component* co::SystemComponent_Base::getInterfaceOwner()
{
	return this;
}

void co::SystemComponent_Base::componentRetain()
{
	incrementRefCount();
}

void co::SystemComponent_Base::componentRelease()
{
	decrementRefCount();
}

co::ComponentType* co::SystemComponent_Base::getComponentType()
{
	co::Type* type = co::getType( "co.SystemComponent" );
	assert( dynamic_cast<co::ComponentType*>( type ) );
	return static_cast<co::ComponentType*>( type );
}

co::Interface* co::SystemComponent_Base::getInterface( co::InterfaceInfo* interfaceInfo )
{
	checkValidInterface( interfaceInfo );
	co::Interface* res = NULL;
	switch( interfaceInfo->getIndex() )
	{
	case 0:		res = co::disambiguate<co::Interface, co::System>( this ); break;
	default:	raiseUnexpectedInterfaceIndex();
	}
	return res;
}

void co::SystemComponent_Base::bindInterface( co::InterfaceInfo* clientInterface, co::Interface* instance )
{
	checkValidClientInterface( clientInterface );
	raiseUnexpectedInterfaceIndex();
	CORAL_UNUSED( instance );
}
