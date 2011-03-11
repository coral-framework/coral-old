/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#include "moduleB_Base.h"
#include "ModuleInstaller.h"
#include <co/System.h>
#include <co/ModuleManager.h>

namespace moduleB {

class ModulePart : public moduleB_Base
{
public:
    ModulePart()
	{
		// empty
	}

	virtual ~ModulePart()
	{
		// empty
	}

	// co::ModulePart methods:

	void initialize( co::Module* )
	{
		moduleB::ModuleInstaller::instance().install();
		
		// require moduleA
		co::getSystem()->getModules()->load( "moduleA" );
	}

	void integrate( co::Module* )
	{
		// empty
	}

	void integratePresentation( co::Module* )
	{
		// empty
	}

	void disintegrate( co::Module* )
	{
		// empty
	}

	void dispose( co::Module* )
	{
		moduleB::ModuleInstaller::instance().uninstall();
	}
};

CORAL_EXPORT_COMPONENT( ModulePart, moduleB );
CORAL_EXPORT_MODULE_PART( ModulePart );

} // namespace moduleB
