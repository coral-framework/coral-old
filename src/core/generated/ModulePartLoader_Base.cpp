/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#include "ModulePartLoader_Base.h"
#include <co/Coral.h>
#include <co/IComponentType.h>
#include <co/IInterfaceInfo.h>
#include <co/IInterfaceType.h>

namespace co {

// ------ co.ModulePartLoader provides an interface named 'loader', of type co.IModulePartLoader ------ //

co::IInterfaceType* ModulePartLoader_co_IModulePartLoader::getInterfaceType()
{
	return co::typeOf<co::IModulePartLoader>::get();
}

const std::string& ModulePartLoader_co_IModulePartLoader::getInterfaceName()
{
	static const std::string s_interfaceName( "loader" );
	return s_interfaceName;
}

// ------ ModulePartLoader_Base ------ //

ModulePartLoader_Base::ModulePartLoader_Base()
{
	// empty
}

ModulePartLoader_Base::~ModulePartLoader_Base()
{
	// empty
}

co::IComponent* ModulePartLoader_Base::getInterfaceOwner()
{
	return this;
}

void ModulePartLoader_Base::componentRetain()
{
	incrementRefCount();
}

void ModulePartLoader_Base::componentRelease()
{
	decrementRefCount();
}

co::IComponentType* ModulePartLoader_Base::getComponentType()
{
	co::IType* type = co::getType( "co.ModulePartLoader" );
	assert( dynamic_cast<co::IComponentType*>( type ) );
	return static_cast<co::IComponentType*>( type );
}

co::Interface* ModulePartLoader_Base::getInterface( co::IInterfaceInfo* interfaceInfo )
{
	checkValidInterface( interfaceInfo );
	co::Interface* res = NULL;
	switch( interfaceInfo->getIndex() )
	{
	case 0:		res = co::disambiguate<co::Interface, co::IModulePartLoader>( this ); break;
	default:	raiseUnexpectedInterfaceIndex();
	}
	return res;
}

void ModulePartLoader_Base::setReceptacle( co::IInterfaceInfo* receptacle, co::Interface* facet )
{
	checkValidReceptacle( receptacle );
	raiseUnexpectedInterfaceIndex();
	CORAL_UNUSED( facet );
}

} // namespace co
