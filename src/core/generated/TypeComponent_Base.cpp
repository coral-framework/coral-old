/*******************************************************************************
** Base class generated for component 'co.TypeComponent'
**
** Created: Mon Jun 28 12:01:36 2010
**      by: Coral Compiler version 0.1.0
**
** WARNING! All changes made in this file will be lost when recompiling!
********************************************************************************/

#include "TypeComponent_Base.h"
#include <co/Coral.h>
#include <co/ComponentType.h>
#include <co/InterfaceInfo.h>
#include <co/InterfaceType.h>

// ------ co.TypeComponent provides an interface named 'type', of type co.Type ------ //

co::InterfaceType* co::TypeComponent_co_Type::getInterfaceType()
{
	return co::typeOf<co::Type>::get();
}

const std::string& co::TypeComponent_co_Type::getInterfaceName()
{
	static const std::string s_interfaceName( "type" );
	return s_interfaceName;
}

// ------ TypeComponent_Base ------ //

co::TypeComponent_Base::TypeComponent_Base()
{
	// empty
}

co::TypeComponent_Base::~TypeComponent_Base()
{
	// empty
}

co::Component* co::TypeComponent_Base::getInterfaceOwner()
{
	return this;
}

void co::TypeComponent_Base::componentRetain()
{
	incrementRefCount();
}

void co::TypeComponent_Base::componentRelease()
{
	decrementRefCount();
}

co::ComponentType* co::TypeComponent_Base::getComponentType()
{
	co::Type* type = co::getType( "co.TypeComponent" );
	assert( dynamic_cast<co::ComponentType*>( type ) );
	return static_cast<co::ComponentType*>( type );
}

co::Interface* co::TypeComponent_Base::getInterface( co::InterfaceInfo* interfaceInfo )
{
	checkValidInterface( interfaceInfo );
	co::Interface* res = NULL;
	switch( interfaceInfo->getIndex() )
	{
	case 0:		res = co::disambiguate<co::Interface, co::Type>( this ); break;
	default:	raiseUnexpectedInterfaceIndex();
	}
	return res;
}

void co::TypeComponent_Base::bindInterface( co::InterfaceInfo* clientInterface, co::Interface* instance )
{
	checkValidClientInterface( clientInterface );
	raiseUnexpectedInterfaceIndex();
	CORAL_UNUSED( instance );
}
