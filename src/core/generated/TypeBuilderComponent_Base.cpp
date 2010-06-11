/*******************************************************************************
** Base class generated for component 'co.TypeBuilderComponent'
**
** Created: Thu Jun 10 20:53:30 2010
**      by: Coral Compiler version 0.1.0
**
** WARNING! All changes made in this file will be lost when recompiling!
********************************************************************************/

#include "TypeBuilderComponent_Base.h"
#include <co/Coral.h>
#include <co/ComponentType.h>
#include <co/InterfaceInfo.h>

// ------ co.TypeBuilderComponent provides an interface named 'typeBuilder', of type co.TypeBuilder ------ //

co::InterfaceType* co::TypeBuilderComponent_co_TypeBuilder::getInterfaceType()
{
	return co::typeOf<co::TypeBuilder>::get();
}

const std::string& co::TypeBuilderComponent_co_TypeBuilder::getInterfaceName()
{
	static const std::string s_interfaceName( "typeBuilder" );
	return s_interfaceName;
}

// ------ TypeBuilderComponent_Base ------ //

co::TypeBuilderComponent_Base::TypeBuilderComponent_Base()
{
	// empty
}

co::TypeBuilderComponent_Base::~TypeBuilderComponent_Base()
{
	// empty
}

co::Component* co::TypeBuilderComponent_Base::getInterfaceOwner()
{
	return this;
}

void co::TypeBuilderComponent_Base::componentRetain()
{
	incrementRefCount();
}

void co::TypeBuilderComponent_Base::componentRelease()
{
	decrementRefCount();
}

co::ComponentType* co::TypeBuilderComponent_Base::getComponentType()
{
	co::Type* type = co::getType( "co.TypeBuilderComponent" );
	co::ComponentType* componentType = dynamic_cast<co::ComponentType*>( type );
	assert( componentType );
	return componentType;
}

co::Interface* co::TypeBuilderComponent_Base::getInterface( co::InterfaceInfo* interfaceInfo )
{
	checkValidInterface( interfaceInfo );
	co::Interface* res = NULL;
	switch( interfaceInfo->getIndex() )
	{
	case 0:		res = co::disambiguate<co::Interface, co::TypeBuilder>( this ); break;
	default:	raiseUnexpectedInterfaceIndex();
	}
	return res;
}

void co::TypeBuilderComponent_Base::bindInterface( co::InterfaceInfo* clientInterface, co::Interface* instance )
{
	checkValidClientInterface( clientInterface );
	raiseUnexpectedInterfaceIndex();
	CORAL_UNUSED( instance );
}
