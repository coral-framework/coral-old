/*
 * Coral - A C++ Component Framework.
 * See Copyright Notice in Coral.h
 */

#include "ModulePart.h"
#include "ModuleInstaller.h"

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
	lua::ModuleInstaller::instance().install();
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
	lua::ModuleInstaller::instance().uninstall();
}

CORAL_EXPORT_COMPONENT( ModulePart, lua );
CORAL_EXPORT_MODULE_PART( ModulePart );
