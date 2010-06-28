/*******************************************************************************
** Base class generated for component '{{TYPE_NAME_FULL}}'
**
** Created: {{CURRENT_DATE_TIME}}
**      by: Coral Compiler version {{CORAL_VERSION}}
**
** WARNING! All changes made in this file will be lost when recompiling!
********************************************************************************/

{{NEWLINE}}

#include "{{TYPE_NAME}}_Base.h"
#include <co/Coral.h>
#include <co/ComponentType.h>
#include <co/InterfaceInfo.h>
#include <co/InterfaceType.h>

{{NEWLINE}}

{{#MODULE_IS_CO_FALSE}}
{{>REVISION_CHECK}}
{{NEWLINE}}
{{/MODULE_IS_CO_FALSE}}

{{#SERVER_INTERFACE_LIST}}
// ------ {{TYPE_NAME_FULL}} provides an interface named '{{NAME}}', of type {{ITF_TYPE_FULL}} ------ //

{{NEWLINE}}

co::InterfaceType* {{NS}}::{{TYPE_NAME}}_{{ITF_TYPE_FULL_UNDERSCORES}}::getInterfaceType()
{
	return co::typeOf<{{ITF_TYPE_CPP}}>::get();
}

{{NEWLINE}}

const std::string& {{NS}}::{{TYPE_NAME}}_{{ITF_TYPE_FULL_UNDERSCORES}}::getInterfaceName()
{
	static const std::string s_interfaceName( "{{NAME}}" );
	return s_interfaceName;
}

{{NEWLINE}}
{{/SERVER_INTERFACE_LIST}}

// ------ {{TYPE_NAME}}_Base ------ //

{{NEWLINE}}

{{NS}}::{{TYPE_NAME}}_Base::{{TYPE_NAME}}_Base()
{
	// empty
}

{{NEWLINE}}

{{NS}}::{{TYPE_NAME}}_Base::~{{TYPE_NAME}}_Base()
{
	// empty
}

{{NEWLINE}}

co::Component* {{NS}}::{{TYPE_NAME}}_Base::getInterfaceOwner()
{
	return this;
}

{{NEWLINE}}

void {{NS}}::{{TYPE_NAME}}_Base::componentRetain()
{
	incrementRefCount();
}

{{NEWLINE}}

void {{NS}}::{{TYPE_NAME}}_Base::componentRelease()
{
	decrementRefCount();
}

{{NEWLINE}}

co::ComponentType* {{NS}}::{{TYPE_NAME}}_Base::getComponentType()
{
	co::Type* type = co::getType( "{{TYPE_NAME_FULL}}" );
	assert( dynamic_cast<co::ComponentType*>( type ) );
	return static_cast<co::ComponentType*>( type );
}

{{NEWLINE}}

co::Interface* {{NS}}::{{TYPE_NAME}}_Base::getInterface( co::InterfaceInfo* interfaceInfo )
{
	checkValidInterface( interfaceInfo );
	co::Interface* res = NULL;
	switch( interfaceInfo->getIndex() )
	{
{{#CLIENT_INTERFACE_LIST}}
	case {{INDEX}}:		res = co::disambiguate<co::Interface, {{ITF_TYPE_CPP}}>( get{{NAME_CAPITALIZED}}() ); break;
{{/CLIENT_INTERFACE_LIST}}
{{#SERVER_INTERFACE_LIST}}
	case {{INDEX}}:		res = co::disambiguate<co::Interface, {{ITF_TYPE_CPP}}>( this ); break;
{{/SERVER_INTERFACE_LIST}}
	default:	raiseUnexpectedInterfaceIndex();
	}
	return res;
}

{{NEWLINE}}

void {{NS}}::{{TYPE_NAME}}_Base::bindInterface( co::InterfaceInfo* clientInterface, co::Interface* instance )
{
	checkValidClientInterface( clientInterface );
{{#HAS_CLIENT_INTERFACES_TRUE}}
	switch( clientInterface->getIndex() )
	{
{{#CLIENT_INTERFACE_LIST}}
	case {{INDEX}}:		set{{NAME_CAPITALIZED}}( checkedInterfaceCast<{{ITF_TYPE_CPP}}>( instance ) ); break;
{{/CLIENT_INTERFACE_LIST}}
	default:	raiseUnexpectedInterfaceIndex();
	}
{{/HAS_CLIENT_INTERFACES_TRUE}}
{{#HAS_CLIENT_INTERFACES_FALSE}}
	raiseUnexpectedInterfaceIndex();
	CORAL_UNUSED( instance );
{{/HAS_CLIENT_INTERFACES_FALSE}}
}
