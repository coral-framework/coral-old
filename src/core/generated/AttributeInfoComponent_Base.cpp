/*******************************************************************************
** Base class generated for component 'co.AttributeInfoComponent'
**
** Created: Thu Jun 10 20:53:30 2010
**      by: Coral Compiler version 0.1.0
**
** WARNING! All changes made in this file will be lost when recompiling!
********************************************************************************/

#include "AttributeInfoComponent_Base.h"
#include <co/Coral.h>
#include <co/ComponentType.h>
#include <co/InterfaceInfo.h>

// ------ co.AttributeInfoComponent provides an interface named 'attributeInfo', of type co.AttributeInfo ------ //

co::InterfaceType* co::AttributeInfoComponent_co_AttributeInfo::getInterfaceType()
{
	return co::typeOf<co::AttributeInfo>::get();
}

const std::string& co::AttributeInfoComponent_co_AttributeInfo::getInterfaceName()
{
	static const std::string s_interfaceName( "attributeInfo" );
	return s_interfaceName;
}

// ------ AttributeInfoComponent_Base ------ //

co::AttributeInfoComponent_Base::AttributeInfoComponent_Base()
{
	// empty
}

co::AttributeInfoComponent_Base::~AttributeInfoComponent_Base()
{
	// empty
}

co::Component* co::AttributeInfoComponent_Base::getInterfaceOwner()
{
	return this;
}

void co::AttributeInfoComponent_Base::componentRetain()
{
	incrementRefCount();
}

void co::AttributeInfoComponent_Base::componentRelease()
{
	decrementRefCount();
}

co::ComponentType* co::AttributeInfoComponent_Base::getComponentType()
{
	co::Type* type = co::getType( "co.AttributeInfoComponent" );
	co::ComponentType* componentType = dynamic_cast<co::ComponentType*>( type );
	assert( componentType );
	return componentType;
}

co::Interface* co::AttributeInfoComponent_Base::getInterface( co::InterfaceInfo* interfaceInfo )
{
	checkValidInterface( interfaceInfo );
	co::Interface* res = NULL;
	switch( interfaceInfo->getIndex() )
	{
	case 0:		res = co::disambiguate<co::Interface, co::AttributeInfo>( this ); break;
	default:	raiseUnexpectedInterfaceIndex();
	}
	return res;
}

void co::AttributeInfoComponent_Base::bindInterface( co::InterfaceInfo* clientInterface, co::Interface* instance )
{
	checkValidClientInterface( clientInterface );
	raiseUnexpectedInterfaceIndex();
	CORAL_UNUSED( instance );
}
