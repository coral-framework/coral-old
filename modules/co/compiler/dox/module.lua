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

	\weakgroup coral_types Coral Types
	\brief This is a list of Coral types, grouped into \ref co::TypeKind "TypeKinds".
 */
//@{
/*!
]] )

	if countTypeKinds.TK_INTERFACE > 0 then
		writer [[
	\weakgroup coral_interfaces Coral Interfaces
	\brief List of Coral Interfaces.
]]
	end

	if c.moduleName ~= "co" and countTypeKinds.TK_COMPONENT > 0 then
		writer [[
	\weakgroup coral_components Coral Components
	\brief List of Coral Components.
]]
	end

	if countTypeKinds.TK_NATIVECLASS > 0 then
		writer [[
	\weakgroup coral_nativeclasses Coral Native Classes
	\brief List of Coral Native Classes.
]]
	end

	if countTypeKinds.TK_STRUCT > 0 then
		writer [[
	\weakgroup coral_structs Coral Structs
	\brief List of Coral Structs.
]]
	end

	if countTypeKinds.TK_ENUM > 0 then
		writer [[
	\weakgroup coral_enums Coral Enums
	\brief List of Coral Enums.
]]
	end

	if countTypeKinds.TK_EXCEPTION > 0 then
		writer [[
	\weakgroup coral_exceptions Coral Exceptions
	\brief List of Coral Exceptions.
]]
	end

	writer( [[
 */
//@}
]] )
end

return template
