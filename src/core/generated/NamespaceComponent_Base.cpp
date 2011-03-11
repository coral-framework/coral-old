/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#include "NamespaceComponent_Base.h"
#include <co/Coral.h>
#include <co/ComponentType.h>
#include <co/InterfaceInfo.h>
#include <co/InterfaceType.h>

namespace co {

// ------ co.NamespaceComponent provides an interface named 'namespace', of type co.Namespace ------ //

co::InterfaceType* NamespaceComponent_co_Namespace::getInterfaceType()
{
	return co::typeOf<co::Namespace>::get();
}

const std::string& NamespaceComponent_co_Namespace::getInterfaceName()
{
	static const std::string s_interfaceName( "namespace" );
	return s_interfaceName;
}

// ------ NamespaceComponent_Base ------ //

NamespaceComponent_Base::NamespaceComponent_Base()
{
	// empty
}

NamespaceComponent_Base::~NamespaceComponent_Base()
{
	// empty
}

co::Component* NamespaceComponent_Base::getInterfaceOwner()
{
	return this;
}

void NamespaceComponent_Base::componentRetain()
{
	incrementRefCount();
}

void NamespaceComponent_Base::componentRelease()
{
	decrementRefCount();
}

co::ComponentType* NamespaceComponent_Base::getComponentType()
{
	co::Type* type = co::getType( "co.NamespaceComponent" );
	assert( dynamic_cast<co::ComponentType*>( type ) );
	return static_cast<co::ComponentType*>( type );
}

co::Interface* NamespaceComponent_Base::getInterface( co::InterfaceInfo* interfaceInfo )
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

void NamespaceComponent_Base::setReceptacle( co::InterfaceInfo* receptacle, co::Interface* facet )
{
	checkValidReceptacle( receptacle );
	raiseUnexpectedInterfaceIndex();
	CORAL_UNUSED( facet );
}

} // namespace co
