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
#include <co/reserved/LibraryManager.h>

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

{{#NAMESPACE_LIST}}
namespace {{NAMESPACE}} {
{{/NAMESPACE_LIST}}

{{NEWLINE}}

// The module's ModulePart instance
co::RefPtr<co::ModulePart> sg_instance;

{{NEWLINE}}

// The module's internal reference count
co::int32 sg_refCount( 0 );

{{NEWLINE}}

void moduleRetain()
{
	++sg_refCount;
}

{{NEWLINE}}

void moduleRelease()
{
	// is the module's ModulePart the only active reference?
	if( --sg_refCount == 1 )
	{
		assert( sg_instance.isValid() );
		co::LibraryManager::release( sg_instance.get() );
	}
}

{{NEWLINE}}

{{#NAMESPACE_LIST}}
} // namespace {{NAMESPACE}}
{{/NAMESPACE_LIST}}

{{NEWLINE}}

// __createModulePart() is implemented by CORAL_EXPORT_MODULE_PART()
co::ModulePart* __createModulePart();

{{NEWLINE}}

extern "C" CORAL_DLL_EXPORT
co::ModulePart* coral_module_part_instance()
{
	if( !{{NS}}::sg_instance.isValid() )
	{
		assert( {{NS}}::sg_refCount == 0 );
		{{NS}}::sg_instance = __createModulePart();
	}
	return {{NS}}::sg_instance.get();
}
