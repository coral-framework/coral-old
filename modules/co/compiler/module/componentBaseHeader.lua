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

	local facets = t.facets
	local receptacles = t.receptacles

	for i, itf in ipairs( facets ) do
		writer( [[
//! ]], t.fullName, [[ has a facet named ']], itf.name, [[', of type ]], itf.type.fullName, ".\n", [[
class ]], t.name , [[_]], itf.type.fullNameUnderline, [[ : public ]], itf.type.cppName, "\n", [[
{
public:
	virtual co::IInterfaceType* getInterfaceType();
	virtual const std::string& getInterfaceName();
};

]] )
	end

	writer( [[
/*!
	Inherit from this class to implement the component ']], t.fullName, [['.
 */
class ]], t.name, [[_Base : public co::ComponentBase]] )

	if #facets > 0 then
		for i, itf in ipairs( facets ) do
			writer( ",\n\tpublic ", t.name, "_", itf.type.fullNameUnderline )
		end
	end

	writer( [[

{
public:
	]], t.name, [[_Base();
	virtual ~]], t.name, [[_Base();

	// co::Interface Methods:
	co::IComponent* getInterfaceOwner();
	void componentRetain();
	void componentRelease();

	// co::IComponent Methods:
	co::IComponentType* getComponentType();
	co::Interface* getInterface( co::IInterfaceInfo* );
	void setReceptacle( co::IInterfaceInfo*, co::Interface* );
]] )

	if #receptacles > 0 then
		writer( "\nprotected:" )

		for i, itf in ipairs( receptacles ) do
			writer( [[

	//! Get receptacle ']], itf.name, [[', of type ]], itf.type.fullName, ".\n", [[
	virtual ]], itf.type.cppName, [[* ]], t.formatAccessor( "getReceptacle", itf.name ), [[() = 0;

	//! Set receptacle ']], itf.name, [[', of type ]], itf.type.fullName, ".\n", [[
	virtual void ]], t.formatAccessor( "setReceptacle", itf.name ), [[( ]], itf.type.cppName, [[* ]], itf.name, [[ ) = 0;
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
