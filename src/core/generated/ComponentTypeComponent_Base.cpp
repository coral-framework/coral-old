/*
 * Coral - A C++ Component Framework.
 * See Copyright Notice in Coral.h
 */

#include "ComponentTypeComponent_Base.h"
#include <co/Coral.h>
#include <co/ComponentType.h>
#include <co/InterfaceInfo.h>
#include <co/InterfaceType.h>

namespace co {

// ------ co.ComponentTypeComponent provides an interface named 'type', of type co.ComponentType ------ //

co::InterfaceType* ComponentTypeComponent_co_ComponentType::getInterfaceType()
{
	return co::typeOf<co::ComponentType>::get();
}

const std::string& ComponentTypeComponent_co_ComponentType::getInterfaceName()
{
	static const std::string s_interfaceName( "type" );
	return s_interfaceName;
}

// ------ ComponentTypeComponent_Base ------ //

ComponentTypeComponent_Base::ComponentTypeComponent_Base()
{
	// empty
}

ComponentTypeComponent_Base::~ComponentTypeComponent_Base()
{
	// empty
}

co::Component* ComponentTypeComponent_Base::getInterfaceOwner()
{
	return this;
}

void ComponentTypeComponent_Base::componentRetain()
{
	incrementRefCount();
}

void ComponentTypeComponent_Base::componentRelease()
{
	decrementRefCount();
}

co::ComponentType* ComponentTypeComponent_Base::getComponentType()
{
	co::Type* type = co::getType( "co.ComponentTypeComponent" );
	assert( dynamic_cast<co::ComponentType*>( type ) );
	return static_cast<co::ComponentType*>( type );
}

co::Interface* ComponentTypeComponent_Base::getInterface( co::InterfaceInfo* interfaceInfo )
{
	checkValidInterface( interfaceInfo );
	co::Interface* res = NULL;
	switch( interfaceInfo->getIndex() )
	{
	case 0:		res = co::disambiguate<co::Interface, co::ComponentType>( this ); break;
	default:	raiseUnexpectedInterfaceIndex();
	}
	return res;
}

void ComponentTypeComponent_Base::bindInterface( co::InterfaceInfo* clientInterface, co::Interface* instance )
{
	checkValidClientInterface( clientInterface );
	raiseUnexpectedInterfaceIndex();
	CORAL_UNUSED( instance );
}

} // namespace co
