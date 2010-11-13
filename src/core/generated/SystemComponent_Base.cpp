/*
 * Coral - Lightweight C++ Component Framework.
 * See Copyright Notice in Coral.h
 */

#include "SystemComponent_Base.h"
#include <co/Coral.h>
#include <co/ComponentType.h>
#include <co/InterfaceInfo.h>
#include <co/InterfaceType.h>

namespace co {

// ------ co.SystemComponent provides an interface named 'system', of type co.System ------ //

co::InterfaceType* SystemComponent_co_System::getInterfaceType()
{
	return co::typeOf<co::System>::get();
}

const std::string& SystemComponent_co_System::getInterfaceName()
{
	static const std::string s_interfaceName( "system" );
	return s_interfaceName;
}

// ------ SystemComponent_Base ------ //

SystemComponent_Base::SystemComponent_Base()
{
	// empty
}

SystemComponent_Base::~SystemComponent_Base()
{
	// empty
}

co::Component* SystemComponent_Base::getInterfaceOwner()
{
	return this;
}

void SystemComponent_Base::componentRetain()
{
	incrementRefCount();
}

void SystemComponent_Base::componentRelease()
{
	decrementRefCount();
}

co::ComponentType* SystemComponent_Base::getComponentType()
{
	co::Type* type = co::getType( "co.SystemComponent" );
	assert( dynamic_cast<co::ComponentType*>( type ) );
	return static_cast<co::ComponentType*>( type );
}

co::Interface* SystemComponent_Base::getInterface( co::InterfaceInfo* interfaceInfo )
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

void SystemComponent_Base::bindInterface( co::InterfaceInfo* receptacle, co::Interface* instance )
{
	checkValidReceptacle( receptacle );
	raiseUnexpectedInterfaceIndex();
	CORAL_UNUSED( instance );
}

} // namespace co
