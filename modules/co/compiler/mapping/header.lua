local function template( writer, c, t )
	c.header( writer, c, "C++ mapping generated for type '", t.fullName, "'" )

	writer( [[

#ifndef _]], t.fullNameUpperUnderline, [[_H_
#define _]], t.fullNameUpperUnderline, [[_H_

#include <co/TypeTraits.h>
]] )

	for header in pairs( t.includedHeaders ) do
		writer( "#include <", header, ">\n" )
	end

	if next( t.forwardDeclTypes ) then
		writer( "\n// Forward Declarations:\n" )

		-- gather all type names into an array and sort them
		local fullTypeNames = {}
		for fullName, type in pairs( t.forwardDeclTypes ) do
			fullTypeNames[#fullTypeNames + 1] = fullName
		end

		table.sort( fullTypeNames )

		local depth = 0
		local lastNS = ''
		local indent = ''
		local s, e

		-- closes namespaces from 'lastNS' until it contains 'ns'
		local function closeNamespacesUntilItContains( ns )
			while true do
				s, e = ns:find( lastNS, 1, true )
				if s == 1 then break end
				local prefix, name = lastNS:match( '(.+)%.([^%.]+)$' )
				if prefix then
					lastNS = prefix
				else
					name = lastNS
					lastNS = ''
				end
				depth = depth - 1
				indent = ( "\t" ):rep( depth )
				writer( indent, "} // namespace ", name, "\n" )
			end
		end

		for _, fullName in ipairs( fullTypeNames ) do
			local type = t.forwardDeclTypes[fullName]
			local ns = type.namespace.fullName
			if ns ~= lastNS then
				closeNamespacesUntilItContains( ns )

				-- ns is within lastNS, just open the necessary namespaces
				local namespacesToOpen = ns:sub( e + 1 )
				for name in namespacesToOpen:gmatch( '([^%.]+)' ) do
					writer( indent, "namespace ", name, " {\n" )
					depth = depth + 1
					indent = ( '\t' ):rep( depth )
				end
				lastNS = ns
			end
			writer( indent, type.kind == 'TK_STRUCT' and "struct " or "class ", type.name, ";\n" )
		end

		closeNamespacesUntilItContains( '' )

		writer( "// End Of Forward Declarations\n" )
	end

	writer( "\n// ", t.fullName, " Mapping:\n" )

	c.utils.openNamespaces( writer, t.namespace.fullName )

	writer( "\n" )
end

return template
