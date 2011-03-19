/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#include "ModulePartBase.h"
#include <co/Coral.h>
#include <co/IModule.h>
#include <co/RefPtr.h>
#include <co/Exception.h>
#include <co/INamespace.h>
#include <co/ITypeBuilder.h>
#include <co/IInterfaceInfo.h>
#include <co/IInterfaceType.h>
#include <co/ITypeCreationTransaction.h>

namespace co {

// ------ ModulePartBase provides an interface named 'part', of type co::IModulePart ------ //

IInterfaceType* ModulePartBase_co_ModulePart::getInterfaceType()
{
	return co::typeOf<IModulePart>::get();
}

const std::string& ModulePartBase_co_ModulePart::getInterfaceName()
{
	static const std::string s_interfaceName( "part" );
	return s_interfaceName;
}

// ------ ModulePartBase ------ //

ModulePartBase::ModulePartBase()
{
	// empty
}

ModulePartBase::~ModulePartBase()
{
	// empty
}

IComponent* ModulePartBase::getInterfaceOwner()
{
	return this;
}

void ModulePartBase::componentRetain()
{
	incrementRefCount();
}

void ModulePartBase::componentRelease()
{
	decrementRefCount();
}

IComponentType* ModulePartBase::getComponentType()
{
	return getOrCreateSimpleInternalComponentType( "co.ModulePartBase", "co.IModulePart", "part" );
}

Interface* ModulePartBase::getInterface( IInterfaceInfo* interfaceInfo )
{
	checkValidInterface( interfaceInfo );

	Interface* res = NULL;
	switch( interfaceInfo->getIndex() )
	{
	case 0: res = static_cast<IModulePart*>( this ); break;
	default:
		raiseUnexpectedInterfaceIndex();
	}

	return res;
}

void ModulePartBase::setReceptacle( IInterfaceInfo* receptacle, Interface* )
{
	checkValidReceptacle( receptacle );
	raiseUnexpectedInterfaceIndex();
}

void ModulePartBase::initialize( co::IModule* )
{
	// empty
}

void ModulePartBase::integrate( co::IModule* )
{
	// empty
}

void ModulePartBase::integratePresentation( co::IModule* )
{
	// empty
}

void ModulePartBase::disintegrate( co::IModule* )
{
	// empty
}

void ModulePartBase::dispose( co::IModule* )
{
	// empty
}

} // namespace co
