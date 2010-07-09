/*******************************************************************************
** Base class generated for component 'co.EnumTypeComponent'
**
** Created: Fri Jul 02 00:14:15 2010
**      by: Coral Compiler version 0.1.0
**
** WARNING! All changes made in this file will be lost when recompiling!
********************************************************************************/

#include "EnumTypeComponent_Base.h"
#include <co/Coral.h>
#include <co/ComponentType.h>
#include <co/InterfaceInfo.h>
#include <co/InterfaceType.h>

namespace co {

// ------ co.EnumTypeComponent provides an interface named 'type', of type co.EnumType ------ //

co::InterfaceType* EnumTypeComponent_co_EnumType::getInterfaceType()
{
	return co::typeOf<co::EnumType>::get();
}

const std::string& EnumTypeComponent_co_EnumType::getInterfaceName()
{
	static const std::string s_interfaceName( "type" );
	return s_interfaceName;
}

// ------ EnumTypeComponent_Base ------ //

EnumTypeComponent_Base::EnumTypeComponent_Base()
{
	// empty
}

EnumTypeComponent_Base::~EnumTypeComponent_Base()
{
	// empty
}

co::Component* EnumTypeComponent_Base::getInterfaceOwner()
{
	return this;
}

void EnumTypeComponent_Base::componentRetain()
{
	incrementRefCount();
}

void EnumTypeComponent_Base::componentRelease()
{
	decrementRefCount();
}

co::ComponentType* EnumTypeComponent_Base::getComponentType()
{
	co::Type* type = co::getType( "co.EnumTypeComponent" );
	assert( dynamic_cast<co::ComponentType*>( type ) );
	return static_cast<co::ComponentType*>( type );
}

co::Interface* EnumTypeComponent_Base::getInterface( co::InterfaceInfo* interfaceInfo )
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

void EnumTypeComponent_Base::bindInterface( co::InterfaceInfo* clientInterface, co::Interface* instance )
{
	checkValidClientInterface( clientInterface );
	raiseUnexpectedInterfaceIndex();
	CORAL_UNUSED( instance );
}

} // namespace co
