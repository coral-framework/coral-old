local wordSubstitutions = {
	["Type"] = "%Type",
	["Interface"] = "%Interface",
	["Component"] = "%Component",
	["Service"] = "%Service",
}

local coICppBlock = co.Type "co.ICppBlock"
local coIDocumentation = co.Type "co.IDocumentation"

local function getDoc( t, memberName )
	local doc = assert( t[coIDocumentation] )
	if memberName then
		return doc:getDocFor( memberName )
	else
		return doc.value
	end
end

local function addDoc( writer, t, memberName )
	local text = getDoc( t, memberName )
	if text == "" then
		writer( "//! Not documented.\n" )
	else
		-- filter the text, escaping common words that could be turned into links
		text = text:gsub( "(%p?[%w]+)", wordSubstitutions )

		-- generate a \brief with the following criteria: if the first line contains a dot,
		-- the brief is up to the last dot in the first line. Otherwise, the brief is up to
		-- the first dot in the text.
		local line = text:match( "([^\n]+\n?)" )
		local brief = line:match( "(.+%.)" )
		if not brief then
			brief = text:match( "^(.-%.)%s" ) or text
		end
		local details = text:sub( #brief + 1 )
		writer( "/*!\n\t\\brief ", brief )
		if details ~= "" then
			writer( "\n\n\t", details )
		end
		writer( "\n */\n" )
	end
end

local write = {}

function write.TK_ENUM( writer, t )
	writer( "enum ", t.name, "\n{\n" )
	for i, id in ipairs( t.identifiers ) do
		addDoc( writer, t, id )
		writer( "\t", id, ",\n" )
	end
	writer( "};\n" )
end

function write.TK_EXCEPTION( writer, t )
	writer( "exception ", t.name, " : co::Exception {};\n" )
end

function write.TK_STRUCT( writer, t )
	writer( "struct ", t.name, "\n{\n//! \\name Fields\n//@{\n" )
	for i, a in ipairs( t.fields ) do
		addDoc( writer, t, a.name )
		writer( "\t", a.type.docName, " ", a.name, ";\n" )
	end
	writer( "//@}\n};\n" )
end

local function writeFieldsAndMethods( writer, t )
	writer( "//! \\name Fields\n//@{\n" )
	for i, a in ipairs( t.fields ) do
		addDoc( writer, t, a.name )
		writer( "\t", a.isReadOnly and "readonly " or "", a.type.docName, " ", a.name, ";\n" )
	end
	writer( "//@}\n//! \\name Methods\n//@{\n" )
	for i, m in ipairs( t.methods ) do
		addDoc( writer, t, m.name )
		local ret = m.returnType
		writer( "\t", ret and ret.docName or "void", " ", m.name, "(" )
		if #m.parameters > 0 then
			writer( " " )
			for i, p in ipairs( m.parameters ) do
				if i > 1 then writer ", " end
				writer( p.isIn and "in" or "", p.isOut and "out" or "", " ", p.type.docName, " ", p.name )
			end
			writer( " " )
		end
		writer( ")" )
		if #m.exceptions > 0 then
			writer( "\n\t\t\tthrows " )
			for i, e in ipairs( m.exceptions ) do
				if i > 1 then writer ", " end
				writer( e.docName )
			end
		end
		writer( ";\n" )
	end
	writer( "//@}\n" )
end

function write.TK_NATIVECLASS( writer, t )
	writer( "value class ", t.name, "\n{\n" )
	writeFieldsAndMethods( writer, t )
	writer( "};\n" )
end

function write.TK_INTERFACE( writer, t )
	writer( "interface class ", t.name )
	if t.baseType then
		writer( " : ", t.baseType.docName )
	end
	writer( "\n{\n" )
	writeFieldsAndMethods( writer, t )

	local cppBlock = t[coICppBlock]
	if cppBlock then
		writer( "\n//! \\name Methods only available in C++\n//@{\n", cppBlock.value, "\n//@}\n" )
	end

	writer( "};\n" )
end

function write.TK_COMPONENT( writer, t )
	writer( "ref class ", t.name, " : co::IObject\n{\n" )
	writer( "\t//! \\name Facets\n\t//@{\n\n" )
	for i, itf in ipairs( t.facets ) do
		addDoc( writer, t, itf.name )
		writer( "\t", itf.type.docName, " ", itf.name, ";\n" )
	end
	writer( "\n\t//@}\n\t//! \\name Receptacles\n\t//@{\n\n" )
	for i, itf in ipairs( t.receptacles ) do
		addDoc( writer, t, itf.name )
		writer( "\t", itf.type.docName, " ", itf.name, ";\n" )
	end
	writer( "\n\t//@}\n};\n" )
end

local typeGroupName = {
	TK_ENUM = "enums",
	TK_EXCEPTION = "exceptions",
	TK_STRUCT = "structs",
	TK_NATIVECLASS = "nativeclasses",
	TK_INTERFACE = "interfaces",
	TK_COMPONENT = "components",
}

local function template( writer, c, t )
	writer( [[
/*!
	\file
	<b>[]], typeGroupName[t.kind], [[]</b> ]], getDoc( t ), '\n', [[
 */
]] )

	c.utils.openNamespaces( writer, t.namespace.fullName )
	writer( "\n" )

	addDoc( writer, t )
	writer( [[/*!
	\ingroup coral_]], typeGroupName[t.kind], "\n", [[
	\nosubgrouping
 */
]] )
	assert( write[t.kind] )( writer, t )

	writer( "\n" )
	c.utils.closeNamespaces( writer, t.namespace.fullName )
end

return template
