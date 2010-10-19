/*
 * Coral - A C++ Component Framework.
 * See Copyright Notice in Coral.h
 */

#include "InterfaceTypeComponent_Base.h"
#include <co/Coral.h>
#include <co/ComponentType.h>
#include <co/InterfaceInfo.h>
#include <co/InterfaceType.h>

namespace co {

// ------ co.InterfaceTypeComponent provides an interface named 'type', of type co.InterfaceType ------ //

co::InterfaceType* InterfaceTypeComponent_co_InterfaceType::getInterfaceType()
{
	return co::typeOf<co::InterfaceType>::get();
}

const std::string& InterfaceTypeComponent_co_InterfaceType::getInterfaceName()
{
	static const std::string s_interfaceName( "type" );
	return s_interfaceName;
}

// ------ InterfaceTypeComponent_Base ------ //

InterfaceTypeComponent_Base::InterfaceTypeComponent_Base()
{
	// empty
}

InterfaceTypeComponent_Base::~InterfaceTypeComponent_Base()
{
	// empty
}

co::Component* InterfaceTypeComponent_Base::getInterfaceOwner()
{
	return this;
}

void InterfaceTypeComponent_Base::componentRetain()
{
	incrementRefCount();
}

void InterfaceTypeComponent_Base::componentRelease()
{
	decrementRefCount();
}

co::ComponentType* InterfaceTypeComponent_Base::getComponentType()
{
	co::Type* type = co::getType( "co.InterfaceTypeComponent" );
	assert( dynamic_cast<co::ComponentType*>( type ) );
	return static_cast<co::ComponentType*>( type );
}

co::Interface* InterfaceTypeComponent_Base::getInterface( co::InterfaceInfo* interfaceInfo )
{
	checkValidInterface( interfaceInfo );
	co::Interface* res = NULL;
	switch( interfaceInfo->getIndex() )
	{
	case 0:		res = co::disambiguate<co::Interface, co::InterfaceType>( this ); break;
	default:	raiseUnexpectedInterfaceIndex();
	}
	return res;
}

void InterfaceTypeComponent_Base::bindInterface( co::InterfaceInfo* receptacle, co::Interface* instance )
{
	checkValidReceptacle( receptacle );
	raiseUnexpectedInterfaceIndex();
	CORAL_UNUSED( instance );
}

} // namespace co
