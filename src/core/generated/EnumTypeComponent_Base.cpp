/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

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

void EnumTypeComponent_Base::setReceptacle( co::InterfaceInfo* receptacle, co::Interface* facet )
{
	checkValidReceptacle( receptacle );
	raiseUnexpectedInterfaceIndex();
	CORAL_UNUSED( facet );
}

} // namespace co
