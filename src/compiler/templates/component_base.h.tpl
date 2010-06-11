/*******************************************************************************
** Base class generated for component '{{TYPE_NAME_FULL}}'
**
** Created: {{CURRENT_DATE_TIME}}
**      by: Coral Compiler version {{CORAL_VERSION}}
**
** WARNING! All changes made in this file will be lost when recompiling!
********************************************************************************/

{{NEWLINE}}

#ifndef _{{TYPE_NAME_FULL_UPPERSCORE}}_BASE_H_
#define _{{TYPE_NAME_FULL_UPPERSCORE}}_BASE_H_

{{NEWLINE}}

{{#HAS_CLIENT_INTERFACES_TRUE}}
#include <co/RefPtr.h>
{{/HAS_CLIENT_INTERFACES_TRUE}}
#include <co/reserved/ComponentBase.h>
{{#HEADER_LIST}}
#include <{{HEADER}}>
{{/HEADER_LIST}}
{{#FW_DECL_LIST}}
#include <{{HEADER}}>
{{/FW_DECL_LIST}}

{{NEWLINE}}

{{#NAMESPACE_LIST}}
namespace {{NAMESPACE}} {
{{/NAMESPACE_LIST}}

{{NEWLINE}}

{{#SERVER_INTERFACE_LIST}}
//! {{TYPE_NAME_FULL}} provides an interface named '{{NAME}}', of type {{ITF_TYPE_FULL}}
class {{TYPE_NAME}}_{{ITF_TYPE_FULL_UNDERSCORES}} : public {{ITF_TYPE_CPP}}
{
public:
	virtual co::InterfaceType* getInterfaceType();
	virtual const std::string& getInterfaceName();
};

{{NEWLINE}}
{{/SERVER_INTERFACE_LIST}}

/*!
	Inherit from this class to implement the component '{{TYPE_NAME_FULL}}'.
 */
class {{TYPE_NAME}}_Base : public co::ComponentBase{{#HAS_SERVER_INTERFACES_TRUE}},{{/HAS_SERVER_INTERFACES_TRUE}}
{{#SERVER_INTERFACE_LIST}}
	public {{TYPE_NAME}}_{{ITF_TYPE_FULL_UNDERSCORES}}{{#SERVER_INTERFACE_LIST_separator}},{{/SERVER_INTERFACE_LIST_separator}}
{{/SERVER_INTERFACE_LIST}}
{
public:
	{{TYPE_NAME}}_Base();
	virtual ~{{TYPE_NAME}}_Base();

{{NEWLINE}}

	// co::Interface Methods:
	virtual co::Component* getInterfaceOwner();
	virtual void componentRetain();
	virtual void componentRelease();

{{NEWLINE}}

	// co::Component Methods:
	virtual co::ComponentType* getComponentType();
	virtual co::Interface* getInterface( co::InterfaceInfo* );
	virtual void bindInterface( co::InterfaceInfo*, co::Interface* );

{{#HAS_CLIENT_INTERFACES_TRUE}}
{{NEWLINE}}
protected:
{{/HAS_CLIENT_INTERFACES_TRUE}}
{{#CLIENT_INTERFACE_LIST}}
	//! Required interface '{{NAME}}', of type {{ITF_TYPE_FULL}}
	inline {{ITF_TYPE_CPP}}* get{{NAME_CAPITALIZED}}() { return _{{NAME}}.get(); }
	inline void set{{NAME_CAPITALIZED}}( {{ITF_TYPE_CPP}}* {{NAME}} ) { _{{NAME}} = {{NAME}}; }
{{NEWLINE}}
{{/CLIENT_INTERFACE_LIST}}

{{#HAS_CLIENT_INTERFACES_TRUE}}
private:
{{/HAS_CLIENT_INTERFACES_TRUE}}
{{#CLIENT_INTERFACE_LIST}}
	co::RefPtr<{{ITF_TYPE_CPP}}> _{{NAME}};
{{/CLIENT_INTERFACE_LIST}}
};

{{NEWLINE}}

{{#NAMESPACE_LIST}}
} // namespace {{NAMESPACE}}
{{/NAMESPACE_LIST}}

{{NEWLINE}}

#endif // _{{TYPE_NAME_FULL_UPPERSCORE}}_BASE_H_
