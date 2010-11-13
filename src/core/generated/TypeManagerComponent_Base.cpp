/*
 * Coral - Lightweight C++ Component Framework.
 * See Copyright Notice in Coral.h
 */

#include "TypeManagerComponent_Base.h"
#include <co/Coral.h>
#include <co/ComponentType.h>
#include <co/InterfaceInfo.h>
#include <co/InterfaceType.h>

namespace co {

// ------ co.TypeManagerComponent provides an interface named 'typeManager', of type co.TypeManager ------ //

co::InterfaceType* TypeManagerComponent_co_TypeManager::getInterfaceType()
{
	return co::typeOf<co::TypeManager>::get();
}

const std::string& TypeManagerComponent_co_TypeManager::getInterfaceName()
{
	static const std::string s_interfaceName( "typeManager" );
	return s_interfaceName;
}

// ------ TypeManagerComponent_Base ------ //

TypeManagerComponent_Base::TypeManagerComponent_Base()
{
	// empty
}

TypeManagerComponent_Base::~TypeManagerComponent_Base()
{
	// empty
}

co::Component* TypeManagerComponent_Base::getInterfaceOwner()
{
	return this;
}

void TypeManagerComponent_Base::componentRetain()
{
	incrementRefCount();
}

void TypeManagerComponent_Base::componentRelease()
{
	decrementRefCount();
}

co::ComponentType* TypeManagerComponent_Base::getComponentType()
{
	co::Type* type = co::getType( "co.TypeManagerComponent" );
	assert( dynamic_cast<co::ComponentType*>( type ) );
	return static_cast<co::ComponentType*>( type );
}

co::Interface* TypeManagerComponent_Base::getInterface( co::InterfaceInfo* interfaceInfo )
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

void TypeManagerComponent_Base::bindInterface( co::InterfaceInfo* receptacle, co::Interface* instance )
{
	checkValidReceptacle( receptacle );
	raiseUnexpectedInterfaceIndex();
	CORAL_UNUSED( instance );
}

} // namespace co
