/*******************************************************************************
** Base class generated for component 'co.EnumTypeComponent'
**
** Created: Mon Jun 28 12:01:36 2010
**      by: Coral Compiler version 0.1.0
**
** WARNING! All changes made in this file will be lost when recompiling!
********************************************************************************/

#include "EnumTypeComponent_Base.h"
#include <co/Coral.h>
#include <co/ComponentType.h>
#include <co/InterfaceInfo.h>
#include <co/InterfaceType.h>

// ------ co.EnumTypeComponent provides an interface named 'type', of type co.EnumType ------ //

co::InterfaceType* co::EnumTypeComponent_co_EnumType::getInterfaceType()
{
	return co::typeOf<co::EnumType>::get();
}

const std::string& co::EnumTypeComponent_co_EnumType::getInterfaceName()
{
	static const std::string s_interfaceName( "type" );
	return s_interfaceName;
}

// ------ EnumTypeComponent_Base ------ //

co::EnumTypeComponent_Base::EnumTypeComponent_Base()
{
	// empty
}

co::EnumTypeComponent_Base::~EnumTypeComponent_Base()
{
	// empty
}

co::Component* co::EnumTypeComponent_Base::getInterfaceOwner()
{
	return this;
}

void co::EnumTypeComponent_Base::componentRetain()
{
	incrementRefCount();
}

void co::EnumTypeComponent_Base::componentRelease()
{
	decrementRefCount();
}

co::ComponentType* co::EnumTypeComponent_Base::getComponentType()
{
	co::Type* type = co::getType( "co.EnumTypeComponent" );
	assert( dynamic_cast<co::ComponentType*>( type ) );
	return static_cast<co::ComponentType*>( type );
}

co::Interface* co::EnumTypeComponent_Base::getInterface( co::InterfaceInfo* interfaceInfo )
{
	checkValidInterface( interfaceInfo );
	co::Interface* res = NULL;
	switch( interfaceInfo->getIndex() )
	{
	case 0:		res = co::disambiguate<co::Interface, co::EnumType>( this ); break;
	default:	raiseUnexpectedInterfaceIndex();
	}
	return res;
}

void co::EnumTypeComponent_Base::bindInterface( co::InterfaceInfo* clientInterface, co::Interface* instance )
{
	checkValidClientInterface( clientInterface );
	raiseUnexpectedInterfaceIndex();
	CORAL_UNUSED( instance );
}
