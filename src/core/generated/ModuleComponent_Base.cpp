/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#include "ModuleComponent_Base.h"
#include <co/Coral.h>
#include <co/ComponentType.h>
#include <co/InterfaceInfo.h>
#include <co/InterfaceType.h>

namespace co {

// ------ co.ModuleComponent provides an interface named 'module', of type co.Module ------ //

co::InterfaceType* ModuleComponent_co_Module::getInterfaceType()
{
	return co::typeOf<co::Module>::get();
}

const std::string& ModuleComponent_co_Module::getInterfaceName()
{
	static const std::string s_interfaceName( "module" );
	return s_interfaceName;
}

// ------ ModuleComponent_Base ------ //

ModuleComponent_Base::ModuleComponent_Base()
{
	// empty
}

ModuleComponent_Base::~ModuleComponent_Base()
{
	// empty
}

co::Component* ModuleComponent_Base::getInterfaceOwner()
{
	return this;
}

void ModuleComponent_Base::componentRetain()
{
	incrementRefCount();
}

void ModuleComponent_Base::componentRelease()
{
	decrementRefCount();
}

co::ComponentType* ModuleComponent_Base::getComponentType()
{
	co::Type* type = co::getType( "co.ModuleComponent" );
	assert( dynamic_cast<co::ComponentType*>( type ) );
	return static_cast<co::ComponentType*>( type );
}

co::Interface* ModuleComponent_Base::getInterface( co::InterfaceInfo* interfaceInfo )
{
	checkValidInterface( interfaceInfo );
	co::Interface* res = NULL;
	switch( interfaceInfo->getIndex() )
	{
	case 0:		res = co::disambiguate<co::Interface, co::Module>( this ); break;
	default:	raiseUnexpectedInterfaceIndex();
	}
	return res;
}

void ModuleComponent_Base::bindInterface( co::InterfaceInfo* receptacle, co::Interface* instance )
{
	checkValidReceptacle( receptacle );
	raiseUnexpectedInterfaceIndex();
	CORAL_UNUSED( instance );
}

} // namespace co
