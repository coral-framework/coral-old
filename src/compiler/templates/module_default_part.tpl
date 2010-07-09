/*******************************************************************************
** Default co::ModulePart component generated for module '{{MODULE_NAME}}'
**
** Created: {{CURRENT_DATE_TIME}}
**      by: Coral Compiler version {{CORAL_VERSION}}
**
** WARNING! All changes made in this file will be lost when recompiling!
********************************************************************************/

{{NEWLINE}}

#include "ModuleInstaller.h"
#include <co/Module.h>
#include <co/reserved/ModulePartBase.h>

{{NEWLINE}}

{{#MODULE_IS_CO_FALSE}}
{{>REVISION_CHECK}}
{{NEWLINE}}
{{/MODULE_IS_CO_FALSE}}

{{#NAMESPACE_LIST}}
namespace {{NAMESPACE}} {
{{/NAMESPACE_LIST}}
	void moduleRetain();
	void moduleRelease();
{{#NAMESPACE_LIST}}
} // namespace {{NAMESPACE}}
{{/NAMESPACE_LIST}}

{{NEWLINE}}

class __ModulePart : public co::ModulePartBase
{
public:
	__ModulePart()
	{
		{{NS}}::moduleRetain();
	}

{{NEWLINE}}

	virtual ~__ModulePart()
	{
		{{NS}}::moduleRelease();
	}

{{NEWLINE}}

	void initialize( co::Module* )
	{
		// install the module's Reflectors
		{{NS}}::ModuleInstaller::instance().install();
	}

{{NEWLINE}}

	void dispose( co::Module* )
	{
		{{NS}}::ModuleInstaller::instance().uninstall();
	}
};

{{NEWLINE}}

CORAL_EXPORT_MODULE_PART( __ModulePart )
