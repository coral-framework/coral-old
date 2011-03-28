local function template( writer, c, t )
	c.header( writer, c, "Base class generated for component '", t.fullName, "'" )

	writer( [[

#include "]], t.name, [[_Base.h"
#include <co/Coral.h>
#include <co/IComponent.h>
#include <co/IPort.h>
#include <co/IInterface.h>

]] )

	c.revisionCheck( writer, c, t )

	c.utils.openNamespaces( writer, c.moduleName )

	if c.moduleName ~= 'co' then
		writer( [[

void moduleRetain();
void moduleRelease();
]] )
	end

	local facets = t.facets
	local receptacles = t.receptacles

	for i, itf in ipairs( facets ) do
		writer( [[

// ------ ]], t.fullName, [[ provides an interface named ']], itf.name, [[', of type ]], itf.type.fullName, [[ ------ //

co::IInterface* ]], t.name, [[_]], itf.type.fullNameUnderline, [[::getInterfaceType()
{
	return co::typeOf<]], itf.type.cppName, [[>::get();
}

const std::string& ]], t.name, [[_]], itf.type.fullNameUnderline, [[::getInterfaceName()
{
	static const std::string s_interfaceName( "]], itf.name, [[" );
	return s_interfaceName;
}
]] )
	end

	writer( [[

// ------ ]], t.name, [[_Base ------ //

]], t.name, [[_Base::]], t.name, [[_Base()
{
]] )

	if c.moduleName == 'co' then
		writer( "\t// empty\n" )
	else
		writer( "\tmoduleRetain();\n" )
	end

	writer( [[
}

]], t.name, [[_Base::~]], t.name, [[_Base()
{
]] )

	if c.moduleName == 'co' then
		writer( "\t// empty\n" )
	else
		writer( "\tmoduleRelease();\n" )
	end

	writer( [[
}

co::IObject* ]], t.name, [[_Base::getInterfaceOwner()
{
	return this;
}

void ]], t.name, [[_Base::componentRetain()
{
	incrementRefCount();
}

void ]], t.name, [[_Base::componentRelease()
{
	decrementRefCount();
}

co::IComponent* ]], t.name, [[_Base::getComponentType()
{
	co::IType* type = co::getType( "]], t.fullName, [[" );
	assert( dynamic_cast<co::IComponent*>( type ) );
	return static_cast<co::IComponent*>( type );
}

co::IService* ]], t.name, [[_Base::getInterface( co::IPort* port )
{
	checkValidPort( port );
	co::IService* res = NULL;
	switch( port->getIndex() )
	{
]] )

	for i, itf in ipairs( facets ) do
		writer( "\tcase ", itf.index, ":\t\tres = co::disambiguate<co::IService, ", itf.type.cppName, ">( this ); break;\n" )
	end

	for i, itf in ipairs( receptacles ) do
		writer( "\tcase ", itf.index, ":\t\tres = co::disambiguate<co::IService, ",
			itf.type.cppName, ">( ", t.formatAccessor( "getReceptacle", itf.name ), "() ); break;\n" )
	end

	writer( [[
	default:	raiseUnexpectedPortIndex();
	}
	return res;
}

void ]], t.name, [[_Base::setReceptacle( co::IPort* receptacle, co::IService* service )
{
	checkValidReceptacle( receptacle );
]] )

	if #receptacles > 0 then
		writer( [[
	switch( receptacle->getIndex() )
	{
]] )

		for i, itf in ipairs( receptacles ) do
			writer( "\tcase ", itf.index, ":\t\t", t.formatAccessor( "setReceptacle", itf.name ),
				"( checkedCast<", itf.type.cppName, ">( service ) ); break;\n" )
		end

		writer( [[
	default:	raiseUnexpectedPortIndex();
	}
]] )
	else
		writer( [[
	raiseUnexpectedPortIndex();
	CORAL_UNUSED( service );
]] )
	end

	writer( "}\n\n" )

	c.utils.closeNamespaces( writer, c.moduleName )

end

return template
