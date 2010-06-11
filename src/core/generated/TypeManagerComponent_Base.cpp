/*******************************************************************************
** Base class generated for component 'co.TypeManagerComponent'
**
** Created: Thu Jun 10 20:53:30 2010
**      by: Coral Compiler version 0.1.0
**
** WARNING! All changes made in this file will be lost when recompiling!
********************************************************************************/

#include "TypeManagerComponent_Base.h"
#include <co/Coral.h>
#include <co/ComponentType.h>
#include <co/InterfaceInfo.h>

// ------ co.TypeManagerComponent provides an interface named 'typeManager', of type co.TypeManager ------ //

co::InterfaceType* co::TypeManagerComponent_co_TypeManager::getInterfaceType()
{
	return co::typeOf<co::TypeManager>::get();
}

const std::string& co::TypeManagerComponent_co_TypeManager::getInterfaceName()
{
	static const std::string s_interfaceName( "typeManager" );
	return s_interfaceName;
}

// ------ TypeManagerComponent_Base ------ //

co::TypeManagerComponent_Base::TypeManagerComponent_Base()
{
	// empty
}

co::TypeManagerComponent_Base::~TypeManagerComponent_Base()
{
	// empty
}

co::Component* co::TypeManagerComponent_Base::getInterfaceOwner()
{
	return this;
}

void co::TypeManagerComponent_Base::componentRetain()
{
	incrementRefCount();
}

void co::TypeManagerComponent_Base::componentRelease()
{
	decrementRefCount();
}

co::ComponentType* co::TypeManagerComponent_Base::getComponentType()
{
	co::Type* type = co::getType( "co.TypeManagerComponent" );
	co::ComponentType* componentType = dynamic_cast<co::ComponentType*>( type );
	assert( componentType );
	return componentType;
}

co::Interface* co::TypeManagerComponent_Base::getInterface( co::InterfaceInfo* interfaceInfo )
{
	checkValidInterface( interfaceInfo );
	co::Interface* res = NULL;
	switch( interfaceInfo->getIndex() )
	{
	case 0:		res = co::disambiguate<co::Interface, co::TypeManager>( this ); break;
	default:	raiseUnexpectedInterfaceIndex();
	}
	return res;
}

void co::TypeManagerComponent_Base::bindInterface( co::InterfaceInfo* clientInterface, co::Interface* instance )
{
	checkValidClientInterface( clientInterface );
	raiseUnexpectedInterfaceIndex();
	CORAL_UNUSED( instance );
}
