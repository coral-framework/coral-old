/*******************************************************************************
** Base class generated for component 'co.InterfaceTypeComponent'
**
** Created: Mon Jun 28 12:01:36 2010
**      by: Coral Compiler version 0.1.0
**
** WARNING! All changes made in this file will be lost when recompiling!
********************************************************************************/

#include "InterfaceTypeComponent_Base.h"
#include <co/Coral.h>
#include <co/ComponentType.h>
#include <co/InterfaceInfo.h>
#include <co/InterfaceType.h>

// ------ co.InterfaceTypeComponent provides an interface named 'type', of type co.InterfaceType ------ //

co::InterfaceType* co::InterfaceTypeComponent_co_InterfaceType::getInterfaceType()
{
	return co::typeOf<co::InterfaceType>::get();
}

const std::string& co::InterfaceTypeComponent_co_InterfaceType::getInterfaceName()
{
	static const std::string s_interfaceName( "type" );
	return s_interfaceName;
}

// ------ InterfaceTypeComponent_Base ------ //

co::InterfaceTypeComponent_Base::InterfaceTypeComponent_Base()
{
	// empty
}

co::InterfaceTypeComponent_Base::~InterfaceTypeComponent_Base()
{
	// empty
}

co::Component* co::InterfaceTypeComponent_Base::getInterfaceOwner()
{
	return this;
}

void co::InterfaceTypeComponent_Base::componentRetain()
{
	incrementRefCount();
}

void co::InterfaceTypeComponent_Base::componentRelease()
{
	decrementRefCount();
}

co::ComponentType* co::InterfaceTypeComponent_Base::getComponentType()
{
	co::Type* type = co::getType( "co.InterfaceTypeComponent" );
	assert( dynamic_cast<co::ComponentType*>( type ) );
	return static_cast<co::ComponentType*>( type );
}

co::Interface* co::InterfaceTypeComponent_Base::getInterface( co::InterfaceInfo* interfaceInfo )
{
	checkValidInterface( interfaceInfo );
	co::Interface* res = NULL;
	switch( interfaceInfo->getIndex() )
	{
	case 0:		res = co::disambiguate<co::Interface, co::InterfaceType>( this ); break;
	default:	raiseUnexpectedInterfaceIndex();
	}
	return res;
}

void co::InterfaceTypeComponent_Base::bindInterface( co::InterfaceInfo* clientInterface, co::Interface* instance )
{
	checkValidClientInterface( clientInterface );
	raiseUnexpectedInterfaceIndex();
	CORAL_UNUSED( instance );
}
