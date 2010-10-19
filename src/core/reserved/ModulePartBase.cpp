/*
 * Coral - A C++ Component Framework.
 * See Copyright Notice in Coral.h
 */

#include "ModulePartBase.h"
#include <co/Coral.h>
#include <co/Module.h>
#include <co/RefPtr.h>
#include <co/Exception.h>
#include <co/Namespace.h>
#include <co/TypeBuilder.h>
#include <co/InterfaceInfo.h>
#include <co/InterfaceType.h>
#include <co/CoreException.h>
#include <co/TypeCreationTransaction.h>

namespace co {

// ------ ModulePartBase provides an interface named 'part', of type co::ModulePart ------ //

InterfaceType* ModulePartBase_co_ModulePart::getInterfaceType()
{
	return co::typeOf<ModulePart>::get();
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

Component* ModulePartBase::getInterfaceOwner()
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

ComponentType* ModulePartBase::getComponentType()
{
	return getOrCreateSimpleInternalComponentType( "co.ModulePartBase", "co.ModulePart", "part" );
}

Interface* ModulePartBase::getInterface( InterfaceInfo* interfaceInfo )
{
	checkValidInterface( interfaceInfo );

	Interface* res = NULL;
	switch( interfaceInfo->getIndex() )
	{
	case 0: res = static_cast<ModulePart*>( this ); break;
	default:
		raiseUnexpectedInterfaceIndex();
	}

	return res;
}

void ModulePartBase::bindInterface( InterfaceInfo* receptacle, Interface* )
{
	checkValidReceptacle( receptacle );
	raiseUnexpectedInterfaceIndex();
}

void ModulePartBase::initialize( co::Module* )
{
	// empty
}

void ModulePartBase::integrate( co::Module* )
{
	// empty
}

void ModulePartBase::integratePresentation( co::Module* )
{
	// empty
}

void ModulePartBase::disintegrate( co::Module* )
{
	// empty
}

void ModulePartBase::dispose( co::Module* )
{
	// empty
}

} // namespace co
