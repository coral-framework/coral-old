/*******************************************************************************
** Base class generated for component 'co.InterfaceInfoComponent'
**
** Created: Thu Jun 10 20:53:30 2010
**      by: Coral Compiler version 0.1.0
**
** WARNING! All changes made in this file will be lost when recompiling!
********************************************************************************/

#include "InterfaceInfoComponent_Base.h"
#include <co/Coral.h>
#include <co/ComponentType.h>
#include <co/InterfaceInfo.h>

// ------ co.InterfaceInfoComponent provides an interface named 'interfaceInfo', of type co.InterfaceInfo ------ //

co::InterfaceType* co::InterfaceInfoComponent_co_InterfaceInfo::getInterfaceType()
{
	return co::typeOf<co::InterfaceInfo>::get();
}

const std::string& co::InterfaceInfoComponent_co_InterfaceInfo::getInterfaceName()
{
	static const std::string s_interfaceName( "interfaceInfo" );
	return s_interfaceName;
}

// ------ InterfaceInfoComponent_Base ------ //

co::InterfaceInfoComponent_Base::InterfaceInfoComponent_Base()
{
	// empty
}

co::InterfaceInfoComponent_Base::~InterfaceInfoComponent_Base()
{
	// empty
}

co::Component* co::InterfaceInfoComponent_Base::getInterfaceOwner()
{
	return this;
}

void co::InterfaceInfoComponent_Base::componentRetain()
{
	incrementRefCount();
}

void co::InterfaceInfoComponent_Base::componentRelease()
{
	decrementRefCount();
}

co::ComponentType* co::InterfaceInfoComponent_Base::getComponentType()
{
	co::Type* type = co::getType( "co.InterfaceInfoComponent" );
	co::ComponentType* componentType = dynamic_cast<co::ComponentType*>( type );
	assert( componentType );
	return componentType;
}

co::Interface* co::InterfaceInfoComponent_Base::getInterface( co::InterfaceInfo* interfaceInfo )
{
	checkValidInterface( interfaceInfo );
	co::Interface* res = NULL;
	switch( interfaceInfo->getIndex() )
	{
	case 0:		res = co::disambiguate<co::Interface, co::InterfaceInfo>( this ); break;
	default:	raiseUnexpectedInterfaceIndex();
	}
	return res;
}

void co::InterfaceInfoComponent_Base::bindInterface( co::InterfaceInfo* clientInterface, co::Interface* instance )
{
	checkValidClientInterface( clientInterface );
	raiseUnexpectedInterfaceIndex();
	CORAL_UNUSED( instance );
}
