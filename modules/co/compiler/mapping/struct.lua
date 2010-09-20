local function estimateFieldSize( t )
	local predefinedSizes = {
		TK_ANY				= co.getType( "any" ).reflector.size,
		TK_BOOLEAN			= co.getType( "bool" ).reflector.size,
		TK_INT8				= co.getType( "int8" ).reflector.size,
		TK_UINT8			= co.getType( "uint8" ).reflector.size,
		TK_INT16			= co.getType( "int16" ).reflector.size,
		TK_UINT16			= co.getType( "uint16" ).reflector.size,
		TK_INT32			= co.getType( "int32" ).reflector.size,
		TK_UINT32			= co.getType( "uint32" ).reflector.size,
		TK_INT64			= co.getType( "int64" ).reflector.size,
		TK_UINT64			= co.getType( "uint64" ).reflector.size,
		TK_FLOAT			= co.getType( "float" ).reflector.size,
		TK_DOUBLE			= co.getType( "double" ).reflector.size,
		TK_STRING			= co.getType( "string" ).reflector.size,
		TK_ARRAY			= co.getType( "string[]" ).reflector.size,
		TK_ENUM				= co.getType( "co.TypeKind" ).reflector.size,
		TK_NATIVECLASS		= 4 * 8, -- average size
		TK_INTERFACE		= ( co.buildKey:find( "x86_64" ) and 8 or 4 ),
	}

	local kind = t.kind
	if kind == 'TK_STRUCT' then
		local res = 0
		for i, a in ipairs( t.memberAttributes ) do
			res = res + estimateFieldSize( a.type )
		end
		return res
	else
		return predefinedSizes[kind]
	end
end

local function template( writer, c, t )
	writer( "struct ", t.name, "\n{\n" )

	local fields = t.memberAttributes
	local fieldSizes = {}
	local sortedIndices = {}
	for i, field in ipairs( fields ) do
		sortedIndices[i] = i
		fieldSizes[i] = estimateFieldSize( field.type )
	end

	-- sort fields by size, descending
	table.sort( sortedIndices, function( a, b )
		return fieldSizes[a] > fieldSizes[b]
	end )

	for _, i in ipairs( sortedIndices ) do
		local field = fields[i]
		writer( "\t", t.formatField( field.type ), " ", field.name, "; // estimated size of ", fieldSizes[i], " bytes\n" )
	end

	writer( "\n\t//! Default constructor.\n\t", t.name, "() :\n" )
	for ii, i in ipairs( sortedIndices ) do
		local field = fields[i]
		writer( "\t\t", field.name, "()", ( ii < #sortedIndices and ",\n" or "\n" ) )
	end
	writer( [[
	{;}

	//! Default equality test operator.
	inline bool operator==( const ]], t.name, [[& o ) const
	{
		return (
]] )

	for ii, i in ipairs( sortedIndices ) do
		local field = fields[i]
		writer( "\t\t\t", field.name, " == o.", field.name, ( ii < #sortedIndices and " &&\n" or "\n" ) )
	end

	writer( [[
		);
	}

	//! Default inequality test operator.
	inline bool operator!=( const ]], t.name, [[& o ) const { return !( *this == o ); }
};
]] )

end

return template
