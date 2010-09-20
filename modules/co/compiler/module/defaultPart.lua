local function template( writer, c )
	c.header( writer, c, "Default co::ModulePart component generated for module '", c.moduleName, "'" )

	writer( [[

#include "ModuleInstaller.h"
#include <co/Module.h>
#include <co/reserved/ModulePartBase.h>

]] )

	c.revisionCheck( writer, c )

	c.utils.openNamespaces( writer, c.moduleName )

	writer( [[
	void moduleRetain();
	void moduleRelease();
]] )

	c.utils.closeNamespaces( writer, c.moduleName )

	writer( [[

class __ModulePart : public co::ModulePartBase
{
public:
	__ModulePart()
	{
		]], c.moduleNS, [[::moduleRetain();
	}

	virtual ~__ModulePart()
	{
		]], c.moduleNS, [[::moduleRelease();
	}

	void initialize( co::Module* )
	{
		]], c.moduleNS, [[::ModuleInstaller::instance().install();
	}

	void dispose( co::Module* )
	{
		]], c.moduleNS, [[::ModuleInstaller::instance().uninstall();
	}
};

CORAL_EXPORT_MODULE_PART( __ModulePart )
]] )

end

return template
