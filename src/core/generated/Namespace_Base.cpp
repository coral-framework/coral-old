/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#include "Namespace_Base.h"
#include <co/Coral.h>
#include <co/IComponentType.h>
#include <co/IInterfaceInfo.h>
#include <co/IInterfaceType.h>

namespace co {

// ------ co.Namespace provides an interface named 'namespace', of type co.INamespace ------ //

co::IInterfaceType* Namespace_co_INamespace::getInterfaceType()
{
	return co::typeOf<co::INamespace>::get();
}

const std::string& Namespace_co_INamespace::getInterfaceName()
{
	static const std::string s_interfaceName( "namespace" );
	return s_interfaceName;
}

// ------ Namespace_Base ------ //

Namespace_Base::Namespace_Base()
{
	// empty
}

Namespace_Base::~Namespace_Base()
{
	// empty
}

co::IComponent* Namespace_Base::getInterfaceOwner()
{
	return this;
}

void Namespace_Base::componentRetain()
{
	incrementRefCount();
}

void Namespace_Base::componentRelease()
{
	decrementRefCount();
}

co::IComponentType* Namespace_Base::getComponentType()
{
	co::IType* type = co::getType( "co.Namespace" );
	assert( dynamic_cast<co::IComponentType*>( type ) );
	return static_cast<co::IComponentType*>( type );
}

co::Interface* Namespace_Base::getInterface( co::IInterfaceInfo* interfaceInfo )
{
	checkValidInterface( interfaceInfo );
	co::Interface* res = NULL;
	switch( interfaceInfo->getIndex() )
	{
	case 0:		res = co::disambiguate<co::Interface, co::INamespace>( this ); break;
	default:	raiseUnexpectedInterfaceIndex();
	}
	return res;
}

void Namespace_Base::setReceptacle( co::IInterfaceInfo* receptacle, co::Interface* facet )
{
	checkValidReceptacle( receptacle );
	raiseUnexpectedInterfaceIndex();
	CORAL_UNUSED( facet );
}

} // namespace co
