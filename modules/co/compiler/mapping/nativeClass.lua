local function template( writer, c, t )
	writer( [[
	// This is a native class mapping.
	// You may use type ']], t.nativeName, [[' directly, or through the typedef below.
	typedef ]], t.nativeName, " ", t.name, ";\n" )
end

return template
