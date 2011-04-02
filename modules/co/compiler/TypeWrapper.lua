-------------------------------------------------------------------------------
-- A TypeWrapper is a smart proxy to a real co.IType instance. Its purpose is
-- to augment types with efficient access to extra (computed) fields and
-- methods, useful when writing template scripts.
-------------------------------------------------------------------------------

local utils = require "co.compiler.utils"

local TypeWrapper = {}	-- the TypeWrapper class
local autoFields = {}	-- table of auto-field handlers
local wrappedTypes = {}	-- cache of TypeWrapper instances, indexed by type

-- Auxiliary function to wrap field values, so a chain of nested field
-- accesses will still return properly wrapped co.Types.
local GenericWrapper = {}
local wrappedObjects = {}
local function wrapValue( v )
	local typeOfV = type( v )
	if typeOfV == 'userdata' and v.facet then
		if v.namespace then
			v = TypeWrapper:wrap( v )
		else
			local wrapped = wrappedObjects[v]
			if not wrapped then
				wrapped = setmetatable( { v }, GenericWrapper )
				wrappedObjects[v] = wrapped
			end
			v = wrapped
		end
	elseif typeOfV == 'table' then
		for i = 1, #v do
			v[i] = wrapValue( v[i] )
		end
	end
	return v
end

function GenericWrapper.__index( obj, key )
	local v = wrapValue( obj[1][key] )
	obj[key] = v
	return v
end

function GenericWrapper.__tostring( obj )
	return "GenericWrapper(" .. tostring( obj[1] ) .. ")"
end

function TypeWrapper.__index( t, key )
	-- if key matches an auto field, compute it
	local v = autoFields[key]
	if v then
		v = v( t )
	else
		-- otherwise, if key matches a function in the 'utils' module, use it;
		v = utils[key]
		-- otherwise, resolve it as a member of the _type instance;
		if not v then
			v = wrapValue( t._type[key] )
		end
	end
	-- cache the value for future accesses
	t[key] = v
	return v
end

function TypeWrapper.__tostring( t )
	return "TypeWrapper(" .. tostring( t._type ) .. ")"
end

function TypeWrapper:wrap( type )
	assert( type and type.namespace,  "wrap() expects a co.IType as argument" )
	local self = wrappedTypes[type]
	if not self then
		self = setmetatable( { _type = type }, TypeWrapper )
		wrappedTypes[type] = self
	end
	return self
end

-------------------------------------------------------------------------------
-- Auto-Field Handlers
-------------------------------------------------------------------------------

local typeKindToCpp = {
	TK_ANY		= "co::Any",
	TK_BOOLEAN	= "bool",
	TK_INT8		= "co::int8",
	TK_UINT8	= "co::uint8",
	TK_INT16	= "co::int16",
	TK_UINT16	= "co::uint16",
	TK_INT32	= "co::int32",
	TK_UINT32	= "co::uint32",
	TK_INT64	= "co::int64",
	TK_UINT64	= "co::uint64",
	TK_FLOAT	= "float",
	TK_DOUBLE	= "double",
	TK_STRING	= "std::string",
}

function autoFields.cppName( t )
	if t.kind == 'TK_ARRAY' then
		return t.elementType.cppName .. "[]"
	else
		return typeKindToCpp[t.kind] or utils.toCppName( t.fullName )
	end
end

local typeKindToDoc = {
	TK_ANY		= "co::Any",
	TK_BOOLEAN	= "bool",
	TK_INT8		= "int8",
	TK_UINT8	= "uint8",
	TK_INT16	= "int16",
	TK_UINT16	= "uint16",
	TK_INT32	= "int32",
	TK_UINT32	= "uint32",
	TK_INT64	= "int64",
	TK_UINT64	= "uint64",
	TK_FLOAT	= "float",
	TK_DOUBLE	= "double",
	TK_STRING	= "string",
}

function autoFields.docName( t )
	if t.kind == 'TK_ARRAY' then
		return t.elementType.docName .. "[]"
	else
		return typeKindToDoc[t.kind] or utils.toCppName( t.fullName )
	end
end

function autoFields.fullNameUnderline( t )
	-- my.type.Name -> my_type_Name
	return t.fullName:gsub( '%.', '_' )
end

