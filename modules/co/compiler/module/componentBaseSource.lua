local function template( writer, c, t )
	c.header( writer, c, "Base class generated for component '", t.fullName, "'" )

	writer( [[

#include "]], t.name, [[_Base.h"
#include <co/Coral.h>
#include <co/ComponentType.h>
#include <co/InterfaceInfo.h>
#include <co/InterfaceType.h>

]] )

	c.revisionCheck( writer, c, t )

	c.utils.openNamespaces( writer, c.moduleName )

	if c.moduleName ~= 'co' then
		writer( [[

void moduleRetain();
void moduleRelease();
]] )
	end

	local providedItfs = t.providedInterfaces
	local requiredItfs = t.requiredInterfaces

	for i, itf in ipairs( providedItfs ) do
		writer( [[

// ------ ]], t.fullName, [[ provides an interface named ']], itf.name, [[', of type ]], itf.type.fullName, [[ ------ //

co::InterfaceType* ]], t.name, [[_]], itf.type.fullNameUnderline, [[::getInterfaceType()
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

co::Component* ]], t.name, [[_Base::getInterfaceOwner()
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

co::ComponentType* ]], t.name, [[_Base::getComponentType()
{
	co::Type* type = co::getType( "]], t.fullName, [[" );
	assert( dynamic_cast<co::ComponentType*>( type ) );
	return static_cast<co::ComponentType*>( type );
}

co::Interface* ]], t.name, [[_Base::getInterface( co::InterfaceInfo* interfaceInfo )
{
	checkValidInterface( interfaceInfo );
	co::Interface* res = NULL;
	switch( interfaceInfo->getIndex() )
	{
]] )

	for i, itf in ipairs( requiredItfs ) do
		writer( "\tcase ", itf.index, ":\t\tres = co::disambiguate<co::Interface, ",
			itf.type.cppName, ">( ", t.formatAccessor( "getRequired", itf.name ), "() ); break;\n" )
	end

	for i, itf in ipairs( providedItfs ) do
		writer( "\tcase ", itf.index, ":\t\tres = co::disambiguate<co::Interface, ", itf.type.cppName, ">( this ); break;\n" )
	end

	writer( [[
	default:	raiseUnexpectedInterfaceIndex();
	}
	return res;
}

void ]], t.name, [[_Base::bindInterface( co::InterfaceInfo* clientInterface, co::Interface* instance )
{
	checkValidClientInterface( clientInterface );
]] )

	if #requiredItfs > 0 then
		writer( [[
	switch( clientInterface->getIndex() )
	{
]] )

		for i, itf in ipairs( requiredItfs ) do
			writer( "\tcase ", itf.index, ":\t\t", t.formatAccessor( "setRequired", itf.name ),
				"( checkedInterfaceCast<", itf.type.cppName, ">( instance ) ); break;\n" )
		end

		writer( [[
	default:	raiseUnexpectedInterfaceIndex();
	}
]] )
	else
		writer( [[
	raiseUnexpectedInterfaceIndex();
	CORAL_UNUSED( instance );
]] )
	end

	writer( "}\n\n" )

	c.utils.closeNamespaces( writer, c.moduleName )

end

return template
