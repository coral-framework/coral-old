local function template( writer, c, t )
	c.header( writer, c, "ModuleInstaller class generated for module '", c.moduleName, "'" )

	writer( [[

#include <co/RefPtr.h>
#include <co/Reflector.h>
#include <co/ModulePart.h>

]] )

	c.utils.openNamespaces( writer, c.moduleName )

	writer( [[

class ModuleInstaller
{
public:
	inline static ModuleInstaller& instance() { return sm_instance; }

	enum TypeId
	{
]] )

	for i, t in ipairs( c.types ) do
		writer( "\t\tTypeId_", t.name, ",\n" )
	end

	writer( [[
		TypeCount
	};

public:
	//! Instantiates Reflectors for the module types that still don't have an assigned Reflector.
	void initialize();

	//! Returns the Reflector that should be assigned to a module type.
	inline co::Reflector* getReflector( TypeId typeId ) { return _reflectors[typeId].get(); }

	//! Sets the Reflector instance that should be assigned to module type.
	//! \warning This method can only be called \b before calling install().
	inline void setReflector( TypeId typeId, co::Reflector* reflector ) { _reflectors[typeId] = reflector; }

	//! Assigns the Reflectors to their corresponding co::Type instances.
	//! This method skips any co::Type instance that already has an assigned Reflector.
	void install();

	//! Removes references to the Reflectors from their corresponding co::Type instances.
	//! This ignores any co::Type instance whose current Reflector was not assigned by this class.
	void uninstall();

private:
	ModuleInstaller();
	~ModuleInstaller();

	co::Reflector* createReflector( TypeId typeId );

private:
	static ModuleInstaller sm_instance;

private:
	co::RefPtr<co::Reflector> _reflectors[TypeCount];
};

// Module's reflector creation functions:
]] )

	for i, t in ipairs( c.types ) do
		writer( "co::Reflector* __create", t.name, "Reflector();\n" )
	end

	writer( "\n" )

	c.utils.closeNamespaces( writer, c.moduleName )

end

return template
