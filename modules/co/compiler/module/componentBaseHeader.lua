local function template( writer, c, t )
	c.header( writer, c, "Base class generated for component '", t.fullName, "'" )

	writer( [[

#ifndef _]], t.fullNameUpperUnderline, [[_BASE_H_
#define _]], t.fullNameUpperUnderline, [[_BASE_H_

]] )

	for header in pairs( t.includedHeaders ) do
		writer( "#include <", header, ">\n" )
	end

	for fullName, type in pairs( t.forwardDeclTypes ) do
		writer( "#include <", type.headerName, ">\n" )
	end

	writer( "#include <co/reserved/ComponentBase.h>\n\n" )

	c.utils.openNamespaces( writer, c.moduleName )

	writer( "\n" )

	local providedItfs = t.providedInterfaces
	local requiredItfs = t.requiredInterfaces

	for i, itf in ipairs( providedItfs ) do
		writer( [[
//! ]], t.fullName, [[ provides an interface named ']], itf.name, [[', of type ]], itf.type.fullName, ".\n", [[
class ]], t.name , [[_]], itf.type.fullNameUnderline, [[ : public ]], itf.type.cppName, "\n", [[
{
public:
	virtual co::InterfaceType* getInterfaceType();
	virtual const std::string& getInterfaceName();
};

]] )
	end

	writer( [[
/*!
	Inherit from this class to implement the component ']], t.fullName, [['.
 */
class ]], t.name, [[_Base : public co::ComponentBase]] )

	if #providedItfs > 0 then
		for i, itf in ipairs( providedItfs ) do
			writer( ",\n\tpublic ", t.name, "_", itf.type.fullNameUnderline )
		end
	end

	writer( [[

{
public:
	]], t.name, [[_Base();
	virtual ~]], t.name, [[_Base();

	// co::Interface Methods:
	co::Component* getInterfaceOwner();
	void componentRetain();
	void componentRelease();

	// co::Component Methods:
	co::ComponentType* getComponentType();
	co::Interface* getInterface( co::InterfaceInfo* );
	void bindInterface( co::InterfaceInfo*, co::Interface* );
]] )

	if #requiredItfs > 0 then
		writer( "\nprotected:" )

		for i, itf in ipairs( requiredItfs ) do
			writer( [[

	//! Gets required interface ']], itf.name, [[', of type ]], itf.type.fullName, ".\n", [[
	virtual ]], itf.type.cppName, [[* ]], t.formatAccessor( "getRequired", itf.name ), [[() = 0;

	//! Sets required interface ']], itf.name, [[', of type ]], itf.type.fullName, ".\n", [[
	virtual void ]], t.formatAccessor( "setRequired", itf.name ), [[( ]], itf.type.cppName, [[* ]], itf.name, [[ ) = 0;
]] )
		end
	end

	writer( "};\n\n" )

	c.utils.closeNamespaces( writer, c.moduleName )

	writer( [[

#endif // _]], t.fullNameUpperUnderline, [[_BASE_H_
]] )

end

return template
