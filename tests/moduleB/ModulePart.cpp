/*
 * Coral - A C++ Component Framework.
 * See Copyright Notice in Coral.h
 */

#include "ModulePart.h"
#include "ModuleInstaller.h"
#include <co/System.h>
#include <co/ModuleManager.h>

ModulePart::ModulePart()
{
	// empty
}

ModulePart::~ModulePart()
{
	// empty
}

void ModulePart::initialize( co::Module* )
{
	moduleB::ModuleInstaller::instance().install();

	// require moduleA
	co::getSystem()->getModules()->load( "moduleA" );
}

void ModulePart::integrate( co::Module* )
{
	// empty
}

void ModulePart::integratePresentation( co::Module* )
{
	// empty
}

void ModulePart::disintegrate( co::Module* )
{
	// empty
}

void ModulePart::dispose( co::Module* )
{
	moduleB::ModuleInstaller::instance().uninstall();
}

CORAL_EXPORT_COMPONENT( ModulePart, moduleB );
CORAL_EXPORT_MODULE_PART( ModulePart );
