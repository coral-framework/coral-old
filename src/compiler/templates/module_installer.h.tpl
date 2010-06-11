/*******************************************************************************
** ModuleInstaller class generated for module '{{MODULE_NAME}}'
**
** Created: {{CURRENT_DATE_TIME}}
**      by: Coral Compiler version {{CORAL_VERSION}}
**
** WARNING! All changes made in this file will be lost when recompiling!
********************************************************************************/

{{NEWLINE}}

#include <co/RefPtr.h>
#include <co/Reflector.h>
#include <co/ModulePart.h>

{{NEWLINE}}

{{#NAMESPACE_LIST}}
namespace {{NAMESPACE}} {
{{/NAMESPACE_LIST}}

{{NEWLINE}}

class ModuleInstaller
{
public:
	inline static ModuleInstaller& instance() { return sm_instance; }

{{NEWLINE}}

	enum TypeId
	{
{{#MODULE_TYPE_LIST}}
		TypeId_{{TYPE_NAME}},
{{/MODULE_TYPE_LIST}}
		TypeCount
	};

{{NEWLINE}}

public:
	//! Instantiates Reflectors for the module types that still don't have an assigned Reflector.
	void initialize();

{{NEWLINE}}

	//! Returns the Reflector that should be assigned to a module type.
	inline co::Reflector* getReflector( TypeId typeId ) { return _reflectors[typeId].get(); }

{{NEWLINE}}

	//! Sets the Reflector instance that should be assigned to module type.
	//! \warning This method can only be called \b before calling install().
	inline void setReflector( TypeId typeId, co::Reflector* reflector ) { _reflectors[typeId] = reflector; }

{{NEWLINE}}

	//! Assigns the Reflectors to their corresponding co::Type instances.
	//! This method skips any co::Type instance that already has an assigned Reflector.
	void install();

{{NEWLINE}}

	//! Removes references to the Reflectors from their corresponding co::Type instances.
	//! This ignores any co::Type instance whose current Reflector was not assigned by this class.
	void uninstall();

{{NEWLINE}}

private:
	ModuleInstaller();
	~ModuleInstaller();

{{NEWLINE}}

	co::Reflector* createReflector( TypeId typeId );

{{NEWLINE}}

private:
	static ModuleInstaller sm_instance;

{{NEWLINE}}

private:
	co::RefPtr<co::Reflector> _reflectors[TypeCount];
};

{{NEWLINE}}

{{#NAMESPACE_LIST}}
} // namespace {{NAMESPACE}}
{{/NAMESPACE_LIST}}
