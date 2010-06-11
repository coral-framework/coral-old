/*******************************************************************************
** ModuleInstaller class generated for module '{{MODULE_NAME}}'
**
** Created: {{CURRENT_DATE_TIME}}
**      by: Coral Compiler version {{CORAL_VERSION}}
**
** WARNING! All changes made in this file will be lost when recompiling!
********************************************************************************/

{{NEWLINE}}

#include "ModuleInstaller.h"
#include <co/Type.h>

{{NEWLINE}}

{{#MODULE_IS_CO_FALSE}}
{{>REVISION_CHECK}}
{{NEWLINE}}
{{/MODULE_IS_CO_FALSE}}

{{NS}}::ModuleInstaller {{NS}}::ModuleInstaller::sm_instance;

{{NEWLINE}}

{{NS}}::ModuleInstaller::ModuleInstaller()
{
	// empty
}

{{NEWLINE}}

{{NS}}::ModuleInstaller::~ModuleInstaller()
{
	// empty
}

{{NEWLINE}}

void {{NS}}::ModuleInstaller::initialize()
{
	for( unsigned int i = 0; i < TypeCount; ++i )
	{
		if( !_reflectors[i].isValid() )
			_reflectors[i] = createReflector( static_cast<TypeId>( i ) );
	}
}

{{NEWLINE}}

void {{NS}}::ModuleInstaller::install()
{
	initialize();
{{NEWLINE}}
	for( unsigned int i = 0; i < TypeCount; ++i )
	{
		co::Reflector* reflector = _reflectors[i].get();
		reflector->getType()->setReflector( reflector );
	}
}

{{NEWLINE}}

void {{NS}}::ModuleInstaller::uninstall()
{
	for( unsigned int i = 0; i < TypeCount; ++i )
	{
		_reflectors[i]->getType()->setReflector( NULL );
		_reflectors[i] = NULL;
	}
}

{{NEWLINE}}

// declare all reflector creation functions:
namespace __coral {
{{#MODULE_TYPE_LIST}}
	co::Reflector* create{{TYPE_NAME}}Reflector();
{{/MODULE_TYPE_LIST}}
} // namespace __coral

{{NEWLINE}}

co::Reflector* {{NS}}::ModuleInstaller::createReflector( TypeId typeId )
{
	co::Reflector* res = NULL;
	switch( typeId )
	{
{{#MODULE_TYPE_LIST}}
	case TypeId_{{TYPE_NAME}}: res = __coral::create{{TYPE_NAME}}Reflector(); break;
{{/MODULE_TYPE_LIST}}
	default: assert( false );
	};
	assert( res != NULL );
	return res;
}
