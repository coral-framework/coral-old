/*******************************************************************************
** Bootstrap functions generated for module '{{MODULE_NAME}}'
**
** Created: {{CURRENT_DATE_TIME}}
**      by: Coral Compiler version {{CORAL_VERSION}}
**
** WARNING! All changes made in this file will be lost when recompiling!
********************************************************************************/

{{NEWLINE}}

#include <co/RefPtr.h>
#include <co/ModulePart.h>

{{NEWLINE}}

{{#MODULE_IS_CO_FALSE}}
{{>REVISION_CHECK}}
{{NEWLINE}}
{{/MODULE_IS_CO_FALSE}}

extern "C" CORAL_DLL_EXPORT
const char* coral_module_query_verification_data()
{
	return  "pattern=CORAL_MODULE_VERIFICATION_DATA\n"
			"version={{CORAL_VERSION}}\n"
			"buildkey=" CORAL_BUILD_KEY "\n"
			"buildmode=" CORAL_BUILD_MODE "\n";
}

{{NEWLINE}}

struct TypeDependency
{
	const char* name;
	const char* binarySignature;
};

{{NEWLINE}}

extern "C" CORAL_DLL_EXPORT
const TypeDependency* coral_module_query_dependencies()
{
	static const TypeDependency s_dependencies[] = {
{{#MODULE_DEPENDENCIES_LIST}}
		{ "{{TYPE_NAME_FULL}}", "{{TYPE_BINARY_SIGNATURE}}" },
{{/MODULE_DEPENDENCIES_LIST}}
		{ NULL, NULL }
	};
	return s_dependencies;
}

{{NEWLINE}}

// __createModulePart() is implemented by CORAL_EXPORT_MODULE_PART()
co::ModulePart* __createModulePart();

{{NEWLINE}}

extern "C" CORAL_DLL_EXPORT
co::ModulePart* coral_module_part_instance()
{
	static co::RefPtr<co::ModulePart> s_instance;
	if( !s_instance.isValid() )
		s_instance = __createModulePart();
	return s_instance.get();
}
