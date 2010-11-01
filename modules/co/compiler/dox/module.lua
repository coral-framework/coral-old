local utils = require "co.compiler.utils"

local function template( writer, c )
	local countTypeKinds = {
		TK_EXCEPTION = 0,
		TK_ENUM = 0,
		TK_STRUCT = 0,
		TK_NATIVECLASS = 0,
		TK_INTERFACE = 0,
		TK_COMPONENT = 0,
	}

	for i, t in ipairs( c.types ) do
		local count = countTypeKinds[t.kind]
		if count then
			countTypeKinds[t.kind] = count + 1
		end
	end

	writer( [[
/*!
	\namespace ]], utils.toCppName( c.moduleName ), '\n', [[
	CSL namespace '<tt>]], c.moduleName, [[</tt>'.

	\weakgroup coral_type List of Coral Types
	\brief List of all Coral Types in this documentation, grouped by \ref co::TypeKind "TypeKind".
 */
//@{
/*!
]] )

	if countTypeKinds.TK_INTERFACE > 0 then
		writer [[
	\weakgroup coral_interface Coral Interface
	\brief List of all Coral Interfaces in this documentation.
]]
	end

	if c.moduleName ~= "co" and countTypeKinds.TK_COMPONENT > 0 then
		writer [[
	\weakgroup coral_component Coral Component
	\brief List of all Coral Components in this documentation.
]]
	end

	if countTypeKinds.TK_NATIVECLASS > 0 then
		writer [[
	\weakgroup coral_nativeclass Coral Native Class
	\brief List of all Coral Native Classes in this documentation.
]]
	end

	if countTypeKinds.TK_STRUCT > 0 then
		writer [[
	\weakgroup coral_struct Coral Struct
	\brief List of all Coral Structs in this documentation.
]]
	end

	if countTypeKinds.TK_ENUM > 0 then
		writer [[
	\weakgroup coral_enum Coral Enum
	\brief List of all Coral Enums in this documentation.
]]
	end

	if countTypeKinds.TK_EXCEPTION > 0 then
		writer [[
	\weakgroup coral_exception Coral Exception
	\brief List of all Coral Exceptions in this documentation.
]]
	end

	writer( [[
 */
//@}
]] )
end

return template
