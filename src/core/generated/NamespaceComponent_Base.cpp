/*******************************************************************************
** Base class generated for component 'co.NamespaceComponent'
**
** Created: Mon Jun 28 12:01:36 2010
**      by: Coral Compiler version 0.1.0
**
** WARNING! All changes made in this file will be lost when recompiling!
********************************************************************************/

#include "NamespaceComponent_Base.h"
#include <co/Coral.h>
#include <co/ComponentType.h>
#include <co/InterfaceInfo.h>
#include <co/InterfaceType.h>

// ------ co.NamespaceComponent provides an interface named 'namespace', of type co.Namespace ------ //

co::InterfaceType* co::NamespaceComponent_co_Namespace::getInterfaceType()
{
	return co::typeOf<co::Namespace>::get();
}

const std::string& co::NamespaceComponent_co_Namespace::getInterfaceName()
{
	static const std::string s_interfaceName( "namespace" );
	return s_interfaceName;
}

// ------ NamespaceComponent_Base ------ //

co::NamespaceComponent_Base::NamespaceComponent_Base()
{
	// empty
}

co::NamespaceComponent_Base::~NamespaceComponent_Base()
{
	// empty
}

co::Component* co::NamespaceComponent_Base::getInterfaceOwner()
{
	return this;
}

void co::NamespaceComponent_Base::componentRetain()
{
	incrementRefCount();
}

void co::NamespaceComponent_Base::componentRelease()
{
	decrementRefCount();
}

co::ComponentType* co::NamespaceComponent_Base::getComponentType()
{
	co::Type* type = co::getType( "co.NamespaceComponent" );
	assert( dynamic_cast<co::ComponentType*>( type ) );
	return static_cast<co::ComponentType*>( type );
}

co::Interface* co::NamespaceComponent_Base::getInterface( co::InterfaceInfo* interfaceInfo )
{
	checkValidInterface( interfaceInfo );
	co::Interface* res = NULL;
	switch( interfaceInfo->getIndex() )
	{
	case 0:		res = co::disambiguate<co::Interface, co::Namespace>( this ); break;
	default:	raiseUnexpectedInterfaceIndex();
	}
	return res;
}

void co::NamespaceComponent_Base::bindInterface( co::InterfaceInfo* clientInterface, co::Interface* instance )
{
	checkValidClientInterface( clientInterface );
	raiseUnexpectedInterfaceIndex();
	CORAL_UNUSED( instance );
}
