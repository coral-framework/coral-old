/*******************************************************************************
** Adapter generated for native class '{{TYPE_NAME_FULL}}'
**
** Created: {{CURRENT_DATE_TIME}}
**      by: Coral Compiler version {{CORAL_VERSION}}
**
** WARNING! All changes made in this file will be lost when recompiling!
********************************************************************************/

{{NEWLINE}}

#ifndef _{{TYPE_NAME_FULL_UPPERSCORE}}_ADAPTER_H_
#define _{{TYPE_NAME_FULL_UPPERSCORE}}_ADAPTER_H_

{{NEWLINE}}

#include <{{TYPE_FILE_PATH}}.h>

{{NEWLINE}}

{{#NAMESPACE_LIST}}
namespace {{NAMESPACE}} {
{{/NAMESPACE_LIST}}

{{NEWLINE}}

/*!
	Implement this class to adapt '{{TYPE_NAME_CPP}}' to the Coral type system.
 */
class {{TYPE_NAME}}_Adapter
{
public:
{{#MY_LOCAL}}
{{#HAS_ATTRIBUTES_TRUE}}
	// ------ Attributes ------
{{#ATTRIBUTE_LIST}}
{{#GETTER}}
{{NEWLINE}}
	static {{RETURN_TYPE}} {{METHOD_NAME}}( const {{TYPE_NAME_CPP}}& instance );
{{/GETTER}}
{{#SETTER}}
	static void {{METHOD_NAME}}( {{TYPE_NAME_CPP}}& instance, {{PARAMETER_TYPE}} value );
{{/SETTER}}
{{/ATTRIBUTE_LIST}}
{{/HAS_ATTRIBUTES_TRUE}}

{{#HAS_METHODS_TRUE}}
{{NEWLINE}}
	// ------ Methods ------
{{#METHOD_LIST}}
{{NEWLINE}}
	static {{RETURN_TYPE}} {{METHOD_NAME}}( {{TYPE_NAME_CPP}}& instance{{#PARAMETER_LIST}}, {{PARAMETER_TYPE}} {{PARAMETER_NAME}}{{/PARAMETER_LIST}} );
{{/METHOD_LIST}}
{{/HAS_METHODS_TRUE}}
{{/MY_LOCAL}}
};

{{NEWLINE}}

{{#NAMESPACE_LIST}}
} // namespace {{NAMESPACE}}
{{/NAMESPACE_LIST}}

{{NEWLINE}}

#endif // _{{TYPE_NAME_FULL_UPPERSCORE}}_ADAPTER_H_
