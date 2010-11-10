local tm = co.system.types

local wordSubstitutions = {
	["Type"] = "%Type",
	["Interface"] = "%Interface",
	["Component"] = "%Component",
}

local function addDoc( writer, compoundName, memberName )
	local text = tm:getDocumentation( memberName and ( compoundName .. ":" .. memberName ) or compoundName )
	if text == "" then
		writer( "//! Not documented.\n" )
	else
		-- filter the text, escaping the words "Interface" and "Component" so
		-- they are not turned into links.
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
		addDoc( writer, t.fullName, id )
		writer( "\t", id, ",\n" )
	end
	writer( "};\n" )
end

function write.TK_EXCEPTION( writer, t )
	writer( "exception ", t.name, " : co::Exception {};\n" )
end

function write.TK_STRUCT( writer, t )
	writer( "struct ", t.name, "\n{\n//! \\name Fields\n//@{\n" )
	for i, a in ipairs( t.memberAttributes ) do
		addDoc( writer, t.fullName, a.name )
		writer( "\t", a.type.docName, " ", a.name, ";\n" )
	end
	writer( "//@}\n};\n" )
end

local function writeAttribAndMethods( writer, t )
	writer( "//! \\name Attributes\n//@{\n" )
	for i, a in ipairs( t.memberAttributes ) do
		addDoc( writer, t.fullName, a.name )
		writer( "\t", a.isReadOnly and "readonly " or "", "attribute ", a.type.docName, " ", a.name, ";\n" )
	end
	writer( "//@}\n//! \\name Methods\n//@{\n" )
	for i, m in ipairs( t.memberMethods ) do
		addDoc( writer, t.fullName, m.name )
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
	writeAttribAndMethods( writer, t )
	writer( "};\n" )
end

function write.TK_INTERFACE( writer, t )
	writer( "interface class ", t.name )
	if #t.superInterfaces > 0 then writer " : " end
	for i, s in ipairs( t.superInterfaces ) do
		if i > 1 then writer ", " end
		writer( s.docName )
	end
	writer( "\n{\n" )
	writeAttribAndMethods( writer, t )

	local cppBlock = t.cppBlock
	if cppBlock ~= "" then
		writer( "\n//! \\name Methods only available in C++\n//@{\n", cppBlock, "\n//@}\n" )
	end

	writer( "};\n" )
end

function write.TK_COMPONENT( writer, t )
	writer( "ref class ", t.name, " : co::Component\n{\n" )
	writer( "\t//! \\name Facets\n\t//@{\n\n" )
	for i, itf in ipairs( t.facets ) do
		addDoc( writer, t.fullName, itf.name )
		writer( "\t", itf.type.docName, " ", itf.name, ";\n" )
	end
	writer( "\n\t//@}\n\t//! \\name Receptacles\n\t//@{\n\n" )
	for i, itf in ipairs( t.receptacles ) do
		addDoc( writer, t.fullName, itf.name )
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
	<b>[]], typeGroupName[t.kind], [[]</b> ]], tm:getDocumentation( t.fullName ), '\n', [[
 */
]] )

	c.utils.openNamespaces( writer, t.namespace.fullName )
	writer( "\n" )

	addDoc( writer, t.fullName )
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
