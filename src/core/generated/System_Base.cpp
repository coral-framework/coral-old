/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#include "System_Base.h"
#include <co/Coral.h>
#include <co/IComponentType.h>
#include <co/IInterfaceInfo.h>
#include <co/IInterfaceType.h>

namespace co {

// ------ co.System provides an interface named 'system', of type co.ISystem ------ //

co::IInterfaceType* System_co_ISystem::getInterfaceType()
{
	return co::typeOf<co::ISystem>::get();
}

const std::string& System_co_ISystem::getInterfaceName()
{
	static const std::string s_interfaceName( "system" );
	return s_interfaceName;
}

// ------ System_Base ------ //

System_Base::System_Base()
{
	// empty
}

System_Base::~System_Base()
{
	// empty
}

co::IComponent* System_Base::getInterfaceOwner()
{
	return this;
}

void System_Base::componentRetain()
{
	incrementRefCount();
}

void System_Base::componentRelease()
{
	decrementRefCount();
}

co::IComponentType* System_Base::getComponentType()
{
	co::IType* type = co::getType( "co.System" );
	assert( dynamic_cast<co::IComponentType*>( type ) );
	return static_cast<co::IComponentType*>( type );
}

co::Interface* System_Base::getInterface( co::IInterfaceInfo* interfaceInfo )
{
	checkValidInterface( interfaceInfo );
	co::Interface* res = NULL;
	switch( interfaceInfo->getIndex() )
	{
	case 0:		res = co::disambiguate<co::Interface, co::ISystem>( this ); break;
	default:	raiseUnexpectedInterfaceIndex();
	}
	return res;
}

void System_Base::setReceptacle( co::IInterfaceInfo* receptacle, co::Interface* facet )
{
	checkValidReceptacle( receptacle );
	raiseUnexpectedInterfaceIndex();
	CORAL_UNUSED( facet );
}

} // namespace co
