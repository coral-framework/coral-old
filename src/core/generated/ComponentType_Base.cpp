/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#include "ComponentType_Base.h"
#include <co/Coral.h>
#include <co/IComponentType.h>
#include <co/IInterfaceInfo.h>
#include <co/IInterfaceType.h>

namespace co {

// ------ co.ComponentType provides an interface named 'type', of type co.IComponentType ------ //

co::IInterfaceType* ComponentType_co_IComponentType::getInterfaceType()
{
	return co::typeOf<co::IComponentType>::get();
}

const std::string& ComponentType_co_IComponentType::getInterfaceName()
{
	static const std::string s_interfaceName( "type" );
	return s_interfaceName;
}

// ------ ComponentType_Base ------ //

ComponentType_Base::ComponentType_Base()
{
	// empty
}

ComponentType_Base::~ComponentType_Base()
{
	// empty
}

co::IComponent* ComponentType_Base::getInterfaceOwner()
{
	return this;
}

void ComponentType_Base::componentRetain()
{
	incrementRefCount();
}

void ComponentType_Base::componentRelease()
{
	decrementRefCount();
}

co::IComponentType* ComponentType_Base::getComponentType()
{
	co::IType* type = co::getType( "co.ComponentType" );
	assert( dynamic_cast<co::IComponentType*>( type ) );
	return static_cast<co::IComponentType*>( type );
}

co::Interface* ComponentType_Base::getInterface( co::IInterfaceInfo* interfaceInfo )
{
	checkValidInterface( interfaceInfo );
	co::Interface* res = NULL;
	switch( interfaceInfo->getIndex() )
	{
	case 0:		res = co::disambiguate<co::Interface, co::IComponentType>( this ); break;
	default:	raiseUnexpectedInterfaceIndex();
	}
	return res;
}

void ComponentType_Base::setReceptacle( co::IInterfaceInfo* receptacle, co::Interface* facet )
{
	checkValidReceptacle( receptacle );
	raiseUnexpectedInterfaceIndex();
	CORAL_UNUSED( facet );
}

} // namespace co
