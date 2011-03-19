/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#include "Module_Base.h"
#include <co/Coral.h>
#include <co/IComponentType.h>
#include <co/IInterfaceInfo.h>
#include <co/IInterfaceType.h>

namespace co {

// ------ co.Module provides an interface named 'module', of type co.IModule ------ //

co::IInterfaceType* Module_co_IModule::getInterfaceType()
{
	return co::typeOf<co::IModule>::get();
}

const std::string& Module_co_IModule::getInterfaceName()
{
	static const std::string s_interfaceName( "module" );
	return s_interfaceName;
}

// ------ Module_Base ------ //

Module_Base::Module_Base()
{
	// empty
}

Module_Base::~Module_Base()
{
	// empty
}

co::IComponent* Module_Base::getInterfaceOwner()
{
	return this;
}

void Module_Base::componentRetain()
{
	incrementRefCount();
}

void Module_Base::componentRelease()
{
	decrementRefCount();
}

co::IComponentType* Module_Base::getComponentType()
{
	co::IType* type = co::getType( "co.Module" );
	assert( dynamic_cast<co::IComponentType*>( type ) );
	return static_cast<co::IComponentType*>( type );
}

co::Interface* Module_Base::getInterface( co::IInterfaceInfo* interfaceInfo )
{
	checkValidInterface( interfaceInfo );
	co::Interface* res = NULL;
	switch( interfaceInfo->getIndex() )
	{
	case 0:		res = co::disambiguate<co::Interface, co::IModule>( this ); break;
	default:	raiseUnexpectedInterfaceIndex();
	}
	return res;
}

void Module_Base::setReceptacle( co::IInterfaceInfo* receptacle, co::Interface* facet )
{
	checkValidReceptacle( receptacle );
	raiseUnexpectedInterfaceIndex();
	CORAL_UNUSED( facet );
}

} // namespace co
