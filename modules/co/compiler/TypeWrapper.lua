-------------------------------------------------------------------------------
-- A TypeWrapper is a smart proxy to a real co.Type instance. Its purpose is
-- to augment types with efficient access to extra (computed) fields and
-- methods, useful when writing template scripts.
-------------------------------------------------------------------------------

local utils = require "co.compiler.utils"

local TypeWrapper = {}	-- the TypeWrapper class
local autoFields = {}	-- table of auto-field handlers
local wrappedTypes = {}	-- cache of TypeWrapper instances, indexed by type

-- Auxiliary function to wrap attribute values, so a chain of nested attribute
-- accesses will still return properly wrapped co.Types.
local GenericWrapper = {}
local wrappedObjects = {}
local function wrapValue( v )
	local typeOfV = type( v )
	if typeOfV == 'userdata' and v.interfaceType then
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
	assert( type and type.namespace,  "wrap() expects a co.Type as argument" )
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
	return typeKindToCpp[t.kind] or utils.toCppName( t.fullName )
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
	TK_ARRAY		= "ArrayType",
	TK_ENUM			= "EnumType",
	TK_EXCEPTION	= "ExceptionType",
	TK_STRUCT		= "StructType",
	TK_NATIVECLASS	= "NativeClassType",
	TK_INTERFACE	= "InterfaceType",
	TK_COMPONENT	= "ComponentType",
}

function autoFields.typeInterfaceName( t )
	return typeKindToInterfaceName[t.kind] or "Type"
end

function autoFields.headerName( t )
	return utils.toHeaderName( t.fullName )
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

	-- add forward-decl types from all ancestors
	for i, ancestor in ipairs( t.interfaceAncestors ) do
		if ancestor.fullName ~= 'co.Interface' then
			for i, a in ipairs( ancestor.memberAttributes ) do
				addType( a.type )
			end

			for i, m in ipairs( t.memberMethods ) do
				local returnType = m.returnType
				if returnType then
					addType( returnType )
				end
				for i, p in ipairs( m.parameters ) do
					addType( p.type )
				end
			end
		end
	end

	return neededTypes
end

function autoFields.ambiguousBases( t )
	-- maps each ancestor to the direct superclass of 't' that descends from it
	local ancestors = {}

	-- if an ancestor can be reached via two different superclasses, it's tagged as 'ambiguous'
	local isAmbiguous = {}

	local function gatherAncestors( itf, originalSuper )
		local current = ancestors[itf]
		if not current then
			ancestors[itf] = originalSuper
		elseif current ~= originalSuper then
			isAmbiguous[itf] = true
		end
		for i, super in ipairs( itf.superInterfaces ) do
			gatherAncestors( super, originalSuper )
		end
	end

	for i, super in ipairs( t.superInterfaces ) do
		gatherAncestors( super, super )
	end

	-- remove non-ambiguous entries from 'ancestors'
	for base, super in pairs( ancestors ) do
		if not isAmbiguous[base] then
			ancestors[base] = nil
		end
	end

	return ancestors
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
	t:includeHeader( "co/UserException.h" )
end

function traverse.TK_STRUCT( t )
	for i, field in ipairs( t.memberAttributes ) do
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

local function traverseAttribMethodContainer( t )
	for i, a in ipairs( t.memberAttributes ) do
		t:includeType( a.type )
	end

	for i, m in ipairs( t.memberMethods ) do
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
	traverseAttribMethodContainer( t )
end

function traverse.TK_INTERFACE( t )
	traverseAttribMethodContainer( t )

	for i, super in ipairs( t.superInterfaces ) do
		t:includeHeader( super )
	end
end

function traverse.TK_COMPONENT( t )
	for i, itf in ipairs( t.interfaces ) do
		t:includeHeader( itf.type )
	end
end

return TypeWrapper
