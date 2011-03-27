local function template( writer, c, t )
	c.header( writer, c, "ModuleInstaller class generated for module '", c.moduleName, "'" )

	writer( [[

#include <co/RefPtr.h>
#include <co/IReflector.h>
#include <co/IModulePart.h>

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
	//! Instantiates Reflectors for the module types that still don't have an assigned IReflector.
	void initialize();

	//! Returns the IReflector that should be assigned to a module type.
	inline co::IReflector* getReflector( TypeId typeId ) { return _reflectors[typeId].get(); }

	//! Sets the IReflector instance that should be assigned to module type.
	//! \warning This method can only be called \b before calling install().
	inline void setReflector( TypeId typeId, co::IReflector* reflector ) { _reflectors[typeId] = reflector; }

	//! Assigns the reflectors to their corresponding co::IType instances.
	//! This method skips any co::IType instance that already has an assigned IReflector.
	void install();

	//! Removes references to the Reflectors from their corresponding co::IType instances.
	//! This ignores any co::IType instance whose current IReflector was not assigned by this class.
	void uninstall();

private:
	ModuleInstaller();
	~ModuleInstaller();

	co::IReflector* createReflector( TypeId typeId );

private:
	static ModuleInstaller sm_instance;

private:
	co::RefPtr<co::IReflector> _reflectors[TypeCount];
};

// Module's reflector creation functions:
]] )

	for i, t in ipairs( c.types ) do
		writer( "co::IReflector* __create", t.name, "Reflector();\n" )
	end

	writer( "\n" )

	c.utils.closeNamespaces( writer, c.moduleName )

end

return template