function autoFields.fullNameUpperUnderline( t )
	-- my.type.Name -> MY_TYPE_NAME
	return t.fullName:upper():gsub( '%.', '_' )
end

local typeKindToInterfaceName = {
	TK_ARRAY		= "IArray",
	TK_ENUM			= "IEnum",
	TK_EXCEPTION	= "IException",
	TK_STRUCT		= "IStruct",
	TK_NATIVECLASS	= "INativeClass",
	TK_INTERFACE	= "IInterface",
	TK_COMPONENT	= "IComponent",
}

function autoFields.typeInterfaceName( t )
	return typeKindToInterfaceName[t.kind] or "IType"
end

function autoFields.headerName( t )
	return utils.toHeaderName( t.fullName )
end

function autoFields.fullSignatureStr( t )
	return t.fullSignature:getString()
end

function autoFields.includedHeaders( t )
	autoFields.traverse( t )
	return rawget( t, 'includedHeaders' )
end

function autoFields.forwardDeclTypes( t )
	autoFields.traverse( t )
	return rawget( t, 'forwardDeclTypes' )
end

function autoFields.typesNeededInReflector( t )
	local forwardDeclTypes = t.forwardDeclTypes
	if t.kind ~= 'TK_INTERFACE' then
		return forwardDeclTypes
	end

	-- copy forwardDeclTypes into a new table
	local neededTypes = {}
	for fullName, type in pairs( forwardDeclTypes ) do
		neededTypes[fullName] = type
	end

	local function addType( type )
		if type.kind == 'TK_ARRAY' then
			type = type.elementType
		end
		local kind = type.kind
		if kind == 'TK_STRUCT' or kind == 'TK_NATIVECLASS' or kind == 'TK_INTERFACE' then
			neededTypes[type.fullName] = type
		end
	end

	-- add forward-decl types from all supertypes
	for i, super in ipairs( t.superTypes ) do
		if super.fullName == 'co.IService' then
			break
		end

		for i, field in ipairs( super.fields ) do
			addType( field.type )
		end

		for i, method in ipairs( super.methods ) do
			local returnType = method.returnType
			if returnType then
				addType( returnType )
			end
			for i, param in ipairs( method.parameters ) do
				addType( param.type )
			end
		end
	end

	return neededTypes
end

-------------------------------------------------------------------------------
-- Helper traversal methods to gather data about a type
-------------------------------------------------------------------------------
local traverse = {}

function autoFields.traverse( t )
	t.includedHeaders = {}
	t.forwardDeclTypes = {}
	assert( traverse[t.kind] )( t )
end

function traverse.TK_ENUM( t )
	-- empty
end

function traverse.TK_EXCEPTION( t )
	t:includeHeader( "co/Exception.h" )
end

function traverse.TK_STRUCT( t )
	for i, field in ipairs( t.fields ) do
		t:includeType( field.type )
		if field.type.kind == 'TK_INTERFACE' then
			t:includeHeader( "co/RefPtr.h" )
		elseif field.type.kind == 'TK_ARRAY' then
			if field.type.elementType.kind == 'TK_INTERFACE' then
				t:includeHeader( "co/RefVector.h" )
			else
				t:includeHeader( "vector" )
			end
		end
	end
end

local function traverseClassType( t )
	for i, a in ipairs( t.fields ) do
		t:includeType( a.type )
	end

	for i, m in ipairs( t.methods ) do
		local returnType = m.returnType
		if returnType then
			t:includeType( returnType )
		end
		for i, p in ipairs( m.parameters ) do
			t:includeType( p.type )
			if p.type.kind == 'TK_ARRAY' then
				if p.type.elementType.kind == 'TK_INTERFACE' then
					t:includeHeader( "co/RefVector.h" )
				else
					t:includeHeader( "vector" )
				end
			end
		end
	end
end

function traverse.TK_NATIVECLASS( t )
	t:includeHeader( t.headerName )
	traverseClassType( t )
end

function traverse.TK_INTERFACE( t )
	traverseClassType( t )
	if t.baseType then
		t:includeHeader( t.baseType )
	end
end

function traverse.TK_COMPONENT( t )
	for i, itf in ipairs( t.ports ) do
		t:includeHeader( itf.type )
	end
end

return TypeWrapper
