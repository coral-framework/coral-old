/*******************************************************************************
** Base class generated for component 'co.AttributeInfoComponent'
**
** Created: Fri Jul 02 00:14:15 2010
**      by: Coral Compiler version 0.1.0
**
** WARNING! All changes made in this file will be lost when recompiling!
********************************************************************************/

#include "AttributeInfoComponent_Base.h"
#include <co/Coral.h>
#include <co/ComponentType.h>
#include <co/InterfaceInfo.h>
#include <co/InterfaceType.h>

namespace co {

// ------ co.AttributeInfoComponent provides an interface named 'attributeInfo', of type co.AttributeInfo ------ //

co::InterfaceType* AttributeInfoComponent_co_AttributeInfo::getInterfaceType()
{
	return co::typeOf<co::AttributeInfo>::get();
}

const std::string& AttributeInfoComponent_co_AttributeInfo::getInterfaceName()
{
	static const std::string s_interfaceName( "attributeInfo" );
	return s_interfaceName;
}

// ------ AttributeInfoComponent_Base ------ //

AttributeInfoComponent_Base::AttributeInfoComponent_Base()
{
	// empty
}

AttributeInfoComponent_Base::~AttributeInfoComponent_Base()
{
	// empty
}

co::Component* AttributeInfoComponent_Base::getInterfaceOwner()
{
	return this;
}

void AttributeInfoComponent_Base::componentRetain()
{
	incrementRefCount();
}

void AttributeInfoComponent_Base::componentRelease()
{
	decrementRefCount();
}

co::ComponentType* AttributeInfoComponent_Base::getComponentType()
{
	co::Type* type = co::getType( "co.AttributeInfoComponent" );
	assert( dynamic_cast<co::ComponentType*>( type ) );
	return static_cast<co::ComponentType*>( type );
}

co::Interface* AttributeInfoComponent_Base::getInterface( co::InterfaceInfo* interfaceInfo )
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

void AttributeInfoComponent_Base::bindInterface( co::InterfaceInfo* clientInterface, co::Interface* instance )
{
	checkValidClientInterface( clientInterface );
	raiseUnexpectedInterfaceIndex();
	CORAL_UNUSED( instance );
}

} // namespace co
